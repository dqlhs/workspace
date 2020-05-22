
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include<driver_adapt.h>

 #define true 1
 #define false 0

static  is_print_menu = true;

extern const struct FcDriverAdaptOps  g_fcDriverWifiOps;

typedef enum {
    HOSTAP_CRATE                    = '1',
    HOSTAP_CONFIG_IP                = '2',
    HOSTAP_SERVER_SET               = '3',
    HOSTAP_SOCKET_DISCONNECT        = '4',
    HOSTAP_INIT                     = '5',
    HOSTAP_DEINIT                   = '6',
    EXIT_CMD                    = 'x',
} CommandType;


void PrintMenu() {
    printf("\n\n===========WIFI SERVICE TEST MENU ===================\n");
    printf("   HostAP:\n");
    printf("   %c. HOSTAP_CRATE\n", HOSTAP_CRATE);
    printf("   %c. HOSTAP_CONFIG_IP\n", HOSTAP_CONFIG_IP);
    printf("   %c. HOSTAP_SERVER_SET\n", HOSTAP_SERVER_SET);
    printf("   %c. HOSTAP_SOCKET_DISCONNECT\n", HOSTAP_SOCKET_DISCONNECT);
    printf("   %c. HOSTAP_INIT\n", HOSTAP_INIT);
    printf("   %c. HOSTAP_DEINIT\n", HOSTAP_DEINIT);
    printf("   %c. EXIT_CMD\n", EXIT_CMD);
    printf("\n   Choice: ");
}

char GetCommand(int is_print_menu)
{
    if (is_print_menu) {
        PrintMenu();
        is_print_menu = false;
    }
    return (CommandType)getchar();
}

void WifiChannelStateChange1(ChannelType channelType, EventType evt, void *data, int dataLen)
{
	if (channelType == CHANNEL_P2P )
	{
        // WifiChannelData *wifidata = (WifiChannelData *)data;
        switch (evt)
        {       
            case EVENT_CONNECT/* constant-expression */:
                /* code */
                printf("\r\n WifiChannelStateChange EVENT_CONNECT mac\r\n");
                break;
            case EVENT_DISCONNECT:
                printf("\r\n WifiChannelStateChange EVENT_DISCONNECT\r\n");
                break;
            case EVENT_CREATE_WIFI_SUCCESS:
                printf("\r\n WifiChannelStateChange EVENT_CREAT_AP_SUCCESS \r\n");
                break;	
            case EVENT_CONFIG_IP_SUCCESS:
                printf("\r\n WifiChannelStateChange EVENT_CONFIG_IP_SUCCESS \r\n");
                break;
            default:
                break;
        }
	}
}

// void wifirecvData(ChannelType channelType, void *channelParam, unsigned char* data, int dataLen)
// {
//     if (channelType ==CHANNEL_P2P )
//     {

//           printf ("recv data CHANN_P2P :%s datalen:%d \r\n",data,dataLen);
//     }
// }

int main(int argc, char *argv[])
{

    printf ("recv data CHANN_P2P ");
    printf ("recv data CHANN_P2P ");
    printf ("recv data CHANN_P2P ");
    printf ("recv data CHANN_P2P ");

    printf ("recv data CHANN_P2P ");
    
    ChannelCallback Wificallback;


    Wificallback.ChannelStateChange = WifiChannelStateChange1;
    // Wificallback.ReceiveData = wifirecvData;
    g_fcDriverWifiOps.RegisterChannelCallback(&Wificallback);

    int exit_test = false;

    while(!exit_test) {

        CommandType type = GetCommand(is_print_menu);

        switch(type) {
            
            case HOSTAP_CRATE:
            {
                // WifiParams ap;
                
                // memset(&ap,0,sizeof(ap));
                printf("\r\n start create ap");
                int frequency=5180;
                
                g_fcDriverWifiOps.CreateWifiChannel(NULL,&frequency);
                
                break;
            }
            case HOSTAP_CONFIG_IP:
            {   
                char *ip = "192.168.43.1";
                WifiSetIpParam ipparam;
                
                memset(&ipparam,0,sizeof(ipparam));

                strcpy((char *)&ipparam.ipAddr,ip);

                g_fcDriverWifiOps.SetParam(SET_WIFI_LOCAL_IP,ip);
                
                break;
            }
            
            case HOSTAP_INIT:
            {
                g_fcDriverWifiOps.Init(NULL,"wifi");
                break;
            }
            case HOSTAP_DEINIT:
            {
                g_fcDriverWifiOps.Deinit(NULL);
                break;
            }
            case EXIT_CMD:
            {
                exit_test = true;

                break;
            }
            default:
                break;

            // getchar(); // read "enter"
        }
    }
    return 0;
}
