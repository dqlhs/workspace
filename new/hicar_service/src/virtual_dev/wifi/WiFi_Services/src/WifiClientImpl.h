#ifndef __WIFICLIENTIMPL_HPP__
#define __WIFICLIENTIMPL_HPP__

#include <string.h>

#include <glibmm.h>
#include <giomm.h>
#include <os.h>

#include <WifiClient.h>

namespace connectivity {

using namespace std;

enum WifiState {
	WIFI_STATE_NONE = 1, 
	WIFI_STATE_ENABLING,
	WIFI_STATE_ENABLED,
	WIFI_STATE_CONNECTING,
	WIFI_STATE_CONNECTED,
	WIFI_STATE_DISCONNECTING,
	WIFI_STATE_DISCONNECTED,
	WIFI_STATE_DISABLING,
	WIFI_STATE_DISABLED,
	WIFI_STATE_SCANNING,
	WIFI_STATE_SCAN_DONE,
	WIFI_STATE_FAILED,
	HOSTAP_STATE_DISABLE,
	HOSTAP_STATE_ENABLED,
	HOSTAP_STATE_IP,
	HOSTAP_STATE_CLIENT_CONNECT,
	HOSTAP_STATE_CLIENT_DISCONNECT,
};
// static string *state_names[] = { 
// "WIFI_STATE_NONE",
// "WIFI_STATE_ENABLING",
// "WIFI_STATE_ENABLED",
// "WIFI_STATE_CONNECTING",
// "WIFI_STATE_CONNECTED",
// "WIFI_STATE_DISCONNECTING",
// "WIFI_STATE_DISCONNECTED",
// "WIFI_STATE_DISABLING",
// "WIFI_STATE_DISABLED",
// "WIFI_STATE_SCANNING",
// "WIFI_STATE_SCAN_DONE",
// "WIFI_STATE_FAILED",
// "HOSTAP_STATE_DISABLE",
// "HOSTAP_STATE_ENABLED",
// "HOSTAP_STATE_IP",
// "HOSTAP_STATE_CLIENT_CONNECT",
// "HOSTAP_STATE_CLIENT_DISCONNECT",
// };
using WifiMessage = pair<string *, GVariant *>;

class WifiClientImpl : public sigc::trackable {
private:
	static string TAG;

	WifiClient *mSkeleton;

	Glib::RefPtr<Glib::MainLoop> mLoop;

	int ScantimesFail;

	void run();

	WifiClientImpl();
	~WifiClientImpl();

	void onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);
	void onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);
	void onNameLost(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&);
	
	
	static gboolean onMethodCall(WifiClient *skeleton,GDBusMethodInvocation *invocation, GVariant *container, gpointer signal);

	gboolean clientaddNewSta(WifiClient *,GDBusMethodInvocation *, GVariant *);
	gboolean hostapStatChange(WifiClient *,GDBusMethodInvocation *, GVariant *);

	
public:
	static WifiClientImpl *getInstance();
	WifiClient * getwifiClient();
	void destroy();

	void start();
	
	unsigned long GetTickCount();
};

}
#ifdef __cplusplus
extern "C" {
#include "os.h"
int client_server_main(void);

}
#else

#endif
#endif
