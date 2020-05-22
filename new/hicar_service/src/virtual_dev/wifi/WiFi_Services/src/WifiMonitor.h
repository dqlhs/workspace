#ifndef __WIFIMONITOR_HPP__
#define __WIFIMONITOR_HPP__

#include <string>
#include <queue>
#include <list>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <WifiServiceImpl.h>
#include <WifiNative.h>

namespace connectivity {

using namespace std;

class WifiServiceImpl;

class WifiMonitor : public sigc::trackable {
public:
	using WifiMessage = pair<string *, GVariant *>;

private:
	static const string TAG;

	WifiServiceImpl *mService;
	WifiNative *mNative;

	mutex mResultsLock;
	GVariant *mScanResults;

	// not automatic scan on the first shot
	bool mWorkInProgress = false;
	mutex mWIPLock;
	condition_variable mWorkDone;

	bool mConnected = false;

	// Command queue
	int mCmdEventFD;
	queue<WifiMessage *, list<WifiMessage *>> mCmdQueue;

	int mRetEventFD;
	queue<WifiMessage *, list<WifiMessage *>> mRetQueue;

	thread *mThread;
	Glib::RefPtr<Glib::MainLoop> mLoop;

	WifiMonitor();
	~WifiMonitor();

	void run();

	bool onCmdQueue(const Glib::IOCondition&);
	void onStartScan(GVariant *);
	void onConnect(GVariant *);
	void onDisconnect(GVariant *);
	void onGetConnectionInfo(GVariant *);
	void onGetSavedNetworks(GVariant *);
	void onSignalPoll(GVariant *);

	void onScanCompleted(bool);

	bool onSupplicantEvent(const Glib::IOCondition&);

	WifiMessage *connect(GVariant *);

public:
	static WifiMonitor *getInstance();

	void destroy();

	WifiMessage *startScan();

	GVariant *getScanResults();

	WifiMessage *connect(const string&, const string&);
	WifiMessage *connect(gint);

	void disconnect();

	GVariant *getConnectionInfo();

	void ensureAutoScan();

	GVariant *getSavedNetworks();

	GVariant *signalPoll();
	unsigned long GetTickCount();
	
};

}

#endif
