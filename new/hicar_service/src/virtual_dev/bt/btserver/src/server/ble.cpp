#include "./ble.h"

static unsigned char s_AdvContextUUID[16] = {0xda, 0x8b, 0xf5, 0xd9, 0x3a, 0x59, 0x42, 0x18, 0x99, 0x44, 0xf3, 0xfe, 0xfd, 0xb2, 0x12, 0x47};
static unsigned char s_GATTSvrContextUUID[16] = {0x6a, 0xea, 0x74, 0xb3, 0xfb, 0xdc, 0x45, 0xe8, 0x82, 0xd6, 0x51, 0xb3, 0x76, 0x6d, 0x89, 0x4a};
static unsigned char s_advData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
std::shared_ptr< BleImp>  BleImp::bleInstance = NULL;
Hsaeble* BleImp::m_interface = NULL;
 extern unsigned long GetTickCountTest();
 BleImp::BleImp()
{
   memset(&advertise, 0, sizeof(HSAE_GATT_Advertise_Info_t));

   mcount = 0;
   m_interval = 1;
   s_GATTServer_Adv = false;
}

std::shared_ptr<BleImp> BleImp::getInstance()
{
    if( bleInstance == NULL )
    {
        bleInstance = std::make_shared< BleImp >();
    }
    return bleInstance;
}


bool BleImp::initble()
{
    
}

void BleImp::broadcastdata( )
{
    mcount =0;

    if (m_interval >0)
    {
        m_createtimer.SetTimer(m_interval,0,timeout);
        m_createtimer.StartTimer();
    }
    GATTServerEnableAdv();
}

void BleImp::setintervals(int interval)
{
    m_interval = interval;
}


void BleImp::GATTServerDisableAdv()
{
    printf("enter BleImp::stopBleAdvertis \r\n ");
    HSAE_UUID_t  temp;
    memcpy(temp.uuid ,s_AdvContextUUID,16);
    hsaeCmdGATTStopAdvertise(NULL, &temp);
    s_GATTServer_Adv = false;
}
void BleImp::GATTServerEnableAdv()
{
    HSAE_UUID_t gattAdvContext;
    HSAE_GATT_ScanResponse_Info_t scanResponse;
    memset(&scanResponse, 0, sizeof(HSAE_GATT_ScanResponse_Info_t));
    advertise.includeName = 0; //advdata include device name;
    scanResponse.includeName = 1;
    advertise.maxSlaveConnectInterval =  0x0052;
    advertise.minSlaveConnectInterval = 0x0020;
    memcpy(gattAdvContext.uuid, s_AdvContextUUID, 16);
    hsaeCmdGATTStartAdvertiseEx(NULL, &gattAdvContext, &advertise, &scanResponse,  adv_min_interval, adv_max_interval);
    s_GATTServer_Adv = true;

}

void BleImp::timeout()
{
    bleInstance->mcount = 0;
    bleInstance->GATTServerDisableAdv();
    bleInstance->m_createtimer.StopTimer();
}

BleImp::~BleImp()
{

}

gboolean BleImp::startBleAdvertise(Hsaeble *object, GDBusMethodInvocation *invocation, GVariant *arg_data, gint arg_datalen)
{

    printf("enter BleImp::startBleAdvertise \r\n");
    GVariantIter *iter = NULL;
    unsigned char str[1024]={0} ;
    g_variant_get(arg_data, "ay", &iter);
    int i =0;
    while (g_variant_iter_loop(iter, "y", str+i))
    {
       printf(" %x", str[i]);
       i++;
    }

   g_variant_iter_free (iter);
   int max ;
   int min;
   int timeout;
   memcpy(&min,str,4);
   memcpy(&max,str+4,4);
   memcpy(&timeout,str+8,4);
 //  printf("min = %d ,max = %d ,timeout =%d\r\n", min,max,arg_datalen-12);
   memset(bleInstance->advertise.serviceData,0,29);
   memcpy(bleInstance->advertise.serviceData,str+12,arg_datalen-12) ;
   bleInstance->advertise.serviceDataLength = arg_datalen-12;
   bleInstance->adv_min_interval =min;
   bleInstance->adv_max_interval =max;
   bleInstance->m_interval  = timeout;
//   printf("min = %d  \r\n", GetTickCountTest() );
   bleInstance->broadcastdata();//GATTServerEnableAdv();

   hsaeble_complete_start_ble_advertise(object,invocation);
   return TRUE;
}

gboolean BleImp::stopBleAdvertise(Hsaeble *object, GDBusMethodInvocation *invocation)
{
    printf("enter BleImp::stopBleAdvertis \r\n ");
    bleInstance->GATTServerDisableAdv();
 //   bleInstance->GATTServerEnableAdv();
    hsaeble_complete_stop_ble_advertise(object,invocation);
    return TRUE;
}

void BleImp::on_bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    GError **error;

    //创建一个skeleton对象,以注册输出对象
    m_interface = hsaeble_skeleton_new();

    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect(m_interface,
                     "handle-start-ble-advertise",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(startBleAdvertise),      //具体的方法处理函数, 注意增加输入参数
                     user_data);
    g_signal_connect(m_interface,
                     "handle-stop-ble-advertise",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(stopBleAdvertise),      //具体的方法处理函数, 注意增加输入参数
                     user_data);

//    //输出interface到总线上
    if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (m_interface),
                                           connection,
                                           "/com/hsaeyz/bluetooth/ble", //object_path
                                           error))
    {

    }
}
