/*************************************************

Copyright: HSAE Technology Co., Ltd.  All rights reserved.

Author:LIHUIMIN

Date:2019-11-26

Description:BT BLE&BR TEST

**************************************************/

#include <iostream>
#include"driver_adapt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;
extern  FcDriverAdaptOps  g_fcAdaptBleOps;
extern  FcDriverAdaptOps  g_fcDriverBrOps;

extern  FcDriverAdaptOps  g_fcDriverWifiOps;

ChannelCallback g_callback;

struct wifiap{
    char ssid[20];
    char psk[20];
    int freqnum;                
};
 string testuuid ="0000FE35-0000-1000-8000-00805F9B34FB";
//char rfcommuuid1[128] =  "2f7a01f8-2fb8-4f5c-a9e3-f3d44117094e";
char rfcommuuid1[128] = "0000FE35-0000-1000-8000-00805F9B34FB";
char rfcommuuid2[128] ="0000FE36-0000-1000-8000-00805F9B34FB";
string remoteMac ="08:07:E0:54:2C:0C";
void ShowMenu()
{
    printf("\n");
    printf("**************************************************\n");
    printf("*         HSAE  Testing Menu                     *\n");
    printf("* <0> set remotemac  default is 08:07:E0:54:2C:0C*\n");
    printf("* <1> start advertise                            *\n");
    printf("* <2> stop advertise                             *\n");
    printf("* <3> connect    rfcomm1                         *\n");
    printf("* <4> disconnet  rfcomm1                         *\n");
    printf("* <5> senddatatest                               *\n");
    printf("* <6> getbrmac                                   *\n");
    printf("* <7> start hostAp                               *\n");
    printf("* <8> getWifiCapt                                *\n");
    printf("* <9> WifiConnect                                *\n");
    printf("* <a> connect rfcomm2                            *\n");
    printf("* <b> disconnet   rfcomm2                        *\n");
    printf("* <c> senddatatest  rfcomm2                      *\n");
    printf("**************************************************\n");
}
void ServerExecCmd(unsigned char choice)
{
    switch (choice) {
    case '0':
        {


            break;
        }
        case '1':
        {
           //maxdatalen is 29
           BleParams  bleparam;

           bleparam.maxInterval =48;
           bleparam.minInterval =48;
           bleparam.timeout =0;
           //SetParam SET_PARAMS ??what mean?

           char buf[10];
           buf[0] = 0x01;
           buf[1] = 0x02;
           buf[2] = 0x03;
           buf[3] = 0x04;
           buf[4] = 0xa5;
           buf[5] = 0xa6;
           buf[6] = 0xa7;
           buf[7] = 0xa8;
           buf[8] = 0x0a;
           bleparam.nearbyAdvData = new char[9] ;
           memcpy( bleparam.nearbyAdvData,buf,9);
           bleparam.advDataLen =9;
           g_fcAdaptBleOps.SetParam(SET_PARAMS,&bleparam);
           int ret = g_fcAdaptBleOps.StartAdv(NULL);
           printf("StartAdv ret =%d\r\n",ret);
           delete []bleparam.nearbyAdvData;
           break;
        }
        case '2':
        {
            int ret =g_fcAdaptBleOps.StopAdv(NULL);
            printf("StopAdv ret =%d\r\n",ret);
            break;
        }
        case '3':
        {
            BrChannelData param ;
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
            string remoteMac ="CE:63:72:ED:E5:B0";
            memset(param.remoteMac,0,20);
            memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
            param.channelId = 2;
            int ret = g_fcDriverBrOps.Connect(&param);
            printf("Connect ret =%d\r\n",ret);
            break;
        }
        case '4':
        {    
            BrChannelData param ;
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
            string remoteMac ="CE:63:72:ED:E5:B0";
            memset(param.remoteMac,0,20);
            memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
            param.channelId = 2;
            int ret = g_fcDriverBrOps.Disconnect(&param);
            printf("Disconnect ret =%d\r\n",ret);
            break;

        }
        case '5':
        {
           // char max len is 1024
            unsigned char  buf[10];
            buf[0] = 0x01;
            buf[1] = 0x02;
            buf[2] = 0x03;
            buf[3] = 0x04;
            buf[4] = 0x05;
            buf[5] = 0x06;
            buf[6] = 0x07;
            buf[7] = 0x08;
            buf[8] = 0x09;
            buf[9] = 0x0a;
            BrChannelData param ;
            memset(&param,0,sizeof(BrChannelData));
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
            string remoteMac ="CE:63:72:ED:E5:B0";
            memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
            param.channelId =2;
            int ret = g_fcDriverBrOps.SendData(buf,10,&param);
            printf("SendData ret =%d\r\n",ret);
            break;
        }

        case '6':
        {
            BrChannelData param ;
            memset(&param,0,sizeof(BrChannelData));
            int ret = g_fcDriverBrOps.GetParam(GET_DEV_MAC,&param);
            printf("getbrparam mac  is %s ,ret = %d \r\n",param.localMac,ret);
            DeviceModelId deviceid;
            ret = g_fcAdaptBleOps.GetParam(GET_MODEL_ID,&deviceid);
            printf("getble param ret = %d  ",ret);
            for (int i =0;i<3;i++)
                printf("%0x ",deviceid.modelId[i]);
            printf("\r\n ");

            break;
        }
        case '7':
        {
            WifiChannelData ap;
            char *ip = "192.168.53.1";

            memset(&ap,0,sizeof(ap));
            printf("\r\n start create ap");
            ap.channel=1;
            ap.port = 5432;
            memcpy(ap.localIp,ip,strlen(ip)+1);

            g_fcDriverWifiOps.CreateWifiChannel(NULL,&ap);
            
            printf("\r\n ssid:%s,psk:%s ",ap.ssid,ap.key);
            break;
        }
        case '8':
        {   
            int is_5g = 0;
            g_fcDriverWifiOps.GetParam(GET_WIFI_ABILITY,&is_5g);
            
            printf("\r\n wifi ability is %s",is_5g > 0? "5G":"2.4G");
            break;
        }
        case '9':
        {
            WifiChannelData para;
            /* socket client connect &ip$port */
            char *ip = "192.168.5.10";
            
            memcpy(para.localIp,ip,strlen(ip)+1);

            para.port = 8003;
            g_fcDriverWifiOps.Connect(&para);
            break;
        }
    case 'a':
    {
        BrChannelData param ;
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
    //    string remoteMac ="CE:63:72:ED:E5:B0";
        memset(param.remoteMac,0,20);
        memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
        param.channelId = 2;
        int ret = g_fcDriverBrOps.Connect(&param);
        printf("Connect ret =%d\r\n",ret);
        break;
    }
    case 'b':
    {
        BrChannelData param ;
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
      //  string remoteMac ="CE:63:72:ED:E5:B0";
        memset(param.remoteMac,0,20);
        memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
        param.channelId = 2;
        int ret = g_fcDriverBrOps.Disconnect(&param);
        printf("Disconnect ret =%d\r\n",ret);
        break;

    }
    case 'c':
    {
       // char max len is 1024
        unsigned char  buf[10];
        buf[0] = 0x01;
        buf[1] = 0x02;
        buf[2] = 0x03;
        buf[3] = 0x04;
        buf[4] = 0x05;
        buf[5] = 0x06;
        buf[6] = 0x07;
        buf[7] = 0x08;
        buf[8] = 0x09;
        buf[9] = 0x0a;
        BrChannelData param ;
        memset(&param,0,sizeof(BrChannelData));
//            memcpy(param.deviceId,testuuid.data(),testuuid.length());
        //string remoteMac ="CE:63:72:ED:E5:B0";
        memcpy(param.remoteMac,remoteMac.data(),remoteMac.length());
        param.channelId =2;
        int ret = g_fcDriverBrOps.SendData(buf,10,&param);
        printf("SendData ret =%d\r\n",ret);
        break;
    }
        default:
            printf("*Invalid command!,please enter 1~6\n");
            break;
    }
}

void ChannelStateChange(ChannType channelType, EventType evt, void *data, int dataLen)
{
  if (channelType ==CHANN_BREDR )
  {
       BrChannelData brdata = *(BrChannelData*)data;
       printf("[HSAE]:remoteMac %s state is %d,channelId is %d\r\n",brdata.remoteMac,evt,brdata.channelId);
  }
}
void recvData(ChannType channelType, void *channelParam, unsigned char* data, int dataLen)
{
    if (channelType ==CHANN_BREDR )
    {
        BrChannelData brdata = *(BrChannelData*)channelParam;
        printf("[HSAE]:recv %d from mac %s,chanelid%d \r\n",dataLen,brdata.remoteMac,brdata.channelId);
         for (int i = 0;i<dataLen;i++)
             printf ("recv data %x \r\n",data[i]);
    }
}
void WifiChannelStateChange(ChannType channelType, EventType evt, void *data, int dataLen)
{
	if (channelType == CHANN_P2P )
	{
	//BrChannelData brdata = *(BrChannelData*)data;
	switch (evt)
	{       
		case EVENT_CONNECT/* constant-expression */:
		    /* code */
		    printf("\r\n WifiChannelStateChange EVENT_CONNECT  mac:%s",data);

		    break;
		case EVENT_DISCONNECT:
			printf("\r\n WifiChannelStateChange EVENT_DISCONNECT mac:%s",data);
			break;
		case EVENT_CREAT_AP_SUCCESS:
            printf("\r\n WifiChannelStateChange EVENT_CREAT_AP_SUCCESS %s",data);
	
			break;	
		case EVENT_CONFIG_IP_SUCCESS:
             printf("\r\n WifiChannelStateChange EVENT_CONFIG_IP_SUCCESS ip:%s",data);
			break;
		default:
		    break;
	}
	}
}
void wifirecvData(ChannType channelType, void *channelParam, unsigned char* data, int dataLen)
{
    if (channelType ==CHANN_P2P )
    {
          printf ("recv data CHANN_P2P :%s datalen:%d \r\n",data,dataLen);
    }
}
int main(int argc, char *argv[])
{
//init ble
//    const  char buf[20] ="fdccf";
    g_fcAdaptBleOps.Init(NULL,NULL);
//init br


    BrChannelData brparam2;
    memset(&brparam2,0,sizeof(BrChannelData));
    brparam2.channelId = 1;
    memcpy(brparam2.serviceUuid,rfcommuuid2,sizeof(rfcommuuid2));

    BrChannelData brparam1;
    memset(&brparam1,0,sizeof(BrChannelData));
    brparam1.channelId = 2;
    memcpy(brparam1.serviceUuid,rfcommuuid1,sizeof(rfcommuuid1));
    g_fcDriverBrOps.SetParam(SET_BR_UUID,&brparam1);

    g_fcDriverBrOps.SetParam(SET_BR_UUID,&brparam2);
    g_callback.ChannelStateChange = ChannelStateChange;
    g_callback.ReceiveData = recvData;
    g_fcDriverBrOps.RegisterChannelCallback(&g_callback);
 //showmenu

//initwifi
    ChannelCallback Wificallback;

    Wificallback.ChannelStateChange = WifiChannelStateChange;
    Wificallback.ReceiveData = wifirecvData;
    g_fcDriverWifiOps.RegisterChannelCallback(&Wificallback);

    char input[10];
    memset(input, 0, 10);
    ShowMenu();
    do
    {
        fflush( stdin );
        scanf( "%21s", input );
        fflush( stdin );
        printf("%s\r\n",input);
        ServerExecCmd((unsigned char)input[0]);
        usleep(100*1000);

    }while(1);
    return 0;
}
