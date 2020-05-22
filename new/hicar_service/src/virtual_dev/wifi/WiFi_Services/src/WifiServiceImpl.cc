#include <iostream>

//#include <unistd.h>
//#include <sys/types.h>
#include <sys/eventfd.h>
#include <string.h>
#include <memory.h>
#include <pthread.h>
//#include <common.h>

#ifdef __cplusplus
extern "C" {
#include "os.h"
}
#else
#include "os.h"
#endif
//extern int os_get_time(struct os_time *t);


#include <WifiServiceImpl.h>

namespace connectivity {

static WifiServiceImpl *gInstance = nullptr;

string WifiServiceImpl::TAG = "WifiService";

WifiServiceImpl::WifiServiceImpl()
{
}

WifiServiceImpl::~WifiServiceImpl()
{
}

unsigned long WifiServiceImpl:: GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	unsigned long temp	= ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	return temp;
}

// Not thread-safe
WifiServiceImpl *WifiServiceImpl::getInstance()
{
	return gInstance ? gInstance :
			(gInstance = new WifiServiceImpl());
}

// Not thread-safe
void WifiServiceImpl::destroy()
{
	gInstance = nullptr;
	delete this;
}

void WifiServiceImpl::onBusAcquired(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
}

void WifiServiceImpl::onNameAcquired(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
	mSkeleton = wifi_service_skeleton_new();

	g_signal_connect(mSkeleton, "handle-set-wifi-enabled",
			G_CALLBACK(onMethodCall), new string("handle-set-wifi-enabled"));
	g_signal_connect(mSkeleton, "handle-connect",
			G_CALLBACK(onMethodCall), new string("handle-connect"));
	/* hostap */
	g_signal_connect(mSkeleton, "handle-start-host-ap",
			G_CALLBACK(onMethodCall), new string("handle-startHostAp"));
	g_signal_connect(mSkeleton, "handle-stop-host-ap",
			G_CALLBACK(onMethodCall), new string("handle-stopHostAp"));
	/* station signal */			
	g_signal_connect(mSkeleton, "handle-addsta",
			G_CALLBACK(onMethodCall), new string("handle-add-new-sta"));
	g_signal_connect(mSkeleton, "handle-deletesta",
			G_CALLBACK(onMethodCall), new string("handle-deletesta"));
	g_signal_connect(mSkeleton, "handle-update-host-ap-mac",
			G_CALLBACK(onMethodCall), new string("handle-update-host-ap-mac"));


	g_signal_connect(mSkeleton, "handle-set-periodic-scan-interval",
			G_CALLBACK(onMethodCall), new string("handle-set-periodic-scan-interval"));
	g_signal_connect(mSkeleton, "handle-select-network",
			G_CALLBACK(onMethodCall), new string("handle-select-network"));
	g_signal_connect(mSkeleton, "handle-start-scan",
			G_CALLBACK(onMethodCallNoArg), new string("handle-start-scan"));
	g_signal_connect(mSkeleton, "handle-get-scan-results",
			G_CALLBACK(onMethodCallNoArg), new string("handle-get-scan-results"));
	g_signal_connect(mSkeleton, "handle-disconnect",
			G_CALLBACK(onMethodCallNoArg), new string("handle-disconnect"));
	g_signal_connect(mSkeleton, "handle-is-wifi-enabled",
			G_CALLBACK(onMethodCallNoArg), new string("handle-is-wifi-enabled"));
	g_signal_connect(mSkeleton, "handle-get-connection-info",
			G_CALLBACK(onMethodCallNoArg), new string("handle-get-connection-info"));
	g_signal_connect(mSkeleton, "handle-get-host-ap-client-info",
			G_CALLBACK(onMethodCallNoArg), new string("handle-gethostapclientinfo"));
	g_signal_connect(mSkeleton, "handle-get-saved-networks",
			G_CALLBACK(onMethodCallNoArg), new string("handle-get-saved-networks"));

	wifi_service_set_state(mSkeleton, WIFI_STATE_DISABLED);

	//g_signal_connect(mSkeleton, "notify::periodic-scan-interval",
	//		G_CALLBACK(onPropertyChanged), new string("periodic-scan-interval"));

	mStateChanged = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE);
	Glib::signal_io().connect(
		sigc::mem_fun(*this, &WifiServiceImpl::onStateChanged),
			mStateChanged, Glib::IOCondition::IO_IN, Glib::PRIORITY_HIGH);

	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(mSkeleton),
			Glib::unwrap(connection), "/com/hsaeyz/sal/WifiService", NULL);
}

void WifiServiceImpl::onNameLost(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
	cout << "onNameLost" << endl;
	mLoop->quit();
}

gboolean WifiServiceImpl::onMethodCall(WifiService *skeleton,
	GDBusMethodInvocation *invocation, GVariant *container, gpointer signal)
{
	string& signalName = *(static_cast<string *>(signal));
	WifiServiceImpl *service = getInstance();

	if (signalName == "handle-set-wifi-enabled")
		return service->setWifiEnabled(skeleton, invocation, container);
	else if(signalName == "handle-startHostAp")
		return service->startHostAp(skeleton,invocation,container);
	else if(signalName == "handle-stopHostAp")
		return service->stopHostAp(skeleton,invocation,container);

	else if(signalName == "handle-add-new-sta")
		return service->addNewSta(skeleton,invocation,container);
	else if(signalName == "handle-deletesta")
		return service->deleteOldSta(skeleton,invocation,container);
	else if(signalName == "handle-update-host-ap-mac")
		return service->updatehostapMac(skeleton,invocation,container);
		
	else if (signalName == "handle-connect")
		return service->connect(skeleton, invocation, container);
	else if (signalName == "handle-set-periodic-scan-interval")
		return service->setPeriodicScanInterval(skeleton, invocation, container);
	else if (signalName == "handle-select-network")
		return service->connect(skeleton, invocation, container);

	g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.NonExist",
					"The method being called does not exist");

	return TRUE;
}

gboolean WifiServiceImpl::onMethodCallNoArg(WifiService *skeleton,
			GDBusMethodInvocation *invocation, gpointer signal)
{
	string& signalName = *(static_cast<string *>(signal));
	WifiServiceImpl *service = getInstance();

	if (signalName == "handle-start-scan")
		return service->startScan(skeleton, invocation);
	else if (signalName == "handle-get-scan-results")
		return service->getScanResults(skeleton, invocation);
	else if (signalName == "handle-disconnect")
		return service->disconnect(skeleton, invocation);
	else if (signalName == "handle-is-wifi-enabled")
		return service->isWifiEnabled(skeleton, invocation);
	else if (signalName == "handle-get-connection-info")
		return service->getConnectionInfo(skeleton, invocation);
	/* start
	 * hostap */
	else if (signalName == "handle-gethostapclientinfo")
		return service->getConnectionInfo(skeleton, invocation);
	/* end */
	else if (signalName == "handle-get-saved-networks")
		return service->getSavedNetworks(skeleton, invocation);

	//g_dbus_method_invocation_return_error_literal(invocation, G_IO_ERROR, G_IO_ERROR_EXISTS, "Non-exist");
	g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.NonExist",
					"The method being called does not exist");

	return TRUE;
}

/*
gboolean WifiServiceImpl::onPropertyChanged(WifiService *skeleton,
					GParamSpec *pspec, gpointer property)
{
	string& propertyName = *(static_cast<string *>(property));
	WifiServiceImpl *service = getInstance();

	if (propertyName == "periodic-scan-interval")
		cout << service->TAG << "::" << __func__ << ": Property \"PeriodicScanInterval\" changed" << endl;

	return TRUE;
}
*/

gboolean WifiServiceImpl::setWifiEnabled(WifiService *skeleton,
		GDBusMethodInvocation *invocation, GVariant *container)
{
	gboolean enabled;

	g_variant_get(container, "(b)", &enabled);

	cout << TAG << "::" << __func__ << ": " << (enabled ? "enable" : "disable") << endl;
	goto done;

	if (enabled) {
		if (mState != WIFI_STATE_DISABLED)
			// nothing actually happened, ignore it
			goto done;

		wifi_service_emit_wifi__state__changed(skeleton,
				WIFI_STATE_ENABLING, WIFI_STATE_DISABLED);

		mNative = WifiNative::getInstance();
		if(mNative->wifi_start_supplicant())
		{
			cout << TAG << "::" << __func__ << ": Cannot start the wpa_supplicant " << "ERROR ! ERROR ! ERROR " << endl;
			return TRUE;
		}
		mMonitor = WifiMonitor::getInstance();

		mState = WIFI_STATE_ENABLED;
		wifi_service_set_state(skeleton, WIFI_STATE_ENABLED);
		wifi_service_emit_wifi__state__changed(skeleton,
				WIFI_STATE_ENABLED, WIFI_STATE_ENABLING);

//		mMonitor->ensureAutoScan();

		auto interval = wifi_service_get_periodic_scan_interval(skeleton);
		if (interval)
			mPeriodicScan = new sigc::connection(
				Glib::signal_timeout().connect_seconds(
					sigc::mem_fun(*this, &WifiServiceImpl::onPeriodicScan), interval));
	} else {
		if (mState == WIFI_STATE_DISABLED)
			// nothing actually happened, ignore it
			goto done;

		if (mState == WIFI_STATE_CONNECTED) {
			wifi_service_emit_wifi__state__changed(skeleton,
					WIFI_STATE_DISCONNECTING, WIFI_STATE_CONNECTED);

//			mSignalPoll->disconnect();
//			delete mSignalPoll;
			mSignalLevel = -9999;

			mMonitor->disconnect();

			wifi_service_emit_wifi__state__changed(skeleton,
					WIFI_STATE_DISCONNECTED, WIFI_STATE_DISCONNECTING);
			wifi_service_emit_wifi__state__changed(skeleton,
					WIFI_STATE_ENABLED, WIFI_STATE_DISCONNECTED);
		}

		wifi_service_emit_wifi__state__changed(skeleton,
				WIFI_STATE_DISABLING, WIFI_STATE_ENABLED);

		if (mPeriodicScan) {
			mPeriodicScan->disconnect();
			delete mPeriodicScan;
			mPeriodicScan = nullptr;
			// Don't clear the periodic scan interval
		}

		mMonitor->destroy();
		mNative->wifi_stop_supplicant();
		mNative->destroy();

		mState = WIFI_STATE_DISABLED;
		wifi_service_set_state(skeleton, WIFI_STATE_DISABLED);

		wifi_service_emit_wifi__state__changed(skeleton,
				WIFI_STATE_DISABLED, WIFI_STATE_DISABLING);
	}

done:
	wifi_service_complete_set_wifi_enabled(skeleton, invocation);

	return TRUE;
}

WifiMonitor::WifiMessage *WifiServiceImpl::startScan()
{
	//wifi_service_emit_wifi__state__changed(mSkeleton,
	//				WIFI_STATE_SCANNING, mState);
	
	// Will return right after initiating a new scan
	auto msg = mMonitor->startScan();
	if (*msg->first != "com.hsaeyz.sal.WifiService.Error.Succ") {
		wifi_service_emit_wifi__state__changed(mSkeleton,
					WIFI_STATE_FAILED, WIFI_STATE_SCANNING);
		wifi_service_emit_wifi__state__changed(mSkeleton,
						mState, WIFI_STATE_FAILED);
	}

	return msg;
}

gboolean WifiServiceImpl::startScan(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	struct os_time start,end;

	cout << TAG << "::" <<  __func__ << endl;
	
	os_get_time(&start);
	cout << __func__ << " start time:sec " << start.sec << " usec "<<start.usec << endl;
	
	if ((mState == WIFI_STATE_DISABLED) ||
		(mState == WIFI_STATE_CONNECTING) ||
		(mState == WIFI_STATE_DISABLING) ||
		(mState == WIFI_STATE_SCANNING))
	{
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"The method cannot be called when Wi-Fi is DISABLED,DISABLING,ONNECTING,SCANNING,SCAN_DONE");
		ScantimesFail++;
		cout << __func__<<  " :: the state is " + mState << endl;
		return TRUE;
	}

	auto msg = startScan();
	if (*msg->first == "com.hsaeyz.sal.WifiService.Error.Succ") {
		wifi_service_complete_start_scan(skeleton, invocation);
		// SCANNING -> SCAN_DONE -> mState in WifiServiceImpl::onStateChanged
	} else {
		gchar *detail;

		g_variant_get(msg->second, "s", &detail);
		g_dbus_method_invocation_return_dbus_error(invocation, msg->first->c_str(), detail);

		cout << TAG << "::" << __func__ << ": error = " << *msg->first << endl;
	}

	g_variant_unref(msg->second);
	delete msg->first;
	delete msg;
	
	os_get_time(&end);
	cout << __func__ << " end time:sec " << end.sec << " usec "<<end.usec << endl;
	
	return TRUE;
}

gboolean WifiServiceImpl::getScanResults(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	struct os_time start,end;

	cout << TAG << "::" <<  __func__ << endl;

	if (mState == WIFI_STATE_DISABLED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"The method cannot be called when Wi-Fi is disabled");
		return TRUE;
	}

	wifi_service_complete_get_scan_results(skeleton, invocation, mMonitor->getScanResults());

	return TRUE;
}

gboolean WifiServiceImpl::connect(WifiService *skeleton,
		GDBusMethodInvocation *invocation, GVariant *container)
{
	unsigned long start,end;

	start = GetTickCount();
	cout << TAG << "::" <<  __func__<< " start BBBBBBB, "<< start << endl;
	
		wifi_service_complete_connect(skeleton, invocation);

	/* cannel the connect */
	if (mState == WIFI_STATE_DISABLED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"The method cannot be called when Wi-Fi is disabled");
		cout << __func__ << ": The method cannot be called when Wi-Fi is disabled  now start it" << endl;	
		setWifiEnabled(skeleton,invocation,container);
		return TRUE;
	}
	
	else if (mState == WIFI_STATE_CONNECTED) {
		
		auto info = mMonitor->getConnectionInfo();
		if(info)
		{
				
			gchar *ssidtemp, *ssidtemp1, *bssidtemp, *authtemp, *iptemp, *gwtemp,*psktemp1;
			gint channeltemp, freqtemp, rssitemp, leveltemp, linkSpeedtemp;
			
			g_variant_get(info, "(ssiisiiiss)", &ssidtemp, &bssidtemp, &channeltemp,
					&freqtemp, &authtemp, &rssitemp, &leveltemp, &linkSpeedtemp, &iptemp, &gwtemp);
			#if 0
			printf("connection ssid: %s\n", ssid);
			printf("connection bssid: %s\n", bssid);
			printf("connection channel: %d\n", channel);
			printf("connection freq: %d\n", freq);
			printf("connection auth: %s\n", auth);
			printf("connection rssi: %d\n", rssi);
			printf("connection level: %d\n", level);
			printf("connection linkspeed: %d\n", linkSpeed);
			printf("connection ip: %s\n", ip);
			printf("connection gateway: %s\n", gw);
			#endif
			
			g_variant_get(container, "(ss)", &ssidtemp1, &psktemp1);
			if(!strcmp(ssidtemp,ssidtemp1))
			{
				g_dbus_method_invocation_return_dbus_error(invocation,
						"com.hsaeyz.sal.WifiService.Error.IllegalState",
							"Already connected");
				
				cout << TAG << ":: " <<__func__<<": The method cannot be called Already connected" << endl;	
				
				return TRUE;
			}
			else
			{
				
					wifi_service_emit_wifi__state__changed(skeleton,
							WIFI_STATE_DISCONNECTING, WIFI_STATE_DISCONNECTED);
				
					mSignalLevel = -9999;
				
					mMonitor->disconnect();
				
					wifi_service_complete_disconnect(skeleton, invocation);
				
					wifi_service_emit_wifi__state__changed(skeleton,
							WIFI_STATE_DISCONNECTED, WIFI_STATE_DISCONNECTING);
			}
				
		}
		

	}
	//#endif
	else if(mState == WIFI_STATE_CONNECTING)
	{
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"Connecting");
		cout << __func__ << ": The method cannot be called Connecting" << endl;	
		return TRUE;
	}

	wifi_service_emit_wifi__state__changed(mSkeleton,
			WIFI_STATE_CONNECTING, WIFI_STATE_ENABLED);

	WifiMonitor::WifiMessage *msg;

	auto paramType = string(g_variant_get_type_string(container));

	cout << TAG << "::" << __func__ << ": ";
	if (paramType == "(i)") {
		gint netid;

		g_variant_get(container, "(i)", &netid);

		cout << netid << endl;

		msg = mMonitor->connect(netid);
	} else {
		gchar *ssid, *psk;

		g_variant_get(container, "(ss)", &ssid, &psk);

		cout << ssid << ", " << psk << endl;

		msg = mMonitor->connect(string(ssid), string(psk));
	}

	//printf("Wifi Service error = %s\n", (*msg->first).c_str());
	cout << TAG << "::" << __func__ << ": error = " << *msg->first << endl;
	if (*msg->first == "com.hsaeyz.sal.WifiService.Error.Succ") {
		mState = WIFI_STATE_CONNECTED;
		wifi_service_set_state(skeleton, WIFI_STATE_CONNECTED);

		wifi_service_emit_wifi__state__changed(mSkeleton,
				WIFI_STATE_CONNECTED, WIFI_STATE_CONNECTING);

		paramType == "(i)" ?
			wifi_service_complete_select_network(skeleton, invocation) :
					wifi_service_complete_connect(skeleton, invocation);

		if (mPeriodicScan) {
			mPeriodicScan->disconnect();
			delete mPeriodicScan;
			mPeriodicScan = nullptr;
		}

		end = GetTickCount();
		
		cout << TAG << "::" <<	__func__<< " success BBBBBBB, "<< end << " wifi connect : " << end - start << endl;
		//mConnectTime[START] = start;
		//mConnectTime[END] = end;

		// perform SSD every 5 seconds
//		mSignalPoll = new sigc::connection(
//			Glib::signal_timeout().connect_seconds(
//				sigc::mem_fun(*this, &WifiServiceImpl::onSignalPoll), 5));
	} else {
		gchar *detail;

		g_variant_get(msg->second, "s", &detail);
		g_dbus_method_invocation_return_dbus_error(invocation, msg->first->c_str(), detail);

		wifi_service_emit_wifi__state__changed(mSkeleton,
				WIFI_STATE_FAILED, WIFI_STATE_CONNECTING);
		wifi_service_emit_wifi__state__changed(mSkeleton,
				WIFI_STATE_ENABLED, WIFI_STATE_FAILED);
	}

	g_variant_unref(msg->second);
	delete msg->first;
	delete msg;

	return TRUE;
}

gboolean WifiServiceImpl::disconnect(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	cout << TAG << "::" << __func__ << endl;

	if (mState == WIFI_STATE_DISABLED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"The method cannot be called when Wi-Fi is disabled");
		return TRUE;
	}

	if (mState != WIFI_STATE_CONNECTED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"Not connected");
		return TRUE;
	}

	wifi_service_emit_wifi__state__changed(skeleton,
			WIFI_STATE_DISCONNECTING, WIFI_STATE_DISCONNECTED);

//	mSignalPoll->disconnect();
//	delete mSignalPoll;
	mSignalLevel = -9999;

	mMonitor->disconnect();

	wifi_service_complete_disconnect(skeleton, invocation);

	wifi_service_emit_wifi__state__changed(skeleton,
			WIFI_STATE_DISCONNECTED, WIFI_STATE_DISCONNECTING);

	auto interval = wifi_service_get_periodic_scan_interval(skeleton);
	if (interval)
		mPeriodicScan = new sigc::connection(
			Glib::signal_timeout().connect_seconds(
				sigc::mem_fun(*this, &WifiServiceImpl::onPeriodicScan), interval));

	mState = WIFI_STATE_ENABLED;
	wifi_service_set_state(skeleton, WIFI_STATE_ENABLED);

	wifi_service_emit_wifi__state__changed(skeleton,
			WIFI_STATE_ENABLED, WIFI_STATE_DISCONNECTED);

	return TRUE;
}

gboolean WifiServiceImpl::isWifiEnabled(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	cout << TAG << "::" << __func__ << " ok test "<< endl;
#if 1
	wifi_service_complete_is_wifi_enabled(skeleton, invocation,mState == WIFI_STATE_DISABLED ? FALSE : TRUE);
#else
    char reply[32];
    size_t reply_len;

	mNative->wifi_command("PING",reply,&reply_len);
#endif
	
	return TRUE;
}

gboolean WifiServiceImpl::setPeriodicScanInterval(WifiService *skeleton,
			GDBusMethodInvocation *invocation, GVariant *container)
{
	gint interval;

	g_variant_get(container, "(i)", &interval);

	cout << TAG << "::" << __func__ << ": interval=" << interval << endl;

	if (mState == WIFI_STATE_DISABLED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.IllegalState",
					"The periodic scan interval cannot be set when disabled");
		return TRUE;
	}

	if (interval < 0 || (interval > 0 && interval < 6)) {
		g_dbus_method_invocation_return_dbus_error(invocation,
				"com.hsaeyz.sal.WifiService.Error.InvalidArgs",
					"The argument \"interval\" is out of range");
		return TRUE;
	}

	wifi_service_set_periodic_scan_interval(skeleton, interval);
	if (!interval) {
		if (mPeriodicScan) {
			mPeriodicScan->disconnect();
			delete mPeriodicScan;
			mPeriodicScan = nullptr;
		}
	} else if (mState == WIFI_STATE_ENABLED) {
		if (mPeriodicScan) {
			mPeriodicScan->disconnect();
			delete mPeriodicScan;
		}
		mPeriodicScan = new sigc::connection(
			Glib::signal_timeout().connect_seconds(
				sigc::mem_fun(*this, &WifiServiceImpl::onPeriodicScan), interval));
	}

	wifi_service_complete_set_periodic_scan_interval(skeleton, invocation);

	return TRUE;
}

gboolean WifiServiceImpl::getConnectionInfo(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	cout << TAG << "::" << __func__ << endl;
	
	if (mState != WIFI_STATE_CONNECTED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
		"com.hsaeyz.sal.WifiService.Error.IllegalState", "Not connected");
		return TRUE;
	}

	auto info = mMonitor->getConnectionInfo();
	if (!info) {
		g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.IllegalState", "Not connected");
		// Will update mState laterly
		return TRUE;
	}

	wifi_service_complete_get_connection_info(skeleton, invocation, info);

	return TRUE;
}
#if 0
gboolean WifiServiceImpl::getConnectionTimeStatices(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	cout << TAG << "::" << __func__ << endl;
	
	if (mState != WIFI_STATE_CONNECTED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.IllegalState", "Not connected");
		return TRUE;
	}

	auto info = mMonitor->getConnectionInfo();
	if (!info) {
		g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.IllegalState", "Not connected");
		// Will update mState laterly
		return TRUE;
	}

	wifi_service_complete_get_connection_info(skeleton, invocation, info);

	return TRUE;
}
#endif
gboolean WifiServiceImpl::getSavedNetworks(WifiService *skeleton, GDBusMethodInvocation *invocation)
{
	cout << TAG << "::" << __func__ << endl;

	if (mState == WIFI_STATE_DISABLED) {
		g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiService.Error.IllegalState",
			"The method cannot be called when Wi-Fi is disabled");
		return TRUE;
	}

	auto networks = mMonitor->getSavedNetworks();

	wifi_service_complete_get_saved_networks(skeleton, invocation, networks);

	return TRUE;
}

bool WifiServiceImpl::onStateChanged(const Glib::IOCondition& condition)
{
	uint64_t count;

	// Clear this event.
	read(mStateChanged, &count, sizeof count);

	auto event = mStateQueue.front();
	mStateQueue.pop();

	cout << TAG << "::" << __func__ <<
		": current=" << event->first << ", previous=" << event->second << endl;

	if (mState == WIFI_STATE_DISABLED) {
		cout << TAG << "::" << __func__ <<
			": Drop transition (" << event->second <<
				" -> " << event->first << ")" << endl;
		goto done;
	}

	// WIFI_STATE_CHANGED(currentState, previousState)
	wifi_service_emit_wifi__state__changed(mSkeleton, event->first, event->second);

	switch (event->first) {
	case WIFI_STATE_SCAN_DONE:
		// Deprecated. Will be removed in future release.
		wifi_service_emit_wifi__scan__results__available(mSkeleton);
		wifi_service_emit_wifi__state__changed(mSkeleton, mState, WIFI_STATE_SCAN_DONE);
		break;
	case WIFI_STATE_FAILED:
		wifi_service_emit_wifi__state__changed(mSkeleton, mState, WIFI_STATE_FAILED);
		break;
	case WIFI_STATE_DISCONNECTED:
		{
			auto interval = wifi_service_get_periodic_scan_interval(mSkeleton);
			if (interval)
				mPeriodicScan = new sigc::connection(
					Glib::signal_timeout().connect_seconds(
						sigc::mem_fun(*this, &WifiServiceImpl::onPeriodicScan), interval));
		}

//		mSignalPoll->disconnect();
//		delete mSignalPoll;
		mSignalLevel = -9999;

		wifi_service_emit_wifi__state__changed(mSkeleton,
				WIFI_STATE_ENABLED, WIFI_STATE_DISCONNECTED);
		mState = WIFI_STATE_ENABLED;
		wifi_service_set_state(mSkeleton, mState);
		break;
	case WIFI_STATE_CONNECTED:
		if (mPeriodicScan) {
			mPeriodicScan->disconnect();
			delete mPeriodicScan;
			mPeriodicScan = nullptr;
		}
		// perform SSD every 5 seconds
//		mSignalPoll = new sigc::connection(
//			Glib::signal_timeout().connect_seconds(
//				sigc::mem_fun(*this, &WifiServiceImpl::onSignalPoll), 5));

		mState = event->first;
		wifi_service_set_state(mSkeleton, mState);
		break;
	default:
		// Intermediate states will be ignored currently
		;
	}

done:
	delete event;

	return true;
}

// Will be called in monitor thread now
void WifiServiceImpl::stateChanged(enum WifiState current, enum WifiState previous)
{
	uint64_t count = 1;

	cout << TAG << "::" << __func__ <<
		": current=" << current << ", previous=" << previous << endl;

	mStateQueue.push(new WifiStateEvent(current, previous));

	// wakeup WifiServiceImpl::onStateChanged
	write(mStateChanged, &count, sizeof count);
}

bool WifiServiceImpl::onPeriodicScan()
{

	/* this state, not allow scan */
	if((WIFI_STATE_ENABLING== mState ) ||
		(WIFI_STATE_ENABLING== mState ) ||
		(WIFI_STATE_CONNECTING== mState ) ||
		(WIFI_STATE_DISABLING == mState ) ||
		(WIFI_STATE_DISABLED == mState ) ||
		(WIFI_STATE_SCANNING == mState ) ||
		(WIFI_STATE_FAILED == mState ))
	{	
		cout << TAG << "::" << __func__ << " not allow scan : " << mState << endl;
		return TRUE;
	}

	cout << TAG << "::" << __func__ << ": interval=" <<
		wifi_service_get_periodic_scan_interval(mSkeleton)<< " BBBBB "<< this->GetTickCount() << endl;
	
	auto msg = startScan();

	g_variant_unref(msg->second);
	delete msg->first;
	delete msg;

	return true;
}

void WifiServiceImpl::wifi_sta_change(gint state,const gchar *addr){

	GVariant *args;	
	WifiClient *proxy;
	GError *error = NULL;

	args = g_variant_new("(is)",state,addr);

	WifiClientImpl* mclient = WifiClientImpl::getInstance();
	proxy = mclient->getwifiClient();

	cout << TAG << "::" << __func__ << " Send start !!"<< endl;

	wifi_client_emit_hostap_stat_change(proxy,args);

	if (error) {
		cout << TAG << "::" << __func__ << "send sta change message error:"<< error->message<<endl;
	} else {
		cout << TAG << "::" << __func__ << " Send succ!!"<< endl;
	}
  return;
}

gboolean WifiServiceImpl::addNewSta(WifiService *skeleton,GDBusMethodInvocation *invocation, GVariant *container)
{
	gchar *addr;
	GVariant *args;

	g_variant_get(container, "(s)", &addr);

	wifi_service_complete_addsta(skeleton,invocation);

	if(string(addr) == "AP-ENABLED ")
	{
		mState = HOSTAP_STATE_ENABLED;

	}else
	{
		mState = HOSTAP_STATE_CLIENT_CONNECT;

	}
	
	cout << TAG << "::" << __func__ << " mState: " << mState << " new station :" << addr << endl;

	wifi_sta_change(mState,addr);

	return true;
}
gboolean WifiServiceImpl::updatehostapMac(WifiService *skeleton,GDBusMethodInvocation *invocation, GVariant *container)
{
	gchar *addr;
	GVariant *args;

	g_variant_get(container, "(s)", &addr);

	wifi_service_complete_update_host_ap_mac(skeleton,invocation);

	cout << TAG << "::" << __func__ << " hostapd status :  " << addr << endl;

	mState = HOSTAP_STATE_ENABLED;
        
	wifi_sta_change(HOSTAP_STATE_ENABLED,addr);

	return true;
}
gboolean WifiServiceImpl::deleteOldSta(WifiService *skeleton,GDBusMethodInvocation *invocation, GVariant *container)
{
	gchar *addr;
	GVariant *args;

	g_variant_get(container, "(s)", &addr);
	
	wifi_service_complete_deletesta(skeleton,invocation);

	mState = HOSTAP_STATE_CLIENT_DISCONNECT;

	cout << TAG << "::" << __func__ << " delete station " << addr << endl;

	wifi_sta_change(HOSTAP_STATE_CLIENT_DISCONNECT,addr);

	return true;
}

gboolean WifiServiceImpl::stopHostAp(WifiService *skeleton,GDBusMethodInvocation *invocation, GVariant *container)
{

	cout << TAG << "::" << __func__ << " stop  hostap " << mState << endl;

	/* kill hostap */

	mNative = WifiNative::getInstance();

	if(mNative->wifi_stop_hostap())
	{
		cout << TAG << "::" << __func__ << ": Cannot start the hostap " << "ERROR ! ERROR ! ERROR " << endl;
		return TRUE;
	}
	wifi_service_emit_wifi__state__changed(skeleton,WIFI_STATE_DISABLED,WIFI_STATE_ENABLED);
	
	wifi_service_complete_stop_host_ap(skeleton,invocation);

	return true;	
}
gboolean WifiServiceImpl::startHostAp(WifiService *skeleton,GDBusMethodInvocation *invocation, GVariant *container)
{
	unsigned long start,end,time1,time2,time3,time4;

	start = GetTickCount();

	mSkeleton_clientA = skeleton; 

	cout << TAG << "::" << __func__ << " create a hostap " << mState << start << endl;

	/* 启动hostap */
	// if(mState != HOSTAP_STATE_ENABLED)
	{
        //wifi_service_emit_wifi__state__changed__host(skeleton,WIFI_STATE_ENABLING, WIFI_STATE_DISABLED);

		mNative = WifiNative::getInstance();
		// time1 = GetTickCount();

		if(mNative->wifi_start_hostap())
		{
			cout << TAG << "::" << __func__ << ": Cannot start the hostap " << "ERROR ! ERROR ! ERROR " << endl;
			return TRUE;
		}
		// time2 = GetTickCount();

		wifi_service_complete_start_host_ap(skeleton,invocation);

		cout << TAG << "::" << __func__ << ": start the hostap "<<  " ok BBBBBBBBB " << endl;

        //wifi_service_emit_wifi__state__changed__host(skeleton,HOSTAP_STATE_ENABLED, WIFI_STATE_ENABLING);
		mState = HOSTAP_STATE_ENABLED;
        
		// wifi_sta_change(HOSTAP_STATE_ENABLED,hostapMac.c_str());

		// time3 = GetTickCount();

	}
	// else{

	// 	cout << TAG << "::" << __func__ << " hostap always started " << mState << endl;
	// }

	/* ip地址 */
	// if(mState == HOSTAP_STATE_ENABLED)
	// {
	// 	/* wlan0 设置固定ip：192.168.1.1 */
		
	// 	if(mNative->wifi_set_ip_address()){
	// 		cout << TAG << "::" << __func__ << ": Cannot set statis address " << "ERROR ! ERROR ! ERROR " << endl;
	// 		return TRUE;			
	// 	}
	// 	time4 = GetTickCount();

	// 	mState = HOSTAP_STATE_IP;

    //     const gchar *ip= "192.168.1.1 test";

    //     wifi_sta_change(HOSTAP_STATE_IP,(gchar *)ip);

		
	// 	cout << TAG << "::" << __func__ << ": set static ip " << " ok BBBBBBBBB " << endl;

	// 	/* 创建dhcp server */		
	// 	/* 
		if(mNative->wifi_start_dhcpserver()){
			
			cout << TAG << "::" << __func__ << ": Cannot start the dhcpserver " << "ERROR ! ERROR ! ERROR " << endl;
			return TRUE;
		}
	// 	cout << TAG << "::" << __func__ << ": start dhcpserver" << " ok BBBBBBBBB " << endl;
	// 	*/

	// }
	// end = GetTickCount();

	// wifi_service_emit_wifi__state__changed__host(skeleton,HOSTAP_STATE_IP, HOSTAP_STATE_ENABLED);
	// cout << TAG << "::" << __func__ << ": BBBBBBBBB " <<"start:" << start << "time1:"<< time1<< "time2:" << time2 << "time3:" << time3  << "time4:"<< time4 << "end:"<< end << endl;
	return true;	
}

bool WifiServiceImpl::onSignalPoll()
{
	gint rssi, level;

	auto signal = mMonitor->signalPoll();
	g_variant_get(signal, "(ii)", &rssi, &level);

	cout << TAG << "::" << __func__ << ": RSSI=" <<
				rssi << ", Level=" << level << endl;

	g_variant_unref(signal);

	if ((mSignalLevel != -9999) && (level != mSignalLevel))
		wifi_service_emit_wifi__signal__level__changed(mSkeleton, level, mSignalLevel);

	mSignalLevel = level;

	return true;
}

void WifiServiceImpl::start()
{
	const auto id =
		Gio::DBus::own_name(
			Gio::DBus::BUS_TYPE_SYSTEM, "com.hsaeyz.sal.WifiService",
			sigc::mem_fun(*this, &WifiServiceImpl::onBusAcquired),
			sigc::mem_fun(*this, &WifiServiceImpl::onNameAcquired),
			sigc::mem_fun(*this, &WifiServiceImpl::onNameLost));

	mLoop = Glib::MainLoop::create();
	mLoop->run();

	Gio::DBus::unown_name(id);
}

} // end of connectivity

extern int client_server_main(void);
int main(int argc, char *argv[])
{
	system("rm -f /var/run/dhcp_done.pipe 2>&1 >/dev/null");
	system("mkfifo /var/run/dhcp_done.pipe 2>/dev/null");

	Gio::init();

	auto *service =
			connectivity::WifiServiceImpl::getInstance();
	
	int ret = client_server_main();

	service->start();

	service->destroy();

	return 0;
}
