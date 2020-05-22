#include <iostream>

//#include <unistd.h>
//#include <sys/types.h>
#include <sys/eventfd.h>
#include <string.h>
#include <memory.h>
//#include <common.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#include "os.h"
}
#else
#include "os.h"
#endif
//extern int os_get_time(struct os_time *t);


#include <WifiClientImpl.h>

namespace connectivity {

static WifiClientImpl *gInstance = nullptr;

string WifiClientImpl::TAG = "WifiClient";

WifiClientImpl::WifiClientImpl()
{
   //创建一个GThread
}

WifiClientImpl::~WifiClientImpl()
{
}


unsigned long WifiClientImpl:: GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	unsigned long temp	= ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	return temp;
}

// Not thread-safe
WifiClientImpl *WifiClientImpl::getInstance()
{
	return gInstance ? gInstance :
			(gInstance = new WifiClientImpl());
}

// Not thread-safe
void WifiClientImpl::destroy()
{
	gInstance = nullptr;
	delete this;
}

void WifiClientImpl::onBusAcquired(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
}


void WifiClientImpl::onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
	mSkeleton = wifi_client_skeleton_new();

	 g_signal_connect(mSkeleton, "handle-addsta",G_CALLBACK(onMethodCall), new string("handle-addsta"));

	 g_signal_connect(mSkeleton, "handle-hostap-stat-change",G_CALLBACK(onMethodCall), new string("handle-hostap-stat-change"));

	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(mSkeleton),
				Glib::unwrap(connection), "/com/hsaeyz/sal/WifiClient", NULL);
}

WifiClient * WifiClientImpl::getwifiClient()
{
	return mSkeleton;
}

void WifiClientImpl::onNameLost(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
					const Glib::ustring& name)
{
	cout << "onNameLost" << endl;
	mLoop->quit();
}

gboolean WifiClientImpl::onMethodCall(WifiClient *skeleton,
	GDBusMethodInvocation *invocation, GVariant *container, gpointer signal)
{
	string& signalName = *(static_cast<string *>(signal));
	WifiClientImpl *service = getInstance();

	if (signalName == "handle-addsta")
		return service->clientaddNewSta(skeleton, invocation, container);
	else if (signalName == "handle-hostap-stat-change")
		return service->hostapStatChange(skeleton, invocation, container);

	g_dbus_method_invocation_return_dbus_error(invocation,
			"com.hsaeyz.sal.WifiClient.Error.NonExist",
					"The method being called does not exist");

	return TRUE;
}

// extern "C" {void WifiStateChange(int ,char *);}

gboolean WifiClientImpl::hostapStatChange(WifiClient *proxy, GDBusMethodInvocation *invocation,GVariant *container)
{
    gint curstate;
    gchar *staMac;

	g_variant_get(container, "(is)", &curstate,&staMac);

  	// wifi_client_complete_hostap_stat_change(proxy,invocation);
	
	// WifiStateChange((int)curstate,(char *)staMac);
	
	return TRUE;
    
}
gboolean WifiClientImpl::clientaddNewSta(WifiClient *skeleton,
		GDBusMethodInvocation *invocation, GVariant *container)
{
	gchar *addr;
	GVariant *args;

	g_variant_get(container, "(s)", &addr);

	cout << TAG << "::" << __func__ << " new station" << addr<< endl;

	wifi_client_complete_addsta(skeleton,invocation);

	
	return true;
}
void WifiClientImpl::start()
{
	auto *service =
			connectivity::WifiClientImpl::getInstance();

	const auto id =
		Gio::DBus::own_name(
			Gio::DBus::BUS_TYPE_SYSTEM, "com.hsaeyz.sal.WifiClient",
			sigc::mem_fun(*this, &WifiClientImpl::onBusAcquired),
			sigc::mem_fun(*this, &WifiClientImpl::onNameAcquired),
			sigc::mem_fun(*this, &WifiClientImpl::onNameLost));


	// mLoop = Glib::MainLoop::create();
	// mLoop->run();

	// Gio::DBus::unown_name(id);
}

} // end of connectivity

#ifdef __cplusplus
extern "C" {
int  client_server_main(void )
{

	printf("\r\n client_server_main");
	auto *service =
			connectivity::WifiClientImpl::getInstance();
	
	service->start();
	printf("\r\n client_server_main2");

	// service->destroy();

	return 0;
}

}
#else
#include "os.h"
#endif


