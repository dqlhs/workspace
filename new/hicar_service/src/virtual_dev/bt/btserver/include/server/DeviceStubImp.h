#ifndef _DEVICESTUBIMP_H_
#define _DEVICESTUBIMP_H_
#include <signal.h>
#include <dirent.h>

#include "HsaeTypes.h"
#include"ctimer.h"
#include <iostream>
#include <memory>
#include <map>
#include <thread>
#include "SettingStubImp.h"
#include "MediaPlayBackStubImp.h"
#include "HFPStubImp.h"
#include "PhoneBookStubImp.h"
#include "spp.h"
#include <pthread.h>
//add by qianzhong
#include <Device/device.h>
class AutoConnectThread;
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;
#define READ_BUFFER_SIZE            256
#define SYNERGY_BT_SERVICE_PROCESS_NAME             "synergy_bt_service"
#define BT_MANAGER_PROCESS_NAME                     "bt_manager"
struct RFCOMMSTATES
{
    int chanelid =0;
    int rfcommid =0;
    HSAE_CONNECTION_STATE status =HSAE_CON_DISCONNECTION;
};
class DeviceStubImp
{
public:
    DeviceStubImp();
    ~DeviceStubImp();
    static Device* interface;
    static std::shared_ptr< DeviceStubImp > deviceInstance;
    static std::shared_ptr< DeviceStubImp > getInstance();

    //add by qianzhong 2017-5-12
    static void on_bus_acquired( GDBusConnection *connection, const gchar *name,
                                 gpointer user_data );
    //end add;

    //client invoke method
//    static gboolean pairRequest(Device *object,
//                     GDBusMethodInvocation *invocation);

    static gboolean accpectPair( Device *object,
                                 GDBusMethodInvocation *invocation );

    static gboolean rejectPair( Device *object,
                                GDBusMethodInvocation *invocation );

    static gboolean deletePaired( Device *object,
                                  GDBusMethodInvocation *invocation,
                                  const gchar *arg_btAddress );

    static gboolean connect( Device *object, GDBusMethodInvocation *invocation,
                             const gchar *arg_btAddress, gint arg_profileId );

    static gboolean deviceInquiryStart( Device *object,
                                        GDBusMethodInvocation *invocation );

    static gboolean deviceInquiryStop( Device *object,
                                       GDBusMethodInvocation *invocation );

    static gboolean disConnect( Device *object,
                                GDBusMethodInvocation *invocation,
                                const gchar *arg_btAddress,
                                gint arg_profileId );

    static gboolean getPairedList( Device *object,
                                   GDBusMethodInvocation *invocation );

    static gboolean getLocalProperties( Device *object,
                                        GDBusMethodInvocation *invocation );


    //add 2017-2-28
    static gboolean getCurrentStates( Device *object,
                                      GDBusMethodInvocation *invocation );

    static gboolean startScan( Device *object,
                               GDBusMethodInvocation *invocation );

    static gboolean stopScan( Device *object,
                               GDBusMethodInvocation *invocation );

    //add by qianzhong 2017/7/4
    static gboolean disConnectCurrentProfiles(Device *object,GDBusMethodInvocation *invocation);


    void getID3Info();

//josn
    void BTPairListChanged( Pairs pairList );

    void BTConnectionStateChanged( string address, PROFILEID profileId,
                                   ConnectionState state );

    void BTLocalPropertiesChanged( LocalProperty property );

    //add by  qianzhong 2017-3-5
    void BTCurrentStateChanged( CurrentStates currentStates );

    void BTSearchChanged(string name,string addr);

    void BTBatteryChanged(int battery);
    void chartostr(char *strmac,unsigned char*mac);


//inner
private:
    static bool a2dpstate, avrcpstate, hfpstate;

public:
    void initProxy();
    void DeviceUnpaired( string devicePath );
    void DevicePaired( string devicePath );
    //add by qianzhong 2017-5-31
    void phoneConnnectNF();
    void clearpairlist();

    bool IsArrayEmpty(unsigned char *buf,int len);
    bool IsCompareArray(unsigned char *buf1,int len1,unsigned char *buf2,int len2);

    //2016-12-28 add by qianzhong
  //  static std::thread::id threadId;

    /**globle hfp_sem hfp connect success  a2dp_sem connect success avrcp_sem connect success avrcp_sem connect success avrcp_sem  avrcp_sem_fail avrcp connect fail**/
//    static sem_t hfp_sem, a2dp_sem, avrcp_sem, avrcp_sem_fail, hfp_sem_fail;
    /**
     * add by qianzhong 2017-1-10
     */
    CurrentStates currentStates;
    static PlayAttribute PLAYATTRIBUTE;
    static unsigned long GetTickCount();
    static unsigned long m_dwDialMoment;
    //add by qianzhong 2017-5-23

    PlayStatus status;
    PlayPosition position;
    //ADD BY QIANZHONG
    static BT_ADDR hfp_connect_addr;
    static BT_ADDR hfp_connecting_addr;
    static BT_ADDR avrcp_connect_addr;
    static BT_ADDR a2dp_connect_addr;
    BT_ADDR sppaddr;
    LocalProperty m_property;
    string localname;
    string localaddess;
    CTimer m_connecttimer;
    CTimer m_disconnecttimer;
    bool m_currentsppstate;
    static void timeout();
    static void disconnecttimeout();
    int m_cout;
    RFCOMMSTATES m_client[2];
    RFCOMMSTATES m_server[2];

};

#endif

