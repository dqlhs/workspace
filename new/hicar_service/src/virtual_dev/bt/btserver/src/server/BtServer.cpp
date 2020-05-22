//============================================================================
// Name        : BtServer.cpp
// Author      : qianzhong
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "DeviceStubImp.h"
#include "spp.h"
#include "ble.h"
#include <Device/device.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <signal.h>
//#define IMX6DBUG
#define PROJECT202
using namespace std;
int SHOW_LOG =0 ;
bool MODEL_DEVICE =false;
bool MODEL_HFP =false;
bool MODEL_MEDIA =false;
bool MODEL_PB =false;
bool MODEL_SET =false;
bool MODEL_SQL =false;
bool isfirst=true;
int isbtpoweroffvalue =0;
std::string projectname="HSAE_";
std::string btversion;
int maxpbdownloadcount= 1000;
int callogsavemax= 100;
int sqlmax= 0;
int supportdownloadcalllog= 0;
unsigned long testcount =0;
bool powersate =false;
//int LOG_MODEL;
#if 1
unsigned long GetTickCountTest()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    unsigned long temp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return temp;
}
#endif
HSAE_CBK_RET power_status( void *hsaeCbkParam, const HSAE_POWER hsaePower )
{
    std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
    std::shared_ptr< DeviceStubImp > m_deviceInstance = DeviceStubImp::getInstance();
    if( hsaePower == HSAE_POWEROFF )
    {
        isbtpoweroffvalue++;
        if (isbtpoweroffvalue<4)
        {
            hsaeCmdSetPower( NULL, HSAE_POWERON );
        }
        g_set->BTPowerStateChanged(false);
        device_set_power_state (DeviceStubImp::interface, false);
        set_set_power_state (SettingStubImp::m_interface, false);
        DeviceStubImp::avrcp_connect_addr ={0,0,0,0,0,0};
        DeviceStubImp::hfp_connect_addr = {0,0,0,0,0,0};
        DeviceStubImp::hfp_connecting_addr = {0,0,0,0,0,0};
        DeviceStubImp::a2dp_connect_addr = {0,0,0,0,0,0};
        m_deviceInstance->currentStates.hfpState =  ConnectionState::DISCONNECTION;
        powersate = false;
    }
    else if( hsaePower == HSAE_POWERON )
    {
        testcount = GetTickCountTest();
        printf( "-----------------------poweron-----------= %d---\n",testcount );
        isbtpoweroffvalue =4;
        //2017-3-4 add
        hsaeCmdSetPlayRing( NULL, false );
        hsaeCmdSetAutoConnectionEnable(NULL, 0);
        hsaeCmdSetAutoConnectionEnableLinklost(NULL,0);
        hsaeCmdSetScanEnable( NULL, true );
        hsaeCmdSetPairDeviceNum(NULL,5);
        hsaeCmdDownloadPhonebookAlwaysConnected(NULL,true);
//         hsaeCmdSetRejectInBandRing(NULL,true);
        if (isfirst)
        {
            HSAE_CMD_RET ret2 = hsaeCmdA2dpMute(NULL, &DeviceStubImp::hfp_connect_addr, false);
            isfirst=false;
            BT_ADDR add;
            int i;
            for(i=0;i<6;i++)
            {
                add.b[i]=0;
            }
            char *m_data = (char*)malloc( 255 );
            char buf[50] ={0};
            hsaeCmdGetLocalBtAddr( NULL, &add );
            sprintf( m_data, "_%02X%02X%02X",  add.b[3], add.b[4], add.b[5] );
            sprintf( buf, "%02X:%02X:%02X:%02X:%02X:%02X", add.b[0], add.b[1], add.b[2], add.b[3], add.b[4], add.b[5] );
            device_set_local_address(DeviceStubImp::interface,buf);
            projectname = projectname + string(m_data);
            hsaeCmdSetLocalDeviceName(NULL, projectname.data());
            device_set_local_name(DeviceStubImp::interface,projectname.data());
            set_set_local_address(SettingStubImp::m_interface,buf);
            set_set_local_name(SettingStubImp::m_interface,projectname.data());

          //  hsaeCmdGetPairedList( NULL );

            m_deviceInstance ->localname = projectname;
            m_deviceInstance ->localaddess = buf;
            g_set->BroadcastbtVersion(btversion);
            free( m_data );
            m_data = NULL;

//            g_set->startAutoConnectThread();
        }
        g_set->BTPowerStateChanged(true);
        powersate = true;
        device_set_power_state (DeviceStubImp::interface, true);
        set_set_power_state (SettingStubImp::m_interface, true);
//        unsigned long test =  GetTickCountTest() - testcount;
//        printf("now  poweron =%d\r\n",test);

    }
    else
    {
        DeviceStubImp::avrcp_connect_addr ={0,0,0,0,0,0};
        DeviceStubImp::hfp_connect_addr = {0,0,0,0,0,0};
        DeviceStubImp::hfp_connecting_addr = {0,0,0,0,0,0};
        DeviceStubImp::a2dp_connect_addr = {0,0,0,0,0,0};
        m_deviceInstance->currentStates.hfpState =  ConnectionState::DISCONNECTION;

        g_set->BTPowerStateChanged(false);
        device_set_power_state (DeviceStubImp::interface, false);
        set_set_power_state (SettingStubImp::m_interface, false);
        powersate = false;

    }
    LOG(BT_LOG_DEVICE,"Local Bluetooth device is%d ,try open %d",(int)hsaePower,isbtpoweroffvalue);
    return HSAE_CBK_SUCCESS;
}


pid_t GetProcessidFromName(char* name)
{
    printf("GetProcessidFromName =%s \r\n" ,name);
     pid_t pid = -1;
 #if 1
    FILE *fp;
    char buf[100];
    char cmd[200] = {'\0'};
//    pid_t pid = -1;
    sprintf(cmd, "pidof %s", name);
    if((fp = popen(cmd, "r")) != NULL)
    {
        if(fgets(buf, 255, fp) != NULL)
        {
            pid = atoi(buf);
        }
    }
    printf("pid = %d \n", pid);
    pclose(fp);
#endif
    return pid;
}
int main( int argc, char *argv[] )
{

   testcount = GetTickCountTest();
//   printf( "-----------------------BTSERVERSTART V1.0--------------\n" );
#ifdef USE_DOWNLOADCALLLOG
   printf( "-----------------------BTSERVERSTART V1.0- USE_DOWNLOADCALLLOG-----------= %d---\n",testcount );
#else
   printf( "-----------------------BTSERVERSTART V1.0- -------------------------------= %d\r\n" ,testcount);
#endif
    char * buf =NULL;
   // cout<<"cire"<<string(buf)<<endl;
    //read env
    char* logenv = getenv( "BT_SLOG_ISSHOW" );
    if( NULL != logenv && !strcmp( logenv, "1" ) )
    {
        SHOW_LOG = 1;
    }
    else
    {
        SHOW_LOG = 0;
    }
    char*  loglevl= getenv( "BT_LOG_MODEL" );
   if (loglevl!=NULL)
   {

       string buf = loglevl;
       if (buf.size()>5)
       {
           MODEL_DEVICE = (buf.at(0)==48)?false:true;
           MODEL_HFP = (buf.at(1)==48)?false:true;
           MODEL_MEDIA = (buf.at(2)==48)?false:true;
           MODEL_PB = (buf.at(3)==48)?false:true;
           MODEL_SET = (buf.at(4)==48)?false:true;
           MODEL_SQL = (buf.at(5)==48)?false:true;
       }
   }
   else
   {
       MODEL_DEVICE =false;
       MODEL_HFP = false;
       MODEL_MEDIA = false;
       MODEL_PB = false;
       MODEL_SET = false;
       MODEL_SQL = false;
   }
    char*  project= getenv( "PROJECTNAME" );
    if (project!=NULL)
    {
        char name[50]={0};

        sprintf(name, "%s",project);// projectname = project;
        projectname = name;
    }
    char*  sqlitecount= getenv( "SQLITE" );
    if (sqlitecount!=NULL)
    {
        char buf[50]={0};
        sprintf(buf, "%s",sqlitecount);// projectname = project;
        sqlmax =atoi(buf) ;
        cout <<"-----------------sqlmax-------------------"<<sqlmax<<endl;
    }
    char*  pbcount= getenv( "MAXDOWNLOADPBCOUNT" );
    if (pbcount!=NULL)
    {
        char pbcountname[50]={0};
        sprintf(pbcountname, "%s",pbcount);// projectname = project;
        maxpbdownloadcount = atoi(pbcountname);
        cout <<"-----------------maxpbdownloadcount-------------------"<<maxpbdownloadcount<<endl;
    }
    char*  callogsavemaxcount= getenv( "MAXSAVECALLLOG" );
    if (callogsavemaxcount!=NULL)
    {
        char callogsavemaxcountname[50]={0};
        sprintf(callogsavemaxcountname, "%s",callogsavemaxcount);// projectname = project;
        callogsavemax = atoi(callogsavemaxcountname);
        cout <<"-----------------callogsavemax-------------------"<<callogsavemax<<endl;
    }

#ifdef IMX6DBUG
   MODEL_DEVICE =true;
   MODEL_HFP = true;
   MODEL_MEDIA = true;
   MODEL_PB = true;
   MODEL_SET = true;
   MODEL_SQL = true;
#endif
//    LOG(BT_LOG_DEVICE,"MODEL_SQL=%d",MODEL_DEVICE);
    HSAE_CMD_RET *hsaeCbkParam;
    HSAE_HANDLE hsaeHandle;
    hsaeCbkParam = (HSAE_CMD_RET *)malloc( sizeof(HSAE_CMD_RET) );
    HSAE_CMD_RET cmd_ret;

    HSAE_CALLBACK hsaeCallback;
    memset( &hsaeCallback, 0, sizeof(hsaeCallback) );
    hsaeCmdSetCallback( hsaeHandle, &hsaeCallback );
    hsaeCmdSetCbk_PowerStatus( hsaeHandle, power_status );

    cmd_ret = hsaeCmdStartup( &hsaeHandle, hsaeCbkParam );
    //QIANZHONG ADD 2016-8-29
    if( *(int*)hsaeHandle == HSAE_HANDLE_TYPE_SYS )
    {
        printf( "[BtServer]Bluelink in System mode.\n" );
    }
    else if( *(int*)hsaeHandle == HSAE_HANDLE_TYPE_APP )
    {
        printf( "[BtServer]Bluelink in Application mode.\n" );
    }
    while (1) {
        char name[20] ="bt_firmware_set";
        pid_t process_pid = GetProcessidFromName(name);
        if (process_pid > 0)
        {
           usleep(100*1000);
        }
        else
          break;

    }
    HSAE_CMD_RET ret = hsaeCmdSetPowerEx( hsaeHandle, HSAE_POWERON,0 );
    HSAE_VER btver;
    hsaeCmdGetMwVersion(&btver);
    char c[15];
    memset(c,0,15);
    sprintf( c,"V%d.%d.%d", btver.major,btver.minor,btver.build);
    btversion = c;
    //add by qianzhong 2017-2-24
    // usleep( 1000 * 1000 );

    isbtpoweroffvalue = 0;
    if( cmd_ret != HSAE_CMD_SUCCESS )
    {
        printf( "[BtServer]Initializing failed, please check if your bluetooth device has been enabled.\n" );
        return 0;
    }

    //init dbus
    std::shared_ptr< DeviceStubImp > m_deviceInstance = DeviceStubImp::getInstance();
    std::shared_ptr< SettingStubImp > m_settingInstance = SettingStubImp::getInstance();
    std::shared_ptr< HFPStubImp > m_phoneInstance = HFPStubImp::getInstance();
    std::shared_ptr< PhoneBookStubImp > m_phoneBookInstance = PhoneBookStubImp::getInstance();
    std::shared_ptr< MediaPlayBackStubImp > m_mediaPlayInstance = MediaPlayBackStubImp::getInstance();
    std::shared_ptr< SPPStubImp > m_sppInstance = SPPStubImp::getInstance();
    std::shared_ptr< BleImp > m_bleInstance = BleImp::getInstance();
    guint owner_id_device, owner_id_media, owner_id_phone, owner_id_phonebook, owner_id_set,owner_id_spp,owner_id_ble;


    GMainLoop *loop;

    //初始化 GObject类型系统
   // g_type_init();

    //连接server到gdbus
    owner_id_device = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_deviceInstance->on_bus_acquired,             //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_media = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_mediaPlayInstance->on_bus_acquired,          //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_phone = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_phoneInstance->on_bus_acquired,              //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_phonebook = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_phoneBookInstance->on_bus_acquired,          //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_set = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_settingInstance->on_bus_acquired,            //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_spp = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_sppInstance->on_bus_acquired,            //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    owner_id_ble = g_bus_own_name( G_BUS_TYPE_SYSTEM,           //bus_type
                    "com.hsaeyz.bluetooth",                           //bus_name
                    G_BUS_NAME_OWNER_FLAGS_NONE,  //flag
                    m_bleInstance->on_bus_acquired,            //注册输出对象的回调函数
                    NULL,
                    NULL,
                    NULL,
                    NULL );
    if( owner_id_device != 0 )
    {
        printf( "[BtServer]Registered on DBus as %i\n", owner_id_device );
    }

    loop = g_main_loop_new( NULL, FALSE );
    g_main_loop_run( loop );

    g_bus_unown_name( owner_id_device );
    g_bus_unown_name( owner_id_media );
    g_bus_unown_name( owner_id_phone );
    g_bus_unown_name( owner_id_phonebook );
    g_bus_unown_name( owner_id_set );
    g_bus_unown_name( owner_id_spp );
    g_bus_unown_name( owner_id_ble );
    return 0;
}
