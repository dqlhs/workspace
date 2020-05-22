#include "spp.h"
#include <string.h>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
//#define IAP_EA_PROTOCOL_TOKEN_STRING_HSAE         "com.example.apple-samplecode.EADemo.tanzhen"
#define IAP_EA_PROTOCOL_TOKEN_STRING_HSAE         "com.hsae.app.ag35"
//#define IAP_EA_PROTOCOL_TOKEN_STRING_HSAE         "com.baidu.CarLifeVehicleProtocol"
//extern unsigned long testcount;
//extern unsigned long GetTickCountTest();
extern "C"
{
#include "ipod_def.h"
#include "ipod_common.h"
#include "ipod_bt_func.h"
#include "ipod_ea_func.h"

//#include "../iap/ipod_assistivetouch.h"
//#include "../iap/iap2_common_func.h"
}

HSAE_UUID_t rfcomm_1 = {0x8c, 0x19, 0xe1, 0x44, 0x84, 0x52, 0x45, 0x56, 0x99, 0xa7, 0x29, 0xa9, 0x03, 0xd3, 0xcb, 0x3e};
HSAE_UUID_t rfcomm_2 = {0x2f, 0x7a, 0x01, 0xf8, 0x2f, 0xb8, 0x4f, 0x5c, 0xa9, 0xe3, 0xf3, 0xd4, 0x41, 0x17, 0x09, 0x4e};
extern pthread_t SPPStubImp::dealdata_id;
struct timeval last_time;
std::shared_ptr< SPPStubImp > SPPStubImp::btSppInstance = NULL;
Spp* SPPStubImp::m_interface = NULL;
extern pthread_mutex_t SPPStubImp::mutex_for_data;
extern pthread_cond_t SPPStubImp::datacond;
ipod_t handle;
PIPODObserver p_observer;

char* HexNumber2String(unsigned char* hex_num, int hex_len)
{
    int i = 0;
    char* str_num = NULL;

    if (hex_len <= 0)
        return NULL;

    str_num = (char*) malloc(hex_len*2+1);
    memset(str_num, 0, hex_len*2+1);

    for (i=0;i<hex_len;i++)
        sprintf(str_num+i*2, "%02X", hex_num[i]);

    return str_num;
}


void ipod_ea_data_recv_process(int protocol_index, int cDataLen, char *pData)
{
     printf("recv_process %d\r\n",cDataLen);
     GVariant *value;
     GVariantBuilder* builder;
     builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
     char buf[20];
     memset(buf,0,20);//=0;
     if (protocol_index == 1)
     {
         buf[0] =0x01;
     }
     else  if (protocol_index == 2)
     {
         buf[0] = 0x02;
     }
     else  if (protocol_index == 3)
     {
         buf[0] =0x03;
     }
     else  if (protocol_index == 4)
     {
         buf[0] =0x04;
     }
     else  if (protocol_index == 5)
     {
         buf[0] =0x05;
     }
//     for(int j =0;j<17;j++)
     memcpy(buf+1,SPPStubImp::btSppInstance->btmac,17);

     for(int j =0;j<18;j++)
         g_variant_builder_add(builder, "y", buf[j]);
     for(int i = 0; i < cDataLen; ++i) {
         g_variant_builder_add(builder, "y", pData[i]);
     }
     value = g_variant_new ("ay", builder);
     g_variant_builder_unref (builder);
     spp_emit_channeldata(SPPStubImp::m_interface,value,cDataLen+18);
     printf("cDataLen =%d\r\n",cDataLen);

}

HSAE_CBK_RET spp_input(void *HSAECbkParam, const BT_ADDR ba, const char input, const HSAE_PROFILEID ProfileID)
{
    static struct timeval current_time;
      printf( "spp_input %02X:%02X:%02X:%02X:%02X:%02X ", ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5] );
    if( last_time.tv_sec == 0 ){
        gettimeofday( &last_time, NULL );

        switch(ProfileID){
        case NPI_SPP:
            printf( "RFCOMM_0:" );
            break;
        case NPI_SPP_IAP:
            printf( "iAP:" );
            break;
        case NPI_RFCOMM_1:
            printf( "RFCOMM_1:" );
            break;
        case NPI_RFCOMM_2:
            printf( "RFCOMM_2:" );
            break;
        case NPI_RFCOMM_3:
            printf( "RFCOMM_3:" );
            break;
        case NPI_RFCOMM_4:
            printf( "RFCOMM_4:" );
            break;
        case NPI_RFCOMM_5:
            printf( "RFCOMM_5:" );
            break;
        case NPI_RFCOMM_6:
            printf( "RFCOMM_6:" );
            break;
        case NPI_RFCOMM_7:
            printf( "RFCOMM_7:" );
            break;
        case NPI_RFCOMM_8:
            printf( "RFCOMM_8:" );
            break;
        case NPI_RFCOMM_9:
            printf( "RFCOMM_9:" );
            break;
        default:
            break;
        }
    }
    else{
        gettimeofday( &current_time, NULL );

        // if the interval between two message is longer than 1 second,
        // say they are discontinuous input
        if( current_time.tv_sec - last_time.tv_sec > 0 ){

            switch(ProfileID){
            case NPI_SPP:
                printf( "RFCOMM_0:" );
                break;
            case NPI_SPP_IAP:
                printf( "iAP:" );
                break;
            case NPI_RFCOMM_1:
                printf( "RFCOMM_1:" );
                break;
            case NPI_RFCOMM_2:
                printf( "RFCOMM_2:" );
                break;
            case NPI_RFCOMM_3:
                printf( "RFCOMM_3:" );
                break;
            case NPI_RFCOMM_4:
                printf( "RFCOMM_4:" );
                break;
            case NPI_RFCOMM_5:
                printf( "RFCOMM_5:" );
                break;
            case NPI_RFCOMM_6:
                printf( "RFCOMM_6:" );
                break;
            case NPI_RFCOMM_7:
                printf( "RFCOMM_7:" );
                break;
            case NPI_RFCOMM_8:
                printf( "RFCOMM_8:" );
                break;
            case NPI_RFCOMM_9:
                printf( "RFCOMM_9:" );
                break;
            default:
                break;
            }
            last_time.tv_sec = current_time.tv_sec;
        }
    }

     printf( "%x", input );

     fflush(stdout);

    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET spp_stream_input(void *HSAECbkParam, const BT_ADDR ba, const char *input, size_t len, const HSAE_PROFILEID ProfileID)
{
    size_t i;

    memset(SPPStubImp::btSppInstance->btmac,0,20);
//
    SPPStubImp::btSppInstance->chartostr(SPPStubImp::btSppInstance->btmac,(unsigned char*)&ba.b);
    switch(ProfileID){
    case NPI_SPP:
       if (len>0)
        {
             SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,0));
             sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
        }

        break;
    case NPI_SPP_IAP:
        if (len>0)
        {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,0));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
        }
        break;
    case NPI_RFCOMM_1:
       if (len>0)
        {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,1));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
              printf( "RFCOMM_1:" );
        }
        break;
    case NPI_RFCOMM_2:
       if (len>0)
       {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,2));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
            printf( "RFCOMM_2:" );
        }
        break;
    case NPI_RFCOMM_3:
       if (len>0)
       {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,1));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
            printf( "RFCOMM_3:" );
        }
        break;
    case NPI_RFCOMM_4:
         if (len>0)
        {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,2));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
            printf( "RFCOMM_4:" );
        }
        break;
    case NPI_RFCOMM_5:
       if (len>0)
       {
            SPPStubImp::btSppInstance->g_sppqueue.push(SPP_DATA((char*)input,(int)len,5));
            sem_post(&SPPStubImp::btSppInstance->spp_wait_sem);
            printf( "RFCOMM_5:" );
       }
        break;
    case NPI_RFCOMM_6:
        printf( "RFCOMM_6:" );
        break;
    case NPI_RFCOMM_7:
        printf( "RFCOMM_7:" );
        break;
    case NPI_RFCOMM_8:
        printf( "RFCOMM_8:" );
        break;
    case NPI_RFCOMM_9:
        printf( "RFCOMM_9:" );
        break;
    default:
        break;
    }


    fflush(stdout);

    return HSAE_CBK_SUCCESS;
}
//void SPPStubImp::str_to_char( const char *addr, unsigned char *b  )
//{
//    int i;
//    char byteInChar[2];

//    for( i = 0; i < BT_ADDR_LEN; i++ )
//    {
//        byteInChar[0] = toupper( addr[0] );
//        byteInChar[1] = toupper( addr[1] );

//        // first 4 bit
//        if( isalpha( byteInChar[0] ) )
//            b[i] = (byteInChar[0] - 55) * 16;
//        else if( isdigit( byteInChar[0] ) )
//            b[i] = (byteInChar[0] - 48) * 16;

//        // last 4 bit
//        if( isalpha( byteInChar[1] ) )
//            b[i] += (byteInChar[1] - 55);
//        else if( isdigit( byteInChar[1] ) )
//            b[i] += (byteInChar[1] - 48);

//        if( addr[2] != '\0' )
//            addr = &addr[3];
//    }
//}

void SPPStubImp::str_to_char( const char *addr, unsigned char *b  )
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

HSAE_CBK_RET hfp_manufacturer_id(void *hsaeCbkParam, const BT_ADDR ba, const char *manufacturerID)
{
    printf("Manufacturer identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5], manufacturerID);

    if(strstr(manufacturerID, "Apple") != NULL){
//        hsaeCmdSetXAPL(hsaeHandle, (BT_ADDR *)&ba, 0x014D, 0xDD64, 0x100, 0);
//        hsaeCmdSetSiriEyeFree(hsaeHandle, (BT_ADDR *)&ba, 1);
        SPPStubImp::btSppInstance->setios(true);

        printf("its a big apple \r\n");

    }
    else
       SPPStubImp::btSppInstance->setios(false);

    return HSAE_CBK_SUCCESS;
}

HSAE_CBK_RET hfp_manufacturer_model_id(void *hsaeCbkParam, const BT_ADDR ba, const char *manufacturerModelID)
{
    printf("Manufacturer model identification of %02X:%02X:%02X:%02X:%02X:%02X: %s\n\n",
            ba.b[0], ba.b[1], ba.b[2], ba.b[3], ba.b[4], ba.b[5], manufacturerModelID);

    return HSAE_CBK_SUCCESS;
}



void on_connected_status_changed(IPODConnectStatus flag)
{
    printf("on_connected_status_changed flag = %d\n", flag);
}
void on_assistive_enable_changed(int flag)
{

}
SPPStubImp::SPPStubImp()
{
    m_benableconnect = false;
    auto_addr ="";
    memset(btmac,0,20);
    autoconnect_device_thread_id =0;
    m_createtimer.SetTimer(1,0,timeout);
    m_mfi =false;
    sem_init(&spp_wait_sem,0,0);

    m_startpos = 0;
    m_ios = false;
    dealdata_id =0;
    mutex_for_data =PTHREAD_MUTEX_INITIALIZER;
    datacond = PTHREAD_COND_INITIALIZER;

    pthread_create(&dealdata_id, NULL, dealdataFunc, (void *)this);
    //register spp
//    hsaeCmdSetCbk_SPPInput( NULL, spp_input);
    hsaeCmdSetCbk_SPPStreamInput(NULL, spp_stream_input);
    hsaeCmdSetCbk_HandsfreeManufacturerID( NULL, hfp_manufacturer_id ) ;
    hsaeCmdSetCbk_HandsfreeManufacturerModelID( NULL, hfp_manufacturer_model_id) ;
}
SPPStubImp::~SPPStubImp()
{
   g_bRun = false;

}

gboolean SPPStubImp::usersenddata(Spp *object,GDBusMethodInvocation *invocation,  GVariant *arg_data,  gint arg_datalen)
{

    printf("SPPStubImp::usersenddata message len: %d \n", arg_datalen);
    GVariantIter *iter = NULL;
    if (arg_datalen > 0)
    {
      char str[10*1024]={0} ;
      g_variant_get(arg_data, "ay", &iter);
      int i =0;
      while (g_variant_iter_loop(iter, "y", str+i))
      {
        printf("%x ", str[i]);
         i++;
      }
      if (arg_datalen > 18)
     {
          int rfcommid =1;
          switch (str[0])
          {
          case 0x01:
          {
              if (DeviceStubImp::getInstance()->m_server[0].status == HSAE_CON_CONNECTION )
                   rfcommid= 1;
              else  if (DeviceStubImp::getInstance()->m_client[0].status == HSAE_CON_CONNECTION )
                   rfcommid= 3;

              break;
          }
          case 0x02:
          {
              if (DeviceStubImp::getInstance()->m_server[1].status == HSAE_CON_CONNECTION )
                   rfcommid= 2;
              else  if (DeviceStubImp::getInstance()->m_client[1].status == HSAE_CON_CONNECTION )
                   rfcommid= 4;

              break;
          }
          case 0x03:
              rfcommid = 3;
              break;
          case 0x04:
              rfcommid = 4;
              break;
          case 0x05:
              rfcommid = 5;
              break;
          }
          unsigned char  mac[6];
          char sourcemac[17];
          memcpy(sourcemac,str+1,17);
          btSppInstance->str_to_char(sourcemac,mac);
          bool ret = btSppInstance->senddata((char*)mac,str+18,arg_datalen-18,rfcommid);
      }

      g_variant_iter_free (iter);
    }
    spp_complete_senddata(object,invocation);
    return TRUE;
}

gboolean SPPStubImp::setparams(Spp *object, GDBusMethodInvocation *invocation, GVariant *arg_data, gint arg_datalen)
{
    printf("[HASE][[BtServer]setbrparams ");
    GVariantIter *iter = NULL;
    char str[1024]={0} ;
    g_variant_get(arg_data, "ay", &iter);
    int i =0;
    if ((arg_datalen > 0)&&(arg_datalen<1024))
    {
        while (g_variant_iter_loop(iter, "y", str+i))
        {
            printf("%x ", str[i]);
            i++;
        }
       g_variant_iter_free (iter);
    }
      if (arg_datalen >16)
      {
            HSAE_UUID_t rfcomm  ;
            memcpy(rfcomm.uuid,str+1,16);
            if (str[0] == 0x01)
            {               
                hsaeCmdSetRFCOMMUsrDefUUIDEx(NULL, NPI_RFCOMM_1, rfcomm, 0x01);

                hsaeCmdSetRFCOMMUsrDefUUIDEx(NULL, NPI_RFCOMM_3, rfcomm, 0x0); //tobeclient
                hsaeCmdRFCOMMAddServer(NULL, NPI_RFCOMM_1);

                DeviceStubImp::getInstance()->m_server[0].chanelid = 1;
                DeviceStubImp::getInstance()->m_server[0].rfcommid = NPI_RFCOMM_1;
                DeviceStubImp:: getInstance()->m_client[0].chanelid = 1;
                DeviceStubImp::getInstance()->m_client[0].rfcommid = NPI_RFCOMM_3;

            }
            else if (str[0] == 0x02)
            {


                hsaeCmdSetRFCOMMUsrDefUUIDEx(NULL, NPI_RFCOMM_2, rfcomm, 0x01);;
                hsaeCmdSetRFCOMMUsrDefUUIDEx(NULL, NPI_RFCOMM_4, rfcomm, 0x0); //tobeclient
                hsaeCmdRFCOMMAddServer(NULL, NPI_RFCOMM_2);

                DeviceStubImp::getInstance()->m_server[1].chanelid = 2;
                DeviceStubImp::getInstance()->m_server[1].rfcommid = NPI_RFCOMM_2;

                DeviceStubImp:: getInstance()->m_client[1].chanelid = 2;
                DeviceStubImp::getInstance()->m_client[1].rfcommid = NPI_RFCOMM_4;


            }
//            else if (str[0] == 0x03)
//            {
//                hsaeCmdSetRFCOMMUsrDefUUID(NULL, NPI_RFCOMM_3, rfcomm);
//                hsaeCmdRFCOMMAddServer(NULL, NPI_RFCOMM_3);
//            }
//            else if (str[0] == 0x04)
//            {
//                hsaeCmdSetRFCOMMUsrDefUUID(NULL, NPI_RFCOMM_4, rfcomm);
//                hsaeCmdRFCOMMAddServer(NULL, NPI_RFCOMM_4);
//            }
//            else if (str[0] == 0x05)
//            {
//                hsaeCmdSetRFCOMMUsrDefUUID(NULL, NPI_RFCOMM_5, rfcomm);
//                hsaeCmdRFCOMMAddServer(NULL, NPI_RFCOMM_5);
//            }
        }
    spp_complete_setparames(object,invocation);
    return TRUE;
}

void SPPStubImp::iapsenddata(int cDataLen, unsigned char *pData)
{
     printf("ios send data =%d,start ,\r\n",cDataLen);
//    for(int i =0;i<cDataLen;i++)
//        printf("0x%x ",pData[i]);
//     printf("ios send datpover \r\n");
    hsaeCmdSPPStreamOutput(NULL, &DeviceStubImp::getInstance()->sppaddr, (char *)pData, cDataLen, NPI_SPP_IAP);
    printf("ios send data =%d,over ,\r\n",cDataLen);

}


std::shared_ptr< SPPStubImp > SPPStubImp::getInstance()
{
    if( btSppInstance == NULL )
    {
        btSppInstance = std::make_shared< SPPStubImp >();
    }
    return btSppInstance;
}
int SPPStubImp::IsFileExist(const char* path)
{
    return !access(path, F_OK);
}

void *SPPStubImp::dealdataFunc(void *args)
{
    printf("[HASE][BT]SPPStubImp::dealdataFunc \r\n");
    SPPStubImp *g_spp = (SPPStubImp *)args;
    char  tempdata[1024] ={0};
    int needsend =0;
    int leavedatalen=0;

    while (1)
    {
        sem_wait( &(g_spp->spp_wait_sem));
//        SPP_DATA *data = &SPPStubImp::btSppInstance->g_sppqueue.front();

        for (int i =0;i<SPPStubImp::btSppInstance->g_sppqueue.size();i++)
        {
            SPP_DATA *data = &SPPStubImp::btSppInstance->g_sppqueue.front();
#if 0
            if (g_spp->m_ios)
            {
//                for (int j = 0;  j < data->len;  j++) {
//                    printf("0X%02X, ", data->fisrtdata[j]);
//                }

//                printf("\n");

                if ((data->fisrtdata[0]==0xff)&&(data->fisrtdata[1] == 0x55))
                {
                    ipod_BT_spp_data_pro( data->len,(unsigned char *)data->fisrtdata);
                }
                else  if ((data->fisrtdata[0]==0xff)&&(data->fisrtdata[1] == 0x5A))
                {
                      int tmplen = data->fisrtdata[2]*256+data->fisrtdata[3];
                       printf("tmplen =%d ,data->len =%d \r\n",tmplen,data->len);
                      if ((tmplen <= data->len )&&(data->len<=1024))
                      {

                          ipod_BT_spp_data_pro( data->len,(unsigned char *)data->fisrtdata);

                      }
                      else  if (tmplen < data->len )
                      {
                          ipod_BT_spp_data_pro( tmplen,(unsigned char *)data->fisrtdata);
                          needsend = tmplen;
                          if( (data->fisrtdata[needsend]==0xff)&&(data->fisrtdata[needsend+1] == 0x5A))
                          {
                              needsend =  data->fisrtdata[needsend+2]*256+data->fisrtdata[needsend+3];
                              leavedatalen =needsend - (data->len-tmplen);
                              memcpy(tempdata,data->fisrtdata+tmplen,data->len-tmplen);
                          }
                      }
                      else if (tmplen > data->len )
                      {
                            needsend = tmplen;
                            leavedatalen = needsend - data->len;
                            memset(tempdata,0,1024);
                            memcpy(tempdata,data->fisrtdata,data->len);
                      }


                }
                else {
                    if (leavedatalen==data->len)
                    {
                        memcpy(tempdata+needsend-leavedatalen,data->fisrtdata,data->len);
                        ipod_BT_spp_data_pro( needsend,(unsigned char *)tempdata);
                        needsend =0;
                        leavedatalen = 0;
                        memset(tempdata,0,1024);
                    }
                    else if (leavedatalen < data->len)
                    {
                        printf("needsend1= %d,leavedatalen =%d\r\n",needsend,leavedatalen);
                        memcpy(tempdata+needsend-leavedatalen,data->fisrtdata,leavedatalen);
                        ipod_BT_spp_data_pro( needsend,(unsigned char *)tempdata);

                        //787  needsend = 747
                        printf("data->len =%d,start pos= %d\r\n",data->len,needsend);
                        memset(tempdata,0,1024);
                        int firstsendnum =  leavedatalen;
                        if( (data->fisrtdata[leavedatalen]==0xff)&&(data->fisrtdata[leavedatalen+1] == 0x5A))
                        {
                            needsend =  data->fisrtdata[leavedatalen+2]*256+data->fisrtdata[leavedatalen+3];
                            leavedatalen =needsend - (data->len-firstsendnum);
                            printf("needsend2 = %d,leavedatalen =%d\r\n",needsend,leavedatalen);
                            memcpy(tempdata,data->fisrtdata+firstsendnum,data->len-firstsendnum);
                            if (leavedatalen ==0)
                            {
                               ipod_BT_spp_data_pro( needsend,(unsigned char *)tempdata);
                            }
                        }
                    }
                }

            }
            else
            {
                ipod_ea_data_recv_process(EA_PROTOCOL_INDEX_RESVER,data->len,data->fisrtdata);
            }
#endif
            ipod_ea_data_recv_process(data->profile,data->len,data->fisrtdata);
            if (data->fisrtdata!= NULL)
                delete[]data->fisrtdata;
            SPPStubImp::btSppInstance->g_sppqueue.pop();
            printf("SPPStubImp::btSppInstance->g_sppqueue.size() =%d\r\n",SPPStubImp::btSppInstance->g_sppqueue.size());

        }
    }

}


bool SPPStubImp::isIos()
{
    return m_ios;
}

bool SPPStubImp::setios(bool state)
{
    m_ios = state;
    return true;
}

bool SPPStubImp::senddata( char *addr, char *b, int len,int rfcommid)
{
    printf("senddata rfcommid =%d \r\n",rfcommid);
    bool bresult= false;
    BT_ADDR desaddr;
    if (addr != NULL)
       memcpy(desaddr.b,addr,6);
   else
       memcpy(desaddr.b,DeviceStubImp::getInstance()->sppaddr.b,6);
    HSAE_CMD_RET ret ;
    switch (rfcommid) {
    case 1:
        hsaeCmdSPPStreamOutput(NULL, &desaddr, b, len, NPI_RFCOMM_1);
        break;
    case 2:
        hsaeCmdSPPStreamOutput(NULL, &desaddr, b, len, NPI_RFCOMM_2);
        break;
    case 3:
        hsaeCmdSPPStreamOutput(NULL, &desaddr, b, len, NPI_RFCOMM_3);
        break;
    case 4:
        hsaeCmdSPPStreamOutput(NULL, &desaddr, b, len, NPI_RFCOMM_4);
        break;
    case 5:
        hsaeCmdSPPStreamOutput(NULL, &desaddr, b, len, NPI_RFCOMM_5);
        break;
    }

    if (ret <= HSAE_CMD_SUCCESS_LAST )
             bresult = true;
    return bresult;
}

bool SPPStubImp::startauthentication()
{

}
void SPPStubImp::on_bus_acquired (GDBusConnection *connection,
                                        const gchar     *name,
                                        gpointer         user_data)
{
    GError **error;

    //创建一个skeleton对象,以注册输出对象
    m_interface = spp_skeleton_new();

    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect(m_interface,
                     "handle-senddata",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(usersenddata),      //具体的方法处理函数, 注意增加输入参数
                     user_data);
    //利用g_signal_connect连接回调函数进行方法处理
    g_signal_connect(m_interface,
                     "handle-setparames",           //signal_name, 在.c文件中的_default_init查找
                     G_CALLBACK(setparams),      //具体的方法处理函数, 注意增加输入参数
                     user_data);

//    //输出interface到总线上
    if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (m_interface),
                                           connection,
                                           "/com/hsaeyz/bluetooth/spp", //object_path
                                           error))
    {

    }

}

bool SPPStubImp::createSession()
{
    printf("++++ SPPStubImp::createSession() =%d++++\r\n",m_ios);
    if (m_ios)
    {
        p_observer = (PIPODObserver) malloc(sizeof(IPODObserver));
        p_observer->on_connected_status_changed = on_connected_status_changed;
        p_observer->on_assistive_enable_changed = on_assistive_enable_changed;
        i_register_event_observer(p_observer);

        ipodRegistBtSppSendData(iapsenddata);
        ipod_bt_spp_state_change(ipod_SppState_Connected);
//        ipod_register_ea_recived_data_proc(ipod_ea_data_recv_process);
        m_mfi =true;
    //    int ret = i_create_session(&handle);
        m_createtimer.StartTimer();
    }
   else
   {
//       unsigned long test = GetTickCountTest()-testcount;
//       printf("spp is connect =%d\r\n",test);
       spp_emit_channel_changed(m_interface,true);
       spp_set_channelstate(m_interface,true);
   }

    // write iap success
     printf("----SPPStubImp::createSession() ------\r\n");
}

bool SPPStubImp::closeSession()
{
    printf("++++SPPStubImp::closeSession()+++++%d\r\n",m_ios );
    if (m_ios)
    {
        if (m_mfi )
       {
            ipodRegistBtSppSendData(NULL);
            ipod_bt_spp_state_change(ipod_SppState_ReadyNoInitialized);
            i_unregister_event_observer();
            free(p_observer);
            int ret =i_close_session(&handle);
            printf("----SPPStubImp::closeSession() =%d------\r\n",ret);
        }
        m_mfi =false;
        m_ios =false;
    }
     spp_emit_channel_changed(m_interface,false);
     spp_set_channelstate(m_interface,false);

     // close iap successdbus-send --system --print-reply --type=method_call --dest=com.hsaeyz.bluetooth  /com/hsaeyz/bluetooth/media com.hsaeyz.bluetooth.media.nextSong
}

void SPPStubImp::timeout()
{
    int ret = i_create_session(&handle);

    if (ret == 0)
    {
        //launch app
//       SPPStubImp::btSppInstance-> m_mfi =true;
       int len =sizeof(IAP_EA_PROTOCOL_TOKEN_STRING_HSAE);
       char *bufname =new char[len];
       printf("----SPPStubImp::len() =%d------\r\n",len);
       memcpy(bufname,IAP_EA_PROTOCOL_TOKEN_STRING_HSAE,len);
       ret = ipod_launcher_appliction(handle,len,bufname);//com.example.apple-samplecode.EADemo.tanzhen
       spp_emit_channel_changed(m_interface,true);
       spp_set_channelstate(m_interface,true);
    }
    else
        SPPStubImp::btSppInstance->m_mfi =false;

    printf("----SPPStubImp::timeout()2 =%d------\r\n",ret);
    SPPStubImp::btSppInstance->m_createtimer.StopTimer();
}

bool SPPStubImp::setsppsate(bool sate)
{
    spp_emit_channel_changed(m_interface,sate);
    spp_set_channelstate(m_interface,sate);
}

void SPPStubImp::chartostr(char *strmac, unsigned char *mac)
{
   sprintf( strmac,"%02X:%02X:%02X:%02X:%02X:%02X", mac[5],mac[4],  mac[3], mac[2],mac[1], mac[0]);
}
/*gboolean SPPStubImp::setPower(Set *interface, GDBusMethodInvocation *invocation, gboolean power)
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
gboolean SPPStubImp::setScanEnable(Set *interface, GDBusMethodInvocation *invocation, gboolean scan)
{
    hsaeCmdSetScanEnable(NULL, scan);
    set_complete_set_scan_enable(interface, invocation);
    return TRUE;
}

gboolean SPPStubImp::setLocalDeviceName(Set *interface, GDBusMethodInvocation *invocation, const gchar *name)
{
    hsaeCmdSetLocalDeviceName(NULL, name);
    LOG(BT_LOG_SET,"name =%s", name);
    set_complete_set_local_device_name(interface, invocation);
    return TRUE;
}
gboolean SPPStubImp::setScanTime(Set *interface,GDBusMethodInvocation *invocation,gint arg_time)
{
     hsaeCmdSetScanEnableTime(NULL, (unsigned int) arg_time);
     set_complete_set_scan_time (interface,invocation);
     return TRUE;
}

gboolean SPPStubImp::enableautoconncet(Set *interface, GDBusMethodInvocation *invocation, gboolean state)
{
     LOG(BT_LOG_SET,"state =%d", state);
     std::shared_ptr< SPPStubImp > g_set= SPPStubImp::getInstance();

     if (state)
     {
        g_set->startAutoConnectThread();
     }
     else
     {
        g_set->closeAutoConnectThread();
     }
     set_complete_set_auto_answer(interface,invocation);
      return TRUE;
}

//gboolean SPPStubImp::setautoconect(Set *interface, GDBusMethodInvocation *invocation, gboolean state)
//{
//     LOG(BT_LOG_SET,"state =%d", state);
//     set_complete_set_auto_connect(interface,invocation);
//     return TRUE;
//}



/**
 * powerState value 0:off 1:on 2:unknown
 */











void *SPPStubImp::autoConnectDeviceFunc(void *args)
{
    std::shared_ptr< DeviceStubImp > g_device = DeviceStubImp::getInstance();

    SPPStubImp*pset =(SPPStubImp*)args;
    int count =0;

    while(pset->g_bRun)
    {
          usleep(3000*1000);
          LOG(BT_LOG_SET,"autoConnectDeviceFunc =%s", pset->auto_addr.data());
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
                     ret = hsaeCmdConnectRemoteDevice( NULL, &m_btAddress,HSAE_PROFILEID_e::NPI_ALL_AUDIO );
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




