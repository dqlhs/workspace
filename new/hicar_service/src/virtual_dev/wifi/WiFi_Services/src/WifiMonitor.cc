#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <memory.h>
#include <glibmm.h>

#ifdef __cplusplus
extern "C" {
#include "os.h"
}
#else
#include "os.h"
#endif
//extern int os_get_time(struct os_time *t);

#include <WifiMonitor.h>
#include <wpa_ctrl.h>

namespace connectivity {

static WifiMonitor *gInstance = nullptr;

const string WifiMonitor::TAG = "WifiMonitor";

unsigned long WifiMonitor:: GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	unsigned long temp	= ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	return temp;
}

WifiMonitor::WifiMonitor()
{
	mNative = WifiNative::getInstance();
	mNative->wifi_connect_to_supplicant();

	mService = WifiServiceImpl::getInstance();

	mScanResults = g_variant_new("a(ssiiiasb)",
				g_variant_builder_new(G_VARIANT_TYPE("a(ssiiiasb)")));

	mCmdEventFD = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE);
	mRetEventFD = eventfd(0, EFD_CLOEXEC);

	mThread = new thread(&WifiMonitor::run, this);
}

WifiMonitor::~WifiMonitor()
{
	mThread->join();
	delete mThread;

	close(mCmdEventFD);
	close(mRetEventFD);

	g_variant_unref(mScanResults);
}

// Not thread-safe
WifiMonitor *WifiMonitor::getInstance()
{
	return gInstance ? gInstance : (gInstance = new WifiMonitor());
}

// Not thread-safe
void WifiMonitor::destroy()
{
	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	mLoop->quit();

	gInstance = nullptr;
	delete this;
}

bool WifiMonitor::onCmdQueue(const Glib::IOCondition& condition)
{
	uint64_t count;

	read(mCmdEventFD, &count, sizeof count);

	auto msg = mCmdQueue.front();
	mCmdQueue.pop();

	if (*msg->first == "scan")
		onStartScan(msg->second);
	else if (*msg->first == "connect")
		onConnect(msg->second);
	else if (*msg->first == "disconnect")
		onDisconnect(msg->second);
	else if (*msg->first == "getConnectionInfo")
		onGetConnectionInfo(msg->second);
	else if (*msg->first == "getSavedNetworks")
		onGetSavedNetworks(msg->second);
	else if (*msg->first == "signalPoll")
		onSignalPoll(msg->second);

	delete msg->first;
	delete msg;

	return true;
}

void WifiMonitor::run()
{
	auto self = Glib::MainContext::create();

	// WifiMonitor inherits sigc::trackable, so it is safe to not call disconnect
	self->signal_io().connect(
		sigc::mem_fun(*this, &WifiMonitor::onCmdQueue),
					mCmdEventFD, Glib::IOCondition::IO_IN);
	self->signal_io().connect(
		sigc::mem_fun(*this, &WifiMonitor::onSupplicantEvent),
			mNative->wifi_get_mon_fd(), Glib::IOCondition::IO_IN);

	mLoop = Glib::MainLoop::create(self);
	mLoop->run();

	if (mConnected) {
		cout << TAG << "::" << __func__ << ": Releasing DHCP..." << endl;
		// release
		system("kill -USR2 `cat /var/run/udhcpc.pid`");
		// terminate
		system("kill `cat /var/run/udhcpc.pid`");
	}

	// Initiate "disconnect" request
	mNative->wifi_close_supplicant_connection();
}

bool WifiMonitor::onSupplicantEvent(const Glib::IOCondition& condition)
{
	char buffer[4096];

	string event = mNative->wifi_wait_for_event(buffer, 4096);
	if (event == WPA_EVENT_SCAN_RESULTS) {
		{
			unique_lock<mutex> lock(mResultsLock);
			g_variant_unref(mScanResults);
			mScanResults = mNative->wifi_get_scan_results();
		}
		onScanCompleted(true);
	} else if (event.find(WPA_EVENT_SCAN_FAILED) != event.npos) {
		// We silently eat this event
		onScanCompleted(false);
	} else if (event == WPA_EVENT_SCAN_STARTED) {
		mService->stateChanged(WIFI_STATE_SCANNING, mService->getCurrentState());
	} else if (event.find(WPA_EVENT_DISCONNECTED) != event.npos) {
		if (!mConnected)
			return true;

		mConnected = false;
		// release
		system("kill -USR2 `cat /var/run/udhcpc.pid`");
		// terminate
		system("kill `cat /var/run/udhcpc.pid`");
		mService->stateChanged(WIFI_STATE_DISCONNECTED, WIFI_STATE_DISCONNECTING);
	} else if (event.find(AP_STA_CONNECTED) != event.npos) {

			cout << TAG << "::" << __func__ << ": waiting for client connected...CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC" << endl;

	} else if (event.find(WPA_EVENT_CONNECTED) != event.npos) {
		mService->stateChanged(WIFI_STATE_CONNECTING, WIFI_STATE_ENABLED);

		/*
		 * succ: udhcpc will run in background (will be terminated during disconnecting)
		 * fail: udhcpc will exit
		 */
		system("udhcpc -t 5 -T 6 -f -i wlan0 -p /var/run/udhcpc.pid -n &");

		{
		        char ret;

		        // will block until udhcpc notification received
		        cout << TAG << "::" << __func__ << ": waiting for DHCP..." << endl;
        		auto dhcp = open("/var/run/dhcp_done.pipe", O_RDONLY | O_CLOEXEC);
	        	read(dhcp, &ret, sizeof ret);
		        close(dhcp);

		        if (ret == '1') {
		        	cout << TAG << "::" << __func__ << ": DHCP done (succ)" << endl;
		        	mNative->notify_dhcp_result(true);
				mService->stateChanged(WIFI_STATE_CONNECTED, WIFI_STATE_CONNECTING);
				mConnected = true;
		        } else {
		        	cout << TAG << "::" << __func__ << ": DHCP done (fail)" << endl;
		        	mNative->notify_dhcp_result(false);
				mService->stateChanged(WIFI_STATE_FAILED, WIFI_STATE_CONNECTING);
				// TODO: reason
        		}
		}
	}

	return true;
}

void WifiMonitor::onScanCompleted(bool succ)
{
	cout << TAG << "::" << __func__ << ": " << (succ ? "succ" : "fail") << endl;

	mService->stateChanged(succ ? WIFI_STATE_SCAN_DONE :
					WIFI_STATE_FAILED, WIFI_STATE_SCANNING);

	unique_lock<mutex> lock(mWIPLock);
	mWorkInProgress = false;
	lock.unlock();
	mWorkDone.notify_one();
}

void WifiMonitor::onStartScan(GVariant *param)
{
	uint64_t count = 1;
	char reply[8] = { 0, };
	size_t len = sizeof(reply) - 1;

	cout << TAG << "::" << __func__ << endl;

	// OK or FAIL
	mNative->wifi_command("SCAN", reply, &len);

	if (string(reply) == "OK\n") {
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.Succ"),
							g_variant_new("s", "None")));
	} else {
		// No one waiting on mWorkDone
		mWorkInProgress = false;

		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.ScanRejected"),
			g_variant_new("s", "The scan request was rejected by Wi-Fi stack")));
	}

	// wakeup WifiMonitor::startScan
	write(mRetEventFD, &count, sizeof count);
}

WifiMonitor::WifiMessage *WifiMonitor::startScan()
{
	uint64_t count = 1;

	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);

		mWorkInProgress = true;
	}

	auto msg = new WifiMessage(new string("scan"), nullptr);

	mCmdQueue.push(msg);
	(void)write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
	msg = mRetQueue.front();
	mRetQueue.pop();

	return msg;
}

GVariant *WifiMonitor::getScanResults()
{
	unique_lock<mutex> lock(mResultsLock);
	g_variant_ref(mScanResults);
	return mScanResults;
}

void WifiMonitor::onConnect(GVariant *param)
{
	uint64_t count = 1;
	int ret;

	auto paramType = string(g_variant_get_type_string(param));

	if (paramType == "(i)") {
		gint netid;

		g_variant_get(param, "(i)", &netid);
		cout << TAG << "::" << __func__ << ": " << "netid=" << netid << endl;

		ret = mNative->wifi_select_network(netid);
		cout << TAG << "::" << __func__ << ": " << "select ret=" << ret << endl;
	} else {
		gchar *ssid, *psk;

		g_variant_get(param, "(ss)", &ssid, &psk);
		cout << TAG << "::" << __func__ << ": " << "ssid=" << ssid << ", psk=" << psk << endl;

		ret = mNative->wifi_connect_network(string(ssid), string(psk));
		cout << TAG << "::" << __func__ << ": " << "connect ret=" << ret << endl;
	}

	g_variant_unref(param);

	switch (ret) {
	case SUCC: // succ
		break;
	case ERR_PASSWD_FORMAT: // ERR_PASSWD_FORMAT
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.WrongPassword"),
						g_variant_new("s", "Password is not valid")));
		goto out;
	case ERR_CONN_NET_PASSWD_INCORRECT: // ERR_CONN_NET_PASSWD_INCORRECT
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.WrongPassword"),
						g_variant_new("s", "Password is wrong")));
		goto out;
	case ERR_CONN_NET_AP_DISABLE: // ERR_CONN_NET_AP_DISABLE
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.ConnectFailed"),
				g_variant_new("s", "The AP being connected to is temporarily down")));
		goto out;
	case ERR_CONN_TIMEOUT: // TIMEOUT
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.ConnectFailed"),
								g_variant_new("s", "Timeout")));
		goto out;
	case ERR_INVALID_NETID:
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.ConnectFailed"),
							g_variant_new("s", "Invalid network ID")));
		goto out;
	default:
		mRetQueue.push(new WifiMessage(
			new string("com.hsaeyz.sal.WifiService.Error.Unknown"),
							g_variant_new("s", "Internal error")));
		goto out;
	}

	/*
	 * succ: udhcpc will run in background (will be terminated during disconnecting)
	 * fail: udhcpc will exit
	 */
    system("udhcpc -t 5 -T 6 -f -i wlan0 -p /var/run/udhcpc.pid -n &");

    {
        int dhcp;
        char ret;

        // will block until udhcpc notification received
        cout << TAG << "::" << __func__ << ": waiting for DHCP..." << endl;
			cout << TAG << "::" <<	__func__<< ": DHCP start BBBBBBB , "<< GetTickCount() << endl;
        dhcp = open("/var/run/dhcp_done.pipe", O_RDONLY | O_CLOEXEC);
        read(dhcp, &ret, sizeof ret);
        close(dhcp);

			cout << TAG << "::" <<	__func__<< ": DHCP end BBBBBBB , "<< GetTickCount() << endl;
        if (ret == '1') {
            cout << TAG << "::" << __func__ << ": DHCP done (succ)" << endl;
            mNative->notify_dhcp_result(true);
        } else {
            cout << TAG << "::" << __func__ << ": DHCP done (fail)" << endl;
            mNative->notify_dhcp_result(false);
            mRetQueue.push(new WifiMessage(
		new string("com.hsaeyz.sal.WifiService.Error.DhcpFailed"),
		g_variant_new("s", "Failed to obtain network information via DHCP")));
            goto out;
        }
    }

	mConnected = true;

	mRetQueue.push(new WifiMessage(
		new string("com.hsaeyz.sal.WifiService.Error.Succ"),
						g_variant_new("s", "None")));

out:
	write(mRetEventFD, &count, sizeof count);
}

WifiMonitor::WifiMessage *WifiMonitor::connect(GVariant *param)
{
	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	uint64_t count = 1;

	auto msg = new WifiMessage(new string("connect"), param);

	mCmdQueue.push(msg);
	write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
	msg = mRetQueue.front();
	mRetQueue.pop();

	return msg;
}

WifiMonitor::WifiMessage *WifiMonitor::connect(const string& ssid, const string& psk)
{
	return connect(g_variant_new("(ss)", ssid.c_str(), psk.c_str()));
}

WifiMonitor::WifiMessage *WifiMonitor::connect(gint netid)
{
	return connect(g_variant_new("(i)", netid));
}

void WifiMonitor::onDisconnect(GVariant *param)
{
	uint64_t count = 1;

	cout << TAG << "::" << __func__ << endl;

	// 0 - succ
	mNative->wifi_disconnect_network(false);

	mConnected = false;

	// release
	system("kill -USR2 `cat /var/run/udhcpc.pid`");
	// terminate
	system("kill `cat /var/run/udhcpc.pid`");

	write(mRetEventFD, &count, sizeof count);
}

void WifiMonitor::disconnect()
{
	uint64_t count = 1;

	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	auto msg = new WifiMessage(new string("disconnect"), nullptr);

	mCmdQueue.push(msg);
	write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
}

void WifiMonitor::onGetConnectionInfo(GVariant *param)
{
	uint64_t count = 1;

	cout << TAG << "::" << __func__ << endl;

	mRetQueue.push(new WifiMessage(nullptr, mNative->wifi_get_connection_info()));
	write(mRetEventFD, &count, sizeof count);
}

GVariant *WifiMonitor::getConnectionInfo()
{
	uint64_t count = 1;

	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	auto msg = new WifiMessage(new string("getConnectionInfo"), nullptr);

	mCmdQueue.push(msg);
	write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
	msg = mRetQueue.front();
	mRetQueue.pop();

	auto info = msg->second;
	delete msg;

	return info;
}

void WifiMonitor::ensureAutoScan()
{
	if (mNative->wifi_check_auto_scan())
		startScan();
	else
		mWorkInProgress = true;
}

void WifiMonitor::onGetSavedNetworks(GVariant *param)
{
	uint64_t count = 1;

	cout << TAG << "::" << __func__ << endl;

	mRetQueue.push(new WifiMessage(nullptr, mNative->wifi_get_saved_networks()));
	write(mRetEventFD, &count, sizeof count);
}

GVariant *WifiMonitor::getSavedNetworks()
{
	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	uint64_t count = 1;

	auto msg = new WifiMessage(new string("getSavedNetworks"), nullptr);

	mCmdQueue.push(msg);
	write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
	msg = mRetQueue.front();
	mRetQueue.pop();

	auto networks = msg->second;
	delete msg;

	return networks;
}

void WifiMonitor::onSignalPoll(GVariant *param)
{
	cout << TAG << "::" << __func__ << endl;

	uint64_t count = 1;

	mRetQueue.push(new WifiMessage(nullptr, mNative->wifi_get_signal_strength()));
	write(mRetEventFD, &count, sizeof count);
}

GVariant *WifiMonitor::signalPoll()
{
	{
		unique_lock<mutex> lock(mWIPLock);
		if (mWorkInProgress)
			// no spurious wakeup here, no need to check mWorkInProgress again
			mWorkDone.wait(lock);
	}

	uint64_t count = 1;

	auto msg = new WifiMessage(new string("signalPoll"), nullptr);

	mCmdQueue.push(msg);
	write(mCmdEventFD, &count, sizeof count);

	read(mRetEventFD, &count, sizeof count);
	msg = mRetQueue.front();
	mRetQueue.pop();

	auto signal = msg->second;
	delete msg;

	return signal;
}

}
