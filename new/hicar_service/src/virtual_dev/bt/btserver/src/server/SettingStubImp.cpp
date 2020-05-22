#include "SettingStubImp.h"
#include <string.h>
#include <thread>
#include <stdlib.h>
//
//extern unsigned long testcount;
 extern unsigned long GetTickCountTest();
std::shared_ptr< SettingStubImp > SettingStubImp::btSettingInstance = NULL;
Set* SettingStubImp::m_interface = NULL;
extern bool powersate  ;
#if 0
void SettingStubImp::str_to_char( const char *addr, unsigned char *b  )
{
    int i;
    char byteInChar[2];

    for( i = 0; i < BT_ADDR_LEN; i++ )
    {
        byteInChar[0] = toupper( addr[0] );
        byteInChar[1] = toupper( addr[1] );

        // first 4 bit
        if( isalpha( byteInChar[0] ) )
            b[i] = (byteInChar[0] - 55) * 16;
        else if( isdigit( byteInChar[0] ) )
            b[i] = (byteInChar[0] - 48) * 16;

        // last 4 bit
        if( isalpha( byteInChar[1] ) )
            b[i] += (byteInChar[1] - 55);
        else if( isdigit( byteInChar[1] ) )
            b[i] += (byteInChar[1] - 48);

        if( addr[2] != '\0' )
            addr = &addr[3];
    }
}
#endif
void SettingStubImp::str_to_char( const char *addr, unsigned char *b  )
{
    int i;
    char byteInChar[2];

    for( i = 0; i < BT_ADDR_LEN; i++ )
    {
        byteInChar[0] = toupper( addr[0] );
        byteInChar[1] = toupper( addr[1] );

        // first 4 bit
        if( isalpha( byteInChar[0] ) )
            b[BT_ADDR_LEN-i-1] = (byteInChar[0] - 55) * 16;
        else if( isdigit( byteInChar[0] ) )
            b[BT_ADDR_LEN-i-1] = (byteInChar[0] - 48) * 16;

        // last 4 bit
        if( isalpha( byteInChar[1] ) )
            b[BT_ADDR_LEN-i-1] += (byteInChar[1] - 55);
        else if( isdigit( byteInChar[1] ) )
            b[BT_ADDR_LEN-i-1] += (byteInChar[1] - 48);

        if( addr[2] != '\0' )
            addr = &addr[3];
    }
}
HSAE_CBK_RET hf_spk_vol( void *HSAECbkParam, const BT_ADDR ba,
                        const unsigned char volume )
{
    LOG(BT_LOG_SET,"volume =%u", volume);


    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET hf_mic_vol( void *HSAECbkParam, const BT_ADDR ba,
                        const unsigned char volume )
{

    LOG(BT_LOG_SET,"volume =%u", volume);
    return HSAE_CBK_SUCCESS;
}


SettingStubImp::SettingStubImp()
{
    m_benableconnect = false;
    auto_addr ="";
    autoconnect_device_thread_id =0;
//    if (IsFileExist(AUTO_CONNECT_FLG)==-1)
//    {
//        //createfile and write 0

//    }


}
SettingStubImp::~SettingStubImp()
{
   g_bRun = false;
}
std::shared_ptr< SettingStubImp > SettingStubImp::getInstance()
{
    if( btSettingInstance == NULL )
    {
        btSettingInstance = std::make_shared< SettingStubImp >();
    }
    return btSettingInstance;
}
int SettingStubImp::IsFileExist(const char* path)
{
    return !access(path, F_OK);
}
void SettingStubImp::on_bus_acquired (GDBusConnection *connection,
                                        const gchar     *name,
                                        gpointer         user_data)
{
    GError **error;

    //创建一个skeleton对象,以注册输出对象
    m_interface = set_skeleton_new();

    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect(m_interface,
                     "handle-set-power",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(setPower),      //具体的方法处理函数, 注意增加输入参数
                     user_data);
    g_signal_connect(m_interface,
                     "handle-set-scan-enable",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(setScanEnable),      //具体的方法处理函数, 注意增加输入参数
                     user_data);
    g_signal_connect(m_interface,
                     "handle-set-local-device-name",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(setLocalDeviceName),      //具体的方法处理函数, 注意增加输入参数
                     user_data);
    g_signal_connect(m_interface,
                         "handle-set-scan-time",           //signal_name, 在.c文件中的_default_init查找
                         G_CALLBACK(setScanTime),      //具体的方法处理函数, 注意增加输入参数
                         user_data);
    g_signal_connect( m_interface, "handle-set-auto-connect",
                      G_CALLBACK (setAutoConnection), user_data );
//    //signal------
    g_signal_connect(m_interface,"handle-factoryreset", G_CALLBACK(setFactory),user_data);

    g_signal_connect( m_interface, "handle_enable_auto_connect",
                      G_CALLBACK (enableautoconncet), user_data );

    //输出interface到总线上
    if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (m_interface),
                                           connection,
                                           "/com/hsaeyz/bluetooth/setObject", //object_path
                                           error))
    {
        /* handle error */
    }

}
gboolean SettingStubImp::setPower(Set *interface, GDBusMethodInvocation *invocation, gboolean power)
{
    HSAE_POWER HSAEPower;

    hsaeCmdGetPower( NULL, &HSAEPower );
     LOG(BT_LOG_SET,"power =%d", (int)HSAEPower);
     if (power)
     {
         hsaeCmdSetPower( NULL, HSAE_POWERON   );
     }
     else
     {
          hsaeCmdSetPower( NULL, HSAE_POWEROFF);
     }
//     if( HSAEPower== HSAE_POWERON)
//     {


//     }
//     else
//     {

//     }
     //
    set_complete_set_power(interface, invocation);
    return TRUE;
}
gboolean SettingStubImp::setScanEnable(Set *interface, GDBusMethodInvocation *invocation, gboolean scan)
{
    hsaeCmdSetScanEnable(NULL, scan);
    set_complete_set_scan_enable(interface, invocation);
    return TRUE;
}

gboolean SettingStubImp::setLocalDeviceName(Set *interface, GDBusMethodInvocation *invocation, const gchar *name)
{
    hsaeCmdSetLocalDeviceName(NULL, name);
    LOG(BT_LOG_SET,"name =%s", name);
    set_complete_set_local_device_name(interface, invocation);
    return TRUE;
}
gboolean SettingStubImp::setScanTime(Set *interface,GDBusMethodInvocation *invocation,gint arg_time)
{
     hsaeCmdSetScanEnableTime(NULL, (unsigned int) arg_time);
     set_complete_set_scan_time (interface,invocation);
     return TRUE;
}

gboolean SettingStubImp::enableautoconncet(Set *interface, GDBusMethodInvocation *invocation, gboolean state)
{
     LOG(BT_LOG_SET,"state =%d", state);
     std::shared_ptr< SettingStubImp > g_set= SettingStubImp::getInstance();
//     unsigned long test = GetTickCountTest()-testcount;
//     printf("enableautoconncet =%d,powersate =%d\r\n",test,powersate);
    if (powersate == true )
    {
        if (state)
         {

            g_set->startAutoConnectThread();
         }
         else
         {
            g_set->closeAutoConnectThread();
         }
    }
     set_complete_set_auto_answer(interface,invocation);
      return TRUE;
}

//gboolean SettingStubImp::setautoconect(Set *interface, GDBusMethodInvocation *invocation, gboolean state)
//{
//     LOG(BT_LOG_SET,"state =%d", state);
//     set_complete_set_auto_connect(interface,invocation);
//     return TRUE;
//}



/**
 * powerState value 0:off 1:on 2:unknown
 */
void SettingStubImp::BTPowerStateChanged(bool powerState)
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();

    LOG(BT_LOG_SET,"powerState =%d", powerState);

    if(m_interface != NULL)
    {
        set_emit_power_state_changed (m_interface, powerState);
    }
     g_device->m_property.powered  = powerState;
     device_set_power_state (DeviceStubImp::interface, powerState);
     set_set_power_state ( m_interface, powerState);

    if (powerState)
    {
        // startAutoConnectThread();
    }
    else
    {
        closeAutoConnectThread();
    }

}

void SettingStubImp::BroadcastbtVersion(string version)
{

    LOG(BT_LOG_SET,"version =%s m_interface = %d", version.data(),m_interface);
    setautoaddress();
    if (m_interface)
    {
        set_emit_version_state_changed(m_interface,version.data());
        set_set_btversion( m_interface,version.data());
        set_emit_auto_state_changed(m_interface,m_benableconnect);
    }

}


void SettingStubImp::setPairable(bool pairable)
{
   hsaeCmdSetPairDeviceNum(NULL, int (pairable) );
}


gboolean SettingStubImp::setAutoConnection(Set *interface, GDBusMethodInvocation *invocation, gboolean state)
{
     LOG(BT_LOG_SET,"setautoconect =%d", state);
     std::shared_ptr< SettingStubImp > g_set = SettingStubImp::getInstance();
     set_set_auto_state(interface,state);
     g_set->m_benableconnect = state;
     FILE *fp = NULL;
     fp =fopen(AUTO_CONNECT_FLG,"rt+");
     //char buf[]='';
     if (fp !=NULL)
     {
         char buf[1]={0};
         if (state)
         {
           buf[0]='1';
         }
         else
           buf[0]='0';
         fseek(fp,0,SEEK_SET);
         fwrite(buf, 1, 1, fp);

         fclose(fp);
      }

     fp=NULL;
     system("sync");
     set_complete_set_auto_connect(interface,invocation);
     return TRUE;
}
string SettingStubImp::getautoaddr()
{
    return auto_addr;
}

gboolean SettingStubImp::setFactory(Set *interface, GDBusMethodInvocation *invocation)
{
      LOG(BT_LOG_SET," ");
//     //open btpower
     std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();
     std::shared_ptr< PhoneBookStubImp > g_book = PhoneBookStubImp::getInstance();
//     HSAE_POWER HSAEPower;
//     hsaeCmdGetPower( NULL, &HSAEPower );
//     LOG(BT_LOG_SET,"power =%d", (int)HSAEPower);
     hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::hfp_connect_addr, HSAE_PROFILEID_e::NPI_ALL_PROFILE);
     g_device->clearpairlist();
     g_book->clearsqltables();
     set_set_auto_state(interface,false);
     set_emit_auto_state_changed(interface,false);
    //remove autoconncetfile
     FILE *fp = NULL;
     fp =fopen(AUTO_CONNECT_FLG,"w+");
     if (fp !=NULL)
     {
        char buf[1]={0};
        buf[0] = '0';
        fseek(fp,0,SEEK_SET);
        fwrite(buf, 1, 1, fp);
        fclose(fp);
      }
      fp=NULL;
      system("rm /var/bluetooth/* -rf \r");
//      if( HSAEPower== HSAE_POWERON)
//      {

//      }
//      else
//      {
//         //if the system not restart we should restart service
//         system("systemctl stop hsaebtserver.service  \r");
//      }
      system("sync");
      set_complete_factoryreset(interface,invocation);
      return TRUE;
}


void SettingStubImp::closeAutoConnectThread()
{
    LOG(BT_LOG_SET,"closeAutoConnectThread = %d ",autoconnect_device_thread_id);
    if (autoconnect_device_thread_id != 0)
         g_bRun = false;
}

void SettingStubImp::startAutoConnectThread()
{

    setautoaddress();

    if( (m_benableconnect == false) ||(auto_addr == ""))
    {

         return;
    }
    if (autoconnect_device_thread_id == 0)
    {

        LOG(BT_LOG_SET,"start  = %d,auto_addr =%s  ",m_benableconnect,auto_addr.data());
        g_bRun = true;
        pthread_create(&autoconnect_device_thread_id, NULL, autoConnectDeviceFunc, (void *)this);
        pthread_detach(autoconnect_device_thread_id);
    }
}
void *SettingStubImp::autoConnectDeviceFunc(void *args)
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();

    SettingStubImp*pset =(SettingStubImp*)args;
    int count =0;

    while(pset->g_bRun)
    {
//          usleep(3000*1000);
          LOG(BT_LOG_SET,"autoConnectDeviceFunc =%s", pset->auto_addr.data());
          if (pset->auto_addr =="")
              break;
          if (!pset->g_bRun)
              break;
             if (count<2)
             {
               count++;
             }
             else if (count ==3)
                count = 4;
             else
                count = 9;
             BT_ADDR m_btAddress;
             size_t i;
             for( i = 0; i < 6; i++ )
             {
                 m_btAddress.b[i] = 0;
             }
             pset->str_to_char( pset->auto_addr.data(), m_btAddress.b );
             HSAE_CMD_RET ret;    
             if (g_device->currentStates.hfpState == ConnectionState::DISCONNECTION)
             {

                 if (!g_device->IsArrayEmpty(DeviceStubImp::avrcp_connect_addr.b,6))
                 {

                     ret = hsaeCmdDisconnectRemoteDevice( NULL, &DeviceStubImp::avrcp_connect_addr,  NPI_ALL_PROFILE );
                     printf("avrcp is connected,but hfp is disconnect so we disconnect avrcp ");
                 }
                 else
                 {
                      unsigned long test = GetTickCountTest() ;
                      int manu_id = 0;
                       hsaeCmdGetRemoteDeviceManufacturerID(NULL,&m_btAddress,&manu_id);
//                      printf("--------now connect spp =%d----------------\r\n",manu_id);
//                      if (manu_id == 1 )
//                      {
//                          SPPStubImp::btSppInstance->setios(true);
//                          ret=  hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,NPI_SPP_IAP );
//                      }
//                      else
//                      {
//                          SPPStubImp::btSppInstance->setios(false);
//                          ret = hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,HSAE_PROFILEID_e::NPI_SPP );
//                      }
                     ret = hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,HSAE_PROFILEID_e::NPI_ALL_AUDIO );


                 }
             }

             if (!pset->auto_addr.empty() )
                 LOG(BT_LOG_SET,"btAddress444 =%s,ret =%d",pset->auto_addr.data(),(int)ret);
             for (int i=0;i<count*20;i++)
             {
                 usleep(1000*1000);
                 if (!(pset->g_bRun))
                     break;
             }
    }

     pset->g_bRun =false;
     pset->autoconnect_device_thread_id = 0;
}

void SettingStubImp::setautoaddress()
{
//    char filepath=AUTO_CONNECT_FLG;
        FILE *fp = NULL;
        fp =fopen(AUTO_CONNECT_FLG,"rt+");

        char buf[100]={0};
        if (fp !=NULL)
        {
            fseek(fp,0,SEEK_SET);
            fread(buf, 1, 100, fp);
            if (buf[0]=='1')
            {
                m_benableconnect= true;
                set_set_auto_state(m_interface,true);

            }
            else if (buf[0]=='0')
            {
                m_benableconnect =false;
                set_set_auto_state(m_interface,false);
            }
            if (ftell (fp)>17)
                auto_addr =string(buf).substr(1,17);
            else
                auto_addr ="";
            cout<<"-------------------------SettingStubImp::setautoaddress----------"<<auto_addr<<(int)m_benableconnect<<endl;

            fclose(fp);
        }
        else
             auto_addr ="";

        fp=NULL;
        system("sync");
    //auto_addr = addr;
}


void SettingStubImp::setautoconnectmac(char* mac)
{

    LOG(BT_LOG_SET," mac =%s",mac);
     FILE *fp = NULL;
     fp =fopen(AUTO_CONNECT_FLG,"rt+");
      if (fp !=NULL)
      {

         fseek(fp,1,SEEK_SET);
         if (mac != NULL )
         {
             fwrite(mac, 1, 17, fp);
              auto_addr = string(mac);
         }
         else
         {
              char buf[17] ;
              //buf[0]= 0x0;
              memset(buf,0,17);
              fwrite(buf, 1, 17, fp);
               auto_addr = "";
         }

          fclose(fp);

       }

       fp=NULL;
       system("sync");
}
