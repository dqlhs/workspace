#include "../include/hsaewifi.h"
#include <gio/gio.h>
 #include<algorithm>
#include "../include/spp.h"
#include "../include/hsaeble.h"
#include "../include/device.h"
#define BLE_PATHNAME        "/com/hsaeyz/bluetooth/ble"
#define BUSNAME             "com.hsaeyz.bluetooth"
#define BR_PATHNAME         "/com/hsaeyz/bluetooth/spp"
#define DEVICE_PATHNAME     "/com/hsaeyz/bluetooth/device"
#define GDBUS_BUSNAME        G_BUS_TYPE_SYSTEM

#include <glib-2.0/glib.h>  /** GLIB for main loop*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
//#include <map>
/*hicar start*/
char testrfcommuuid1[128] = "0000FE3500001000800000805F9B34FB";
char testrfcommuuid2[128] = "0000FE3600001000800000805F9B34FB";

//extern FcDriverAdaptOps  g_fcAdaptBleOps;
const char deviceid[3]={0x00,0x04,0x14};
int StartAdv(const void *priv);
int StopAdv(const void *priv);
int SendData(const unsigned char *data, int dataLen,  const void *priv);  //??????????????????????
int RegisterBrCallback( const ChannelCallback* btCallback);
int ConnectChannel(const void* btParams);
int DisconnectChannel(const void* btParams);
void StrToHex( char *pbDest, char *pszSrc, int nLen);
int SetBrParam(SetParamsType paramType, const void *priv);
int GetBrParam(GetParamsType paramType, void *result);
int SetBleParam(SetParamsType paramType,const void *priv);
int GetBleParam(GetParamsType paramType,void *result);
int initbt(const void *ctx, const char *ifName);
int deinitbt(const void *ctx);
/*hicar end*/
//extern FcDriverAdaptOps * pDriverAdaptOps[5];

static GMainLoop *pLoop = NULL;
static GDBusConnection *pConnection = NULL;
static Hsaeble *blepProxy = NULL;
static Spp *sppProxy = NULL;
static Device *deviceProxy = NULL;
ChannelCallback g_brcallback ;
Hsaewifi*Hsaewifi::btInstance =NULL;
bool registerbleSignalHandler(Hsaeble *pProxy);
/*ble method*/
bool registerbrSignalHandler(Spp *pProxy);
using namespace std;
static gboolean sigal_recvdata(Spp *object, GVariant *arg_data,gint arg_datalen);
static gboolean sigal_chanelstate(Device *object,  const gchar *arg_address, gint arg_profileId,gint arg_connectState);


static void cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata);


 struct FcDriverAdaptOps g_fcDriverBrOps = {
           name  :"br",
           desc  :"br driver",
           Init:initbt,
           Deinit:deinitbt,
           SetParam : SetBrParam,
           GetParam : GetBrParam,
           StartAdv:NULL,
           StopAdv:NULL,
           SetAdvData:NULL,
           StartScan:NULL,
           StopScan:NULL,
           SendData:SendData,
           NotifyData:NULL,
           DisplayPin:NULL,
           PushMsgBox:NULL,
           CreateWifiChannel:NULL,
           Connect:ConnectChannel,
           Disconnect : DisconnectChannel,
           RegisterChannelCallback:RegisterBrCallback,

};
 struct FcDriverAdaptOps g_fcAdaptBleOps = {
               name  :"ble",
               desc  :"ble driver",
               Init:initbt,
               Deinit:NULL,
               SetParam : SetBleParam,
               GetParam : GetBleParam,
               StartAdv:StartAdv,
               StopAdv:StopAdv,
               SetAdvData:NULL,
               StartScan:NULL,
               StopScan:NULL,
               SendData:NULL,
               NotifyData:NULL,
               DisplayPin:NULL,
               PushMsgBox:NULL,
               CreateWifiChannel:NULL,
               Connect:NULL,
               Disconnect : NULL,
               RegisterChannelCallback:NULL,

    };
//typedef struct
//{
//      char hsaeuuid[16];
//}UUID;
// UUID rfcommuuid[2];
 void StrToHex( char *pbDest, char *pszSrc, int nLen)
 {

     char h1, h2;
     char s1, s2;
     for (int i = 0; i < nLen; i++)
     {

         h1 = pszSrc[2 * i];
         h2 = pszSrc[2 * i + 1];
         s1 = toupper(h1) - 0x30;
         if (s1 > 9)
              s1 -= 7;
         s2 = toupper(h2) - 0x30;
         if (s2 > 9)
              s2 -= 7;
         pbDest[i] = s1 * 16 + s2;

     }
 }
 int SetBleParam(SetParamsType paramType, const void *priv)
 {
     if (priv == NULL)
        return HASEERROR;
     BleParams *ble = (BleParams*)priv;
     printf("[HSAE]:Enter SetBleParam,ble->minInterval =%d ble->maxInterval =%d ,ble->timeout =%d\r\n",\
            ble->minInterval,ble->maxInterval,ble->timeout);
     printf("[HSAE]:advdata len is %d ",ble->advDataLen);
     for (int i =0;i<ble->advDataLen;i++ )
         printf ("%0x ",ble->nearbyAdvData[i]);
     printf("\r\n");
     memcpy(Hsaewifi::btInstance->m_bleParams,&ble->minInterval,sizeof(int));
     memcpy(Hsaewifi::btInstance->m_bleParams+4,&ble->maxInterval,sizeof(int));
     memcpy(Hsaewifi::btInstance->m_bleParams+8,&ble->timeout,sizeof(int));
    if (ble->advDataLen <=30)
        memcpy(Hsaewifi::btInstance->m_bleParams+12,ble->nearbyAdvData,ble->advDataLen);
     else
        return HASEERROR;
     Hsaewifi::btInstance->m_bledatalen = ble->advDataLen +12;
     return HSAESUCCESS;
 }
 int GetBleParam(GetParamsType paramType,  void *result)
 {
     printf("[HSAE]:Enter GetBleParam");
    if (GET_MODEL_ID==paramType)
     {
         memcpy((*(DeviceModelId*)result).modelId,deviceid,3);
         printf(" deviceid is ");
         for (int i =0;i<3 ; i++)
             printf("%0x ",deviceid[i] );
         printf("\r\n");
         ((DeviceModelId*)result)->subModelId = 0x01;

     }
    return HSAESUCCESS;
 }

 int SetBrParam(SetParamsType paramType, const void *priv)
{
#if 1
     printf("[HSAE]:Enter SetBrParam  SetParamsType %d ",paramType);
     if (paramType == SET_BR_UUID )
     { 
         ServiceUuidRelatedSessionId brParam = *(ServiceUuidRelatedSessionId*)priv;
         if (brParam.sessionId > 4)
             return HASEERROR;
         printf("%s",brParam.serviceUuid);
         string deviceid = string( (char *)brParam.serviceUuid);
         deviceid.erase(std::remove(deviceid.begin(),deviceid.end(),'-'),deviceid.end()); 
         printf("%d,len =%d,%s",brParam.sessionId,deviceid.length(),deviceid.data());
         if (deviceid.length() < 32)
         {
            // return HASEERROR;
             if (brParam.sessionId == 1)
             {
                 memcpy(Hsaewifi::btInstance->rfcomm_id[0].uuid,brParam.serviceUuid,BT_SERVICE_UUID_LEN);
                 deviceid =string(testrfcommuuid1);
             }
             else
             {
                 memcpy(Hsaewifi::btInstance->rfcomm_id[1].uuid,brParam.serviceUuid,BT_SERVICE_UUID_LEN);
                 deviceid =string(testrfcommuuid2);
             }
         }


         unsigned char uuid[16]={0};
         StrToHex((char*)uuid,(char*)deviceid.data(),16);
         for(int i =0;i<16;i++)
            printf("%0x ",uuid[i]);
         printf("\r\n");
          char buf[17]={0};
        printf("brParam.channelId =%d ",brParam.sessionId);
         switch (brParam.sessionId) {
         case 1:
             buf[0] = 0x01;
             break;
         case 2:
             buf[0] = 0x02;
             break;
         case 3:
             buf[0] = 0x03;
             break;
         case 4:
             buf[0] = 0x04;
             break;
         case 5:
             buf[0] = 0x05;
             break;
         default:
             break;
         }
         memcpy(buf+1,uuid,16);
//         for(int i =0;i<17;i++)
//            printf("%0x ",buf[i]);
//         printf("\r\n");
         GVariant *value;
         GVariantBuilder* builder;
         builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
         for(int i = 0; i < 17; ++i) {
             g_variant_builder_add(builder, "y",buf[i]);
         }
         value = g_variant_new ("ay", builder);
         g_variant_builder_unref (builder);
         spp_call_setparames(sppProxy,value,17,NULL, Hsaewifi::SetParamCallback ,NULL);
     }
#endif

     return HSAESUCCESS;
}

int GetBrParam(GetParamsType paramType,  void *result)
{
    printf("[HSAE]:Enter GetBrParam %d ",paramType);
   if (GET_DEV_MAC == paramType)
   {
        const gchar * mac = device_get_local_address(deviceProxy);
        MacParam *macDst = (MacParam *)result;
        memcpy(macDst->mac,mac,17);
        macDst->len = 17;
        printf(" Localmac =%s \r\n",(char *)macDst->mac);
    }
    else  if (GET_MODEL_ID==paramType)
    {
        memcpy((*(DeviceModelId*)result).modelId,deviceid,3);
        printf(" deviceid is ");
        for (int i =0;i<3 ; i++)
            printf("%0x ",deviceid[i]);
        printf("\r\n");
        ((DeviceModelId*)result)->subModelId = 0x01;

    }
    else  if (GET_MAP_PORT==paramType)
    {

       ServiceUuidMapPort  *brdata =(ServiceUuidMapPort  *)result;
        brdata->port  = 0;
       if ( strcmp(Hsaewifi::btInstance->rfcomm_id[1].uuid,brdata->serviceUuid))
       {
          brdata->port = 4;
       }
       else  if  (strcmp(Hsaewifi::btInstance->rfcomm_id[0].uuid,brdata->serviceUuid))
       {
          brdata->port = 3;
       }
       printf("[HSAE]:uuid %s brport is %d \r\n",brdata->serviceUuid,brdata->port);
   }
    return HSAESUCCESS;
}


int StartAdv(const void *priv)
{
     printf("[HSAE]:Enter StartAdv \r\n");
     if (priv == NULL)
     {
        return HASEERROR;
     }
     BleAdvData *data = (BleAdvData *)priv;
     if (data->advDataLen <=30)
     {

         memcpy(Hsaewifi::btInstance->m_bleParams+12,data->advData,data->advDataLen);
         Hsaewifi::btInstance->m_bledatalen = data->advDataLen+12;
         Hsaewifi::getInstance()->sendbledata((unsigned char*)Hsaewifi::btInstance->m_bleParams,Hsaewifi::btInstance->m_bledatalen);
         return HSAESUCCESS;
     }
     else
     {
         return HASEERROR;
     }
}
int StopAdv(const void *priv)
{
  printf("[HSAE]:Enter StopAdv\r\n");
  hsaeble_call_stop_ble_advertise(blepProxy, NULL, Hsaewifi::StopAdvCallback, NULL);
  return HSAESUCCESS;
}
//
int SendData(const unsigned char *data, int dataLen,  const void *priv)
{
//    char remotemac="";
    if (priv ==NULL)
    {
        return HASEERROR;
    }
    else
    {
        BrChannelData param =*(BrChannelData*)priv;

        //  BrChannelData param;
         printf("[HSAE]:Enter SendData dataLen %d,remoteMac %s,chanel %d data is ",dataLen,param.remoteMac,param.sessionId);
         for (int i =0;i<dataLen ;i++)
             printf("%0x ",data[i]);
         printf("\r\n");
         char *senddata = new  char [dataLen+18];
         if (param.sessionId == 1)
         {
              senddata[0]= 0x01;
         }
         else  if (param.sessionId == 2)
         {
              senddata[0]= 0x02;
         }
         printf("senddata[0] %x",senddata[0]);
         memcpy(senddata+1,param.remoteMac,17);
         memcpy(senddata+18,data,dataLen);
         Hsaewifi::getInstance()->sendbrdata(senddata,dataLen+18);
         delete [] senddata;
         return HSAESUCCESS;
    }


}
void Hsaewifi::SetParamCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{
    g_print("Enter SetParamCallback call.\n");
    gchar* out_arg = NULL;
    GError* pError = NULL;
    spp_call_setparames_finish(sppProxy, res, &pError);

    if(NULL == pError){

//        if(NULL != g_method_callback.callback1){
//            g_method_callback.callback1(out_arg, pError);
//            g_free(out_arg);
//        }
//         g_free(out_arg);
    }
    else{
        g_error_free(pError);
    }
}
//int Hsaewifi::RegisterChannelCallback(ChannelParams *btParams, ChannelCallback *btCallback)

int RegisterBrCallback(const ChannelCallback *btCallback)
{
    printf("[HSAE]:Enter RegisterChannelCallback\r\n");
    g_brcallback.ChannelStateChange =btCallback->ChannelStateChange;
    g_brcallback.ScanCallback =  btCallback->ScanCallback;
    g_brcallback.ReceiveData = btCallback->ReceiveData;
    return HSAESUCCESS;
}

//int Hsaewifi::ConnectChannel(ChannelParams *btParams)
int ConnectChannel(const void *btParams)
{

   if (btParams == NULL)
   {
        return HASEERROR;
   }
   BrChannelData param =*(BrChannelData*)btParams;
//   UUID buf;
//   memcpy(buf.hsaeuuid,param.deviceId,16);
   PROFILEID id = RFCOMM_1;
   printf("[HSAE]:Enter ConnectChannel =%d\r\n",param.sessionId);
   if (param.sessionId == 1)
   {
           id = RFCOMM_3;

   }
   else  if (param.sessionId == 2)
   {
           id = RFCOMM_4;
   }
   std::string mac = std::string(param.remoteMac);
   Hsaewifi::getInstance()->connectdevice((char *)mac.data(),id,true);
   return HSAESUCCESS;
}

//int Hsaewifi::DisconnectChannel(ChannelParams *btParams)
int DisconnectChannel(const void *btParams)
{
     printf("[HSAE]:enter DisconnectChannel\r\n");
     if (btParams == NULL)
     {
          return HASEERROR;
     }
     BrChannelData param =*(BrChannelData*)btParams;
     PROFILEID id = RFCOMM_1;

     if (param.sessionId == 1)
     {
        id = RFCOMM_1;
     }
     else  if (param.sessionId == 2)
     {      
         id = RFCOMM_2;
     }
     else  if (param.sessionId == 0)
     {

        id = ALL_RFCOMM;
     }
     Hsaewifi::getInstance()->connectdevice(param.remoteMac,id,false);
     return HSAESUCCESS;

}
static int btinitnum =0;
int deinitbt (const void *ctx)
{
    printf("[HSAE]:deinitbt() enter \r\n");
    BrChannelData param;
    param.sessionId = 0;
    string mac ="error";
    memset(param.remoteMac,0,STR_MAC_ADDR_LEN);
    memcpy(param.remoteMac,mac.data(),mac.length());
    DisconnectChannel(&param);
    return HSAESUCCESS;
}
int initbt(const void *ctx, const char *ifName)
{
   printf("[HSAE]:initbt() =%d \r\n",btinitnum);
   if (btinitnum == 0)
    {
        Hsaewifi*pbt = Hsaewifi::getInstance();
        pbt->InitDBusCommunication();
        while(!(pbt->ExampleDBusServiceIsReady()));
        btinitnum++;
        printf("ExampleDBusServiceIsReady()end \r\n");
        g_brcallback.ChannelStateChange = NULL;
        g_brcallback.ReceiveData =NULL;
    }
    return HSAESUCCESS;
}

void  cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata)
{
    printf("enter cb_OwnerNameChangedNotify arg_datalen \r\n");
}
gboolean  sigal_recvdata(Spp *object, GVariant *arg_data, gint arg_datalen)
{
   printf("[HASE]:enter sigal_recvdata arg_datalen =%d \r\n",arg_datalen);
    GVariantIter *iter = NULL;
    unsigned char str[10*1024]={0} ;
    g_variant_get(arg_data, "ay", &iter);
    int i =0;
    while (g_variant_iter_loop(iter, "y", str+i))
    {
         printf(" %x ", str[i]);
       i++;
    }
    BrChannelData brparam;
    memset(&brparam,0,sizeof(BrChannelData));
    switch (str[0]) {
    case 0x01:
         brparam.sessionId = 1;
        break;
    case 0x02:
         brparam.sessionId = 2;
        break;
    case 0x03:
        brparam.sessionId = 3;
        break;
    case 0x04:
        brparam.sessionId = 4;
        break;
    case 0x05:
        brparam.sessionId = 5;
        break;
    }

    memcpy(brparam.remoteMac,str+1,17);
//    device_get_local_address(deviceProxy);
//    const gchar * mac = device_get_local_address(deviceProxy);
//    memcpy(brparam.localMac,mac,17);

    if (g_brcallback.ReceiveData != NULL)
    {
        printf("\r\n [HASE]:brparam  channle=%d,romacte =%s\r\n",brparam.sessionId,brparam.remoteMac);
        g_brcallback.ReceiveData(CHANNEL_BREDR,&brparam,str+18,arg_datalen-18);

    }
    else
    {
        printf("[HASE]:ERROR ReceiveData is  NULL \r\n");
    }
    g_variant_iter_free (iter);

}
//  static int t=0;
gboolean sigal_chanelstate(Device *object,  const gchar *arg_address, gint arg_profileId,gint arg_connectState)
{
//    t++;
    printf("[HSAE]:arg_address =%s,arg_profileId =%d,arg_connectState  =%d\r\n",arg_address ,arg_profileId,arg_connectState);
    EventType type = EVENT_DISCONNECT;
//    printf("=========================RFCOMM_4 =%d=,%d===================\r\n",arg_profileId == RFCOMM_4,RFCOMM_4);
    if ((arg_profileId == RFCOMM_1)||(arg_profileId == RFCOMM_2)||\
            (arg_profileId == RFCOMM_3)||(arg_profileId == RFCOMM_4))
    {    

        if (arg_connectState == 2)
        {
            Hsaewifi::getInstance()->m_connectmac = arg_address;
            type = EVENT_CONNECT;
        }
         else
         {
//             Hsaewifi::getInstance()->m_connectmac ="";
         }

        BrChannelData brdata;
        memset(&brdata,0,sizeof(BrChannelData));
        memcpy(brdata.remoteMac,arg_address,17);
//        printf("[HSAE]:id22222 =%d,arg_connectState  =%d\r\n",brdata.sessionId ,arg_connectState);
        switch (arg_profileId) {
        case RFCOMM_1:
            brdata.sessionId = 1;
            break;
        case RFCOMM_2:
            brdata.sessionId = 2;
            break;
        case RFCOMM_3:
            brdata.sessionId = 1;
            break;
        case RFCOMM_4:
            brdata.sessionId = 2;
            break;

        }
        if ( g_brcallback.ChannelStateChange != NULL)
        {
            g_brcallback.ChannelStateChange(CHANNEL_BREDR,type,&brdata,sizeof(BrChannelData));
            printf("[HSAE]:id =%d,arg_connectState  =%d\r\n",brdata.sessionId ,arg_connectState);
        }
        else
        {
            printf("[HSAE]:ERROR ChannelStateChange is NULL ");
        }


    }
}
bool registerbrSignalHandler(Spp *pProxy)
{
    printf("[HSAE]:enter registerbrSignalHandler deviceProxy =%d\r\n",deviceProxy);
    g_signal_connect(sppProxy, "notify::g-name-owner", G_CALLBACK(cb_OwnerNameChangedNotify), NULL);
    g_signal_connect(deviceProxy, "connect-state-changed", G_CALLBACK(sigal_chanelstate), NULL);
    g_signal_connect(sppProxy, "channeldata", G_CALLBACK(sigal_recvdata), NULL);
}

bool  registerbleSignalHandler(Hsaeble *pProxy)
{

}


void Hsaewifi::sendbledata(unsigned char *data, int len)
{

   GVariant *value;
   GVariantBuilder* builder;

   builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
   for(int i = 0; i < len; ++i) {
       g_variant_builder_add(builder, "y", data[i]);
   }
   value = g_variant_new ("ay", builder);
   g_variant_builder_unref (builder);
   hsaeble_call_start_ble_advertise(blepProxy, value,len, NULL, StartAdvCallback, NULL);
}

int Hsaewifi::connectdevice(char* mac, PROFILEID profile, bool state)
{
   printf("[HSAE]:Enter connectdevice = %s,profile =%d,state =%d \r\n",mac,profile,state);
   if (state ==true)
      device_call_connect_device(deviceProxy, mac,profile, NULL, connectCallback, NULL);
   else
   {
       device_call_dis_connect_device(deviceProxy, mac,profile, NULL, disconnectCallback, NULL);
   }
   return HSAESUCCESS;
}

void Hsaewifi::connectCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{
    g_print("bledataCallback call.\n");
    gchar* out_arg = NULL;
    GError* pError = NULL;

    device_call_connect_device_finish(deviceProxy, res, &pError);

    if(NULL == pError){
//        if(NULL != g_method_callback.callback1){
//            g_method_callback.callback1(out_arg, pError);
//            g_free(out_arg);
//        }
//         g_free(out_arg);
    }
    else{
        g_error_free(pError);
    }
}

void Hsaewifi::disconnectCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{
    g_print("bledataCallback call.\n");
    gchar* out_arg = NULL;
    GError* pError = NULL;

    device_call_dis_connect_device_finish(deviceProxy, res, &pError);

    if(NULL == pError){
//        if(NULL != g_method_callback.callback1){
//            g_method_callback.callback1(out_arg, pError);
//            g_free(out_arg);
//        }
//         g_free(out_arg);
    }
    else{
        g_error_free(pError);
    }
}


void Hsaewifi::sendbrdata(char *data, int len)
{
//    g_print("client data call, in_arg = %s\r\n", data);

    GVariant *value;
    GVariantBuilder* builder;
    builder = g_variant_builder_new(G_VARIANT_TYPE("ay")); 
    for(int i = 0; i < len; ++i) {
        g_variant_builder_add(builder, "y", data[i]);
    }
    value = g_variant_new ("ay", builder);

    spp_call_senddata(sppProxy,value,len,NULL, SendDataCallback ,NULL);
    g_variant_builder_unref (builder);
}
void Hsaewifi::StartAdvCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{

    gchar* out_arg = NULL;
    GError* pError = NULL;

    hsaeble_call_start_ble_advertise_finish(blepProxy, res, &pError);

    if(NULL == pError){

    }
    else{
        g_error_free(pError);
    }
}
void Hsaewifi::StopAdvCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{
//    g_print("bledataCallback call.\n");
    gchar* out_arg = NULL;
    GError* pError = NULL;

    hsaeble_call_stop_ble_advertise_finish(blepProxy, res, &pError);

    if(NULL == pError){
//        if(NULL != g_method_callback.callback1){
//            g_method_callback.callback1(out_arg, pError);
//            g_free(out_arg);
//        }
//         g_free(out_arg);
    }
    else{
        g_error_free(pError);
    }
}




void Hsaewifi::SendDataCallback(GObject *source_object, GAsyncResult *res, gpointer userdata)
{
//    g_print("brdataCallback call.\n");
    gchar* out_arg = NULL;
    GError* pError = NULL;

    spp_call_senddata_finish(sppProxy, res, &pError);

    if(NULL == pError){
//        if(NULL != g_method_callback.callback1){
//            g_method_callback.callback1(out_arg, pError);
//            g_free(out_arg);
//        }
    }
    else{
        g_error_free(pError);
    }
}



bool Hsaewifi::ExampleDBusServiceIsReady()
{
    printf("Hsaewifi::ExampleDBusServiceIsReady() \r\n");
    gchar *owner_name = NULL;
    owner_name = g_dbus_proxy_get_name_owner((GDBusProxy*)blepProxy);
    if(NULL != owner_name)
    {
        g_print("Owner Name: %s\n", owner_name);
        g_free(owner_name);
        return true;
    }
    else
    {
        g_print("Owner Name is NULL.");
        return false;
    }
      printf("Hsaewifi::ExampleDBusServiceIsReady() end\r\n");
}


void *run(void* arg)
{
   // g_print("run call in the client.\n");
    /** start the main event loop which manages all available sources of events */
    g_main_loop_run(pLoop);

    return ((void*)0);
}

int thread_create(void)
{
    int err;
    pthread_t thr;

    err = pthread_create(&thr, NULL, run, NULL);

    if(0 != err){
            g_print("Can't create thread: %s\n", strerror(err));
    }
    else{
            g_print("New thread created: %s\n", strerror(err));
    }

    return err;
}
bool Hsaewifi::InitDBusCommunication()
{
    bool bRet = TRUE;
    GError *pConnError = NULL;
    GError *pProxyError = NULL;
    g_type_init();  /** initial for usage of "g" types */
    do{
        bRet = TRUE;
        pLoop = g_main_loop_new(NULL, FALSE);   /** create main loop, but do not start it.*/

        /** First step: get a connection */
        pConnection = g_bus_get_sync(GDBUS_BUSNAME, NULL, &pConnError);

        if (NULL == pConnError){
            /** Second step: try to get a connection to the given bus.*/
            blepProxy = hsaeble_proxy_new_sync(pConnection,G_DBUS_PROXY_FLAGS_NONE,\
                                                        BUSNAME,
                                                        BLE_PATHNAME,
                                                         NULL,
                                                         &pProxyError);
            if (0 == blepProxy){
                g_print("InitDBusCommunication: Failed to create bleproxy. Reason: %s.\n", pProxyError->message);
                g_error_free(pProxyError);
                bRet = FALSE;
            }
            /** Second step: try to get a connection to the given bus.*/
            sppProxy = spp_proxy_new_sync(pConnection,G_DBUS_PROXY_FLAGS_NONE,\
                                                        BUSNAME,
                                                        BR_PATHNAME,
                                                         NULL,
                                                         &pProxyError);
            if (0 == sppProxy){
                g_print("InitDBusCommunication spp :Failed to create sppproxy. Reason: %s.\n", pProxyError->message);
                g_error_free(pProxyError);
                bRet = FALSE;
            }
            deviceProxy = device_proxy_new_sync(pConnection,G_DBUS_PROXY_FLAGS_NONE,\
                                                         BUSNAME,
                                                         DEVICE_PATHNAME,
                                                         NULL,
                                                         &pProxyError);
            if (0 == deviceProxy){
                g_print("InitDBusCommunication device :Failed to create deviceProxy. Reason: %s.\n", pProxyError->message);
                g_error_free(pProxyError);
                bRet = FALSE;
            }

        }
        else{
            g_print("InitDBusCommunication: Failed to connect to dbus. Reason: %s.\n", pConnError->message);
            g_error_free(pConnError);
            bRet = FALSE;
        }


    }while(FALSE == bRet);

    if (TRUE == bRet){
        /** Third step: Attach to dbus signals */
        registerbrSignalHandler(sppProxy);
//        registerbleSignalHandler(blepProxy);
    }
      thread_create();
}

bool Hsaewifi::DeInitDBusCommunication()
{

}




Hsaewifi::Hsaewifi()
{
    btInstance =NULL;
    memset(m_bleParams,0,sizeof(BleParams));
    m_bledatalen =0;
    m_connectmac ="";
    memset(rfcomm_id,0,sizeof(RF_UUID));
//    gbtcakkback.OnChannelStateChange  =NULL;
//    gbtcakkback.OnDataRead = NULL;
}

Hsaewifi *Hsaewifi::getInstance()
{
  if (btInstance == NULL)
  {
      btInstance = new Hsaewifi();
  }
  return btInstance;
}




Hsaewifi::~Hsaewifi()
{

}





