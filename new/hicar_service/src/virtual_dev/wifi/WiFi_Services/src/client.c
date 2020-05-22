#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <glib.h>
#include <gio/gio.h>
#include <pthread.h>

#include <WifiService.h>
#include <WifiClient.h>



static bool is_print_menu = true;
static pthread_t tid;
static GMainLoop *loop;

typedef enum {
    WIFI_ENABLE_CMD                   = '1',
    WIFI_DISABLE_CMD                  = '2',
    START_SCAN_CMD                    = '3',
    GET_SCAN_RESULTS_CMD              = '4',
    CONNECT_CMD                       = '5',
    DISCONNECT_CMD                    = '6',
    PERIODIC_SCAN_CMD                 = '7',
    IS_WIFI_ENABLED_CMD               = '8',
    GET_CONNECTION_INFO_CMD           = '9',
    GET_SAVED_NETWORKS_CMD            = 'a',
    SELECT_NETWORK_CMD                = 'b',
    GET_STATE_CMD                     = 'c',
    GET_PERIODIC_INTERVAL_CMD         = 'd',
    SET_ETH0_IP                       = 's',
    WIFI_START_HOSTAP				  = 'm',
    WIFI_STOP_HOSTAP				  = 'n',
    WIFI_GET_CONNECT_INFO		      = 'x',
    WIFI_GET_HOSTAP_INFO		      = 'y',
    WIFI_SENDTEST       		      = 'z',
    EXIT_CMD                          = 'e',
    NEXT_CMD                          = '\n',
    INVALID_CMD                       = '0'
} CommandType;

enum WifiState {
	WIFI_STATE_NONE = 1, 
	WIFI_STATE_ENABLING,
	WIFI_STATE_ENABLED,
	WIFI_STATE_CONNECTING,
	WIFI_STATE_CONNECTED,
	WIFI_STATE_DISCONNECTING,
	WIFI_STATE_DISCONNECTED,
	WIFI_STATE_DISABLING,
	WIFI_STATE_DISABLED,
	WIFI_STATE_SCANNING,
	WIFI_STATE_SCAN_DONE,
	WIFI_STATE_FAILED,
	HOSTAP_STATE_DISABLE,
	HOSTAP_STATE_ENABLED,
	HOSTAP_STATE_IP,
	HOSTAP_STATE_CLIENT_CONNECT,
	HOSTAP_STATE_CLIENT_DISCONNECT,
};
static char *state_names[] = { 
"WIFI_STATE_NONE",
"WIFI_STATE_ENABLING",
"WIFI_STATE_ENABLED",
"WIFI_STATE_CONNECTING",
"WIFI_STATE_CONNECTED",
"WIFI_STATE_DISCONNECTING",
"WIFI_STATE_DISCONNECTED",
"WIFI_STATE_DISABLING",
"WIFI_STATE_DISABLED",
"WIFI_STATE_SCANNING",
"WIFI_STATE_SCAN_DONE",
"WIFI_STATE_FAILED",
"HOSTAP_STATE_DISABLE",
"HOSTAP_STATE_ENABLED",
"HOSTAP_STATE_IP",
"HOSTAP_STATE_CLIENT_CONNECT",
"HOSTAP_STATE_CLIENT_DISCONNECT",
"WIFI_GET_HOSTAP_INFO",
"WIFI_SENDTEST"
};
void PrintMenu() {

    printf("\n\n===========WIFI SERVICE TEST MENU ===================\n");

    printf(" \nWifi Service Test Application commands \n");
    printf(" -----------------------------\n");
    printf("   %c. Enable\n", WIFI_ENABLE_CMD);
    printf("   %c. Disable\n", WIFI_DISABLE_CMD);
    printf("   %c. StartScan\n", START_SCAN_CMD);
    printf("   %c. GetScanResults\n", GET_SCAN_RESULTS_CMD);
    printf("   %c. Connect\n", CONNECT_CMD);
    printf("   %c. Disconnect\n", DISCONNECT_CMD);
    printf("   %c. SetPeriodicScanInterval\n", PERIODIC_SCAN_CMD);
    printf("   %c. IsWifiEnabled\n", IS_WIFI_ENABLED_CMD);
    printf("   %c. GetConnectionInfo\n", GET_CONNECTION_INFO_CMD);
    printf("   %c. set eth0 ip address\n", SET_ETH0_IP);
    printf("   %c. GetSavedNetworks\n", GET_SAVED_NETWORKS_CMD);
    printf("   %c. SelectNetwork\n", SELECT_NETWORK_CMD);
    printf("   %c. GetState\n", GET_STATE_CMD);
    printf("   %c. GetPeriodicScanInterval\n", GET_PERIODIC_INTERVAL_CMD);
    printf("   HostAP:\n");
    printf("   %c. StartHostAp\n", WIFI_START_HOSTAP);
    printf("   %c. StopHostAp\n", WIFI_STOP_HOSTAP);
    printf("   %c. Exit\n", EXIT_CMD);
    printf("\n   Choice: ");
}

char GetCommand(bool is_print_menu)
{
    if (is_print_menu) {
        PrintMenu();
        is_print_menu = false;
    }
    return (CommandType)getchar();
}

static void on_wifi_state_changed(WifiService *proxy, gint currentState,
    gint previousState)
{
    printf("[STATE CHANGED]currentState = %s, previousState = %s\n", state_names[currentState], state_names[previousState]);
}

static void on_wifi_state_changed_hostap(WifiService *proxy, GVariant *currentState,gint previousState)
{
    gint curstate,prestate;
    gchar *staMac;
    
    printf("on_wifi_state_changed_hostap,%s",state_names[previousState]);

    g_variant_get(currentState, "(is)", &curstate, &staMac);
    
    printf("[STATE CHANGED] mac:%s currentState = %s, previousState = %s\n",staMac,state_names[curstate], state_names[previousState]);

    switch (curstate)
    {
    case HOSTAP_STATE_CLIENT_CONNECT/* constant-expression */:
        /* code */
        printf("\r\n %s CONNECT",staMac);

        break;
    case HOSTAP_STATE_CLIENT_DISCONNECT:
        printf("\r\n %s DISCONNECT",staMac);
        break;
    default:
        break;
    }
}

static void on_wifi_signal_level_changed(WifiService *proxy, gint currentLevel,
    gint previousLevel)
{
    printf("[SIGNAL LEVEL CHANGED]currentLevel = %d, previousLevel = %d\n", currentLevel, previousLevel);
}

// extern  int client_server_main(void);

static void init_client_server(void){

    printf("\r\n init_client_server\r\n");
	// (void)client_server_main();
}

static void *thread_main(void *arg)
{
    /* init server */

    init_client_server();

    g_signal_connect((WifiService *)arg, "wifi--state--changed--host",
        G_CALLBACK(on_wifi_state_changed), NULL);
    
    g_signal_connect((WifiService *)arg, "wifi--state--changed--hoatap",G_CALLBACK(on_wifi_state_changed_hostap), NULL);

    g_signal_connect((WifiService *)arg, "wifi--signal--level--changed",
        G_CALLBACK(on_wifi_signal_level_changed), NULL);

    //InitDBusCommunicationServer();
    
    loop = g_main_loop_new(NULL, FALSE);

    g_main_loop_run(loop);

    return NULL;
}

#define CONF_PATH  "/etc/hostap.conf"
int save_wifiinfo(char *ssid,char *key)
{
    FILE *fp;
    int i,j,ssid_length,key_length,line=0;
    char *str;
    

    bool is_ssid_ok = false;
    bool is_psk_ok = false;
    
    /*open hostap.conf*/
    fp = fopen(CONF_PATH,"a+");
    if(fp == NULL){
        printf("文件打开失败！\n");
        return -1;
    }
    printf("ssid = %s,key = %s",ssid,key);

    ssid_length =strlen(ssid);
    key_length = strlen(key);

    
    while(fgets(str,line,fp) != NULL)
        line++;

    printf("\r\n line:%d",line);

    while (1)
    {
        /* code */
        for (i = 0; i < ssid_length; i++)
        {
            /* code */
        }

        for ( j = 0; j < key_length; j++)
        {
            /* code */
        }

        if((i == ssid_length) && (j == key_length))
        {
            printf("\r\n save ssid/key always ok");
            break;
        }
    }

    return 0;
}



int main(int argc, char *argv[])
{
    WifiService *proxy;

    proxy = wifi_service_proxy_new_for_bus_sync(
            G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE,
            "com.hsaeyz.sal.WifiService", "/com/hsaeyz/sal/WifiService",
            NULL, NULL);

    if (!proxy) {
        printf("wifi_service_proxy_new_for_bus_sync failed\n");
        return 1;
    }
    if(pthread_create(&tid, NULL, thread_main, proxy))
    {
        printf("\r\n pthread_create error\r\n");
        return -1;
    }
    printf("\r\n pthread_create \r\n");

    int exit_test = false;

    while(!exit_test) {

        CommandType type = GetCommand(is_print_menu);
        switch(type) {
            case WIFI_ENABLE_CMD:
                {
                    GVariant *args;
                    args = g_variant_new("(b)", TRUE);
                    wifi_service_call_set_wifi_enabled_sync(proxy, args, NULL, NULL);
                }
                break;
            case WIFI_DISABLE_CMD:
                {
                    GVariant *args;
                    args = g_variant_new("(b)", FALSE);
                    wifi_service_call_set_wifi_enabled_sync(proxy, args, NULL, NULL);
                }
                break;
            case START_SCAN_CMD:
                {
                    GError *error = NULL;

                    wifi_service_call_start_scan_sync(proxy, NULL, &error);
                    if (error) {
                        printf("Start Scan error: %s\n", error->message);
                    } else {
                        printf("Start Scan succ\n");
                    }
                }
                break;
            case GET_SCAN_RESULTS_CMD:
                {
                    GError *error = NULL;
                    GVariant *scanResults;
                    GVariantIter *iter, *_iter;

                    wifi_service_call_get_scan_results_sync(proxy, &scanResults, NULL, &error);

                    if (error) {
                        printf("Get scan results error: %s\n", error->message);
                    } else {
                        g_variant_get(scanResults, "a(ssiiiasb)", &iter);

                        gchar *ssid, *bssid;
                        gint channel, rssi, level;
                        gboolean wps;

                        while (g_variant_iter_loop(iter, "(ssiiiasb)", &ssid, &bssid, &channel, &rssi, &level, &_iter, &wps)) {
                            gchar *auth;

                            printf("ssid:%s\nbssid:%s\nchannel:%d\nrssi:%d\nlevel:%d\n", ssid, bssid, channel, rssi, level);
                            while (g_variant_iter_loop(_iter, "s", &auth))
                                printf("auto:%s\n", auth);
                            printf("wps:%d\n\n", wps);
                        }

                        g_variant_iter_free(iter);
                        g_variant_unref(scanResults);
                    }

                }
                break;
            case CONNECT_CMD:
                {
                    GError *error = NULL;
                    GVariant *args;
                    char ssid[32] = {0,};
                    char psk[32] = {0,};

                    printf("input ssid:");
                    scanf("%s", ssid);

                    getchar();

                    printf("input password:");
                    scanf("%[^\n]", psk);

                    args = g_variant_new("(ss)", ssid, psk);
                    wifi_service_call_connect_sync(proxy, args, NULL, &error);

                    if (error) {
                        printf("Connect fail: %s\n", error->message);
                    } else {
                        printf("Connect succ!!\n");
                    }
                    break;
                }
                break;
            case DISCONNECT_CMD:
                {
                    GError *error = NULL;

                    wifi_service_call_disconnect_sync(proxy, NULL, &error);

                    if (error) {
                        printf("Disconnect fail: %s\n", error->message);
                    } else {
                        printf("Disconnect succ!!\n");
                    }
                }
                break;
            case PERIODIC_SCAN_CMD:
                {
                    GError *error = NULL;
                    char interval[8] = { 0, };

                    printf("input interval:");
                    scanf("%s", interval);

                    GVariant *args;
                    args = g_variant_new("(i)", atoi(interval));

                    wifi_service_call_set_periodic_scan_interval_sync(proxy, args, NULL, &error);

                    if (error) {
                        printf("set interval fail: %s\n", error->message);
                    } else {
                        printf("set interval succ!!\n");
                    }
                }
                break;
            case IS_WIFI_ENABLED_CMD:
                {
                    gboolean enabled;

                    wifi_service_call_is_wifi_enabled_sync(proxy, &enabled, NULL, NULL);

                    if (enabled) {
                        printf("Wifi is enabled\n");
                    } else {
                        printf("Wifi is disabled\n");
                    }
                }
                break;
            case GET_CONNECTION_INFO_CMD:
                {
                    GError *error = NULL;
                    GVariant *info;

                    gchar *ssid, *bssid, *auth, *ip, *gw;
                    gint channel, freq, rssi, level, linkSpeed;
                    wifi_service_call_get_connection_info_sync (
                            proxy,
                            &info,
                            NULL,
                            &error);

                    if (error) {
                        printf("Get connection info error: %s\n", error->message);
                    } else {
                        g_variant_get(info, "(ssiisiiiss)", &ssid, &bssid, &channel,
                                &freq, &auth, &rssi, &level, &linkSpeed, &ip, &gw);

                        printf("connection ssid: %s\n", ssid);
                        printf("connection bssid: %s\n", bssid);
                        printf("connection channel: %d\n", channel);
                        printf("connection freq: %d\n", freq);
                        printf("connection auth: %s\n", auth);
                        printf("connection rssi: %d\n", rssi);
                        printf("connection level: %d\n", level);
                        printf("connection linkspeed: %d\n", linkSpeed);
                        printf("connection ip: %s\n", ip);
                        printf("connection gateway: %s\n", gw);
                    }

                }
                break;
            case GET_SAVED_NETWORKS_CMD:
                {
                    GVariant *networks;
                    GVariantIter *iter;
                    GError *error = NULL;

                    wifi_service_call_get_saved_networks_sync(proxy, &networks, NULL, &error);

                    if (error) {
                        printf("Get saved networks error: %s\n", error->message);
                    } else {
                        g_variant_get(networks, "a(issibb)", &iter);

                        gchar *ssid, *psk;
                        gint netId, priority;
                        gboolean enabled, hidden;

                        while (g_variant_iter_loop(iter, "(issibb)", &netId, &ssid,
                                    &psk, &priority, &enabled, &hidden)) {
                            printf("netid:%d\nssid:%s\npsk:%s\npriority:%d\nenabled:%d\nhidden:%d\n",
                                    netId, ssid, psk, priority, enabled, hidden);
                            printf("\n");
                        }

                        g_variant_iter_free(iter);
                        g_variant_unref(networks);
                    }

                }
                break;
            case SELECT_NETWORK_CMD:
                {
                    GError *error = NULL;
                    char netId[8] = { 0, };

                    printf("input netId:");
                    scanf("%s", netId);

                    GVariant *args;
                    args = g_variant_new("(i)", atoi(netId));

                    wifi_service_call_select_network_sync(proxy, args, NULL, &error);

                    if (error) {
                        printf("Select network fail: %s\n", error->message);
                    } else {
                        printf("Select network succ!!\n");
                    }
                }
                break;
            case GET_STATE_CMD:
                {
                    gint state;
                    state = wifi_service_get_state(proxy);

                    printf("WifiService State is %d\n", state);
                }
                break;
            case GET_PERIODIC_INTERVAL_CMD:
                {
                    gint interval = 1;
                    interval = wifi_service_get_periodic_scan_interval(proxy);

                    printf("Periodic Scan interval is %d\n", interval);
                }
                break;
            case WIFI_STOP_HOSTAP:
                {
                    GError *error = NULL;
                    GVariant *args;

                    char ssid[32] = {0,};
                    char psk[32] = {0,};
                    args = g_variant_new("(ss)", ssid, psk);

                    wifi_service_call_stop_host_ap_sync(proxy, args, NULL, &error);
                    
                    if (error) {
                        printf("start hostap fail: %s\n", error->message);
                    } else {
                        printf("start hostap succ!!\n");
                    }
                    break;	

                
                }
                break;
            case WIFI_START_HOSTAP:
                {
                    GError *error = NULL;
                    GVariant *args;
                    char ssid[32] = {0,};
                    char psk[32] = {0,};
                    /*
                    printf("input ssid:");
                    scanf("%s", ssid);
                    
                    getchar();
                    
                    printf("input password:");
                    scanf("%[^\n]", psk);
                    
                    save_wifiinfo(ssid,psk);*/
  

                    args = g_variant_new("(ss)", ssid, psk);

                    //wifi_service_call_stop_host_ap_sync(proxy, args, NULL, &error);
            

                    wifi_service_call_start_host_ap_sync(proxy, args, NULL, &error);
                    
                    if (error) {
                        printf("start hostap fail: %s\n", error->message);
                    } else {
                        printf("start hostap succ!!\n");
                    }
                    break;				
                }
            case SET_ETH0_IP:
            {
                printf("ifconfig eth0 192.168.2.2");
                
                system("ifconfig eth0 192.168.2.2");

                break;

            }
            case EXIT_CMD:
            {
                exit_test = true;
                g_main_loop_unref(loop);
                break;
            }
            default:
                break;
        }

        getchar(); // read "enter"
    }

    g_object_unref(proxy);

    return 0;
}
