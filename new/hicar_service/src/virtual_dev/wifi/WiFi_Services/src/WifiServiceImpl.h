#ifndef __WIFISERVICEIMPL_HPP__
#define __WIFISERVICEIMPL_HPP__

#include <string>

#include <glibmm.h>
#include <giomm.h>
#include <os.h>

#include <WifiService.h>
#include <WifiMonitor.h>
#include <WifiNative.h>
#include <WifiClientImpl.h>

namespace connectivity {

using namespace std;

// enum WifiState {
// 	WIFI_STATE_NONE,
// 	WIFI_STATE_ENABLING,		// 0
// 	WIFI_STATE_ENABLED,	    	// 1
// 	WIFI_STATE_CONNECTING,		// 2
// 	WIFI_STATE_CONNECTED,		// 3
// 	WIFI_STATE_DISCONNECTING,	// 4
// 	WIFI_STATE_DISCONNECTED,	// 5
// 	WIFI_STATE_DISABLING,		// 6
// 	WIFI_STATE_DISABLED,		// 7
// 	WIFI_STATE_SCANNING,		// 8
// 	WIFI_STATE_SCAN_DONE,		// 9
// 	WIFI_STATE_FAILED,	    	// 10
// 	HOSTAP_STATE_DISABLE,
// 	HOSTAP_STATE_ENABLED,
// 	HOSTAP_STATE_IP,
// 	HOSTAP_STATE_CLIENT_CONNECT,
// 	HOSTAP_STATE_CLIENT_DISCONNECT,

// };


class WifiMonitor;

class WifiClientImpl;

using WifiMessage = pair<string *, GVariant *>;

class WifiServiceImpl : public sigc::trackable {
private:
	static string TAG;
	
	string hostapMac;

	WifiService *mSkeleton;

	WifiService *mSkeleton_clientA;

	WifiService *mSkeleton_clientB;

	Glib::RefPtr<Glib::MainLoop> mLoop;

	WifiMonitor *mMonitor;
	WifiNative *mNative;

	enum WifiState mState = WIFI_STATE_DISABLED;

	int ScantimesFail;

	sigc::connection *mPeriodicScan = nullptr;
	bool onPeriodicScan();

	gint mSignalLevel = -9999;
	sigc::connection *mSignalPoll = nullptr;
	bool onSignalPoll();

	using WifiStateEvent = pair<enum WifiState, enum WifiState>;

	int mStateChanged;
	bool onStateChanged(const Glib::IOCondition&);
	queue<WifiStateEvent *, list<WifiStateEvent *>> mStateQueue;

	WifiServiceImpl();
	~WifiServiceImpl();

	void onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);
	void onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);
	void onNameLost(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);

	static gboolean onMethodCall(WifiService *,
				GDBusMethodInvocation *, GVariant *, gpointer);
	gboolean setWifiEnabled(WifiService *, GDBusMethodInvocation *, GVariant *);
	gboolean connect(WifiService *, GDBusMethodInvocation *, GVariant *);
	/* hostap */
	gboolean startHostAp(WifiService *,		GDBusMethodInvocation *, GVariant *);
	gboolean stopHostAp(WifiService *,		GDBusMethodInvocation *, GVariant *);
	gboolean getHostApClinetInfo(WifiService *, GDBusMethodInvocation *);

	/* station signal */
	gboolean addNewSta(WifiService *,GDBusMethodInvocation *, GVariant *);
	gboolean updatehostapMac(WifiService *,GDBusMethodInvocation *, GVariant *);

	gboolean deleteOldSta(WifiService *,GDBusMethodInvocation *, GVariant *);
	gboolean setstaip(WifiService *,GDBusMethodInvocation *, GVariant *);

	gboolean setPeriodicScanInterval(WifiService *, GDBusMethodInvocation *, GVariant *);

	static gboolean onMethodCallNoArg(WifiService *, GDBusMethodInvocation *, gpointer);
	gboolean startScan(WifiService *, GDBusMethodInvocation *);
	gboolean getScanResults(WifiService *, GDBusMethodInvocation *);
	gboolean disconnect(WifiService *, GDBusMethodInvocation *);
	gboolean isWifiEnabled(WifiService *, GDBusMethodInvocation *);
	gboolean getConnectionInfo(WifiService *, GDBusMethodInvocation *);
	gboolean getSavedNetworks(WifiService *, GDBusMethodInvocation *);

	//static gboolean onPropertyChanged(WifiService *, GParamSpec *, gpointer);

	// To be called internally
	WifiMessage *startScan();
    void wifi_sta_change(gint state,const gchar *s);

public:
	static WifiServiceImpl *getInstance();

	void stateChanged(enum WifiState, enum WifiState);

	WifiState getCurrentState() {
		return mState;
	}

	void destroy();

	void start();
	
	unsigned long GetTickCount();
};

}

#endif
