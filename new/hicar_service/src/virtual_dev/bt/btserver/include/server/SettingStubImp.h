#ifndef _SETTINGSTUBIMP_H_
#define _SETTINGSTUBIMP_H_
//#include "com/hsae/bluetooth/Setting/SettingStubDefault.h"
#include "src-gen/Set/set.h"
#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>

#include "HsaeTypes.h"
#include "DeviceStubImp.h"
using namespace com::hsae::bluetooth::util::HsaeTypes;
using namespace std;
class SettingStubImp {//: public SettingStubDefault {

public:
    SettingStubImp();
    static std::shared_ptr<SettingStubImp> btSettingInstance;
    static std::shared_ptr<SettingStubImp> getInstance();
    ~SettingStubImp();
    static Set* m_interface;
    void closeAutoConnectThread();
    void startAutoConnectThread();
    void setautoaddress();
    //method
    static gboolean setPower(Set *interface, GDBusMethodInvocation *invocation, gboolean power);

    static gboolean setScanEnable(Set *interface, GDBusMethodInvocation *invocation, gboolean scan);

    static gboolean setLocalDeviceName(Set *interface, GDBusMethodInvocation *invocation, const gchar *name);
    //add by qianzhong 2017-6-21
    static gboolean setScanTime(Set *object,GDBusMethodInvocation *invocation,gint arg_time);
    //add by lihuimin 2017-09-07

    static gboolean enableautoconncet(Set *interface, GDBusMethodInvocation *invocation, gboolean state);

//    static gboolean setautoconect(Set *interface, GDBusMethodInvocation *invocation, gboolean state);

    static gboolean setAutoConnection(Set *interface, GDBusMethodInvocation *invocation, gboolean state);

    static gboolean setFactory(Set *interface, GDBusMethodInvocation *invocation);
//    static gboolean setPower(Set *interface, GDBusMethodInvocation *invocation, gboolean power);

//    void setPhoneVolume(std::string volume);

    /**
     * powerState value 0:off 1:on 2:unknown
     */
    //signal
    void BTPowerStateChanged(bool powerState);

    void BroadcastbtVersion(string version);

    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);

    void setPairable(bool pairable);
    void setBlooProperty(char* propertyName,bool value);

    void setDiscoverable(bool value);
    string getautoaddr();
    void setautoconnectmac(char * mac);

//    void SetPropertyResult(DBusPendingCall *call, void *user_data);
//    BT_ADAPTER_PROPERTIES *Properties;
private:
    static void SetPropertyResult(DBusPendingCall *call, void *user_data);
    pthread_t autoconnect_device_thread_id;
    static void* autoConnectDeviceFunc(void* args);
    bool g_bRun;
    string auto_addr;
   void  str_to_char( const char *addr, unsigned char * b );
   int IsFileExist(const char* path);
public:
    bool m_benableconnect;
};
//}
//}
//}
//}

#endif

