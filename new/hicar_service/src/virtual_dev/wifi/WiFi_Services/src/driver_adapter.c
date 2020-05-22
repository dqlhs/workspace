#ifdef __cplusplus
extern "C"
{
#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <glib.h>
#include <gio/gio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <asm-generic/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <driver_adapt.h>
#include <sys/select.h>
#include <sys/msg.h>
#include <stdint.h>
#include <inttypes.h>

#include <WifiService.h>
#include <WifiClient.h>

#include <wifilocal.h>

pthread_t socket_tid;
struct WifiApConnectInfo *g_pstWifiApInfo;
static struct WifiApConnectInfo WifiApConnectInfo;
static GMainLoop *loop;
static bool is_print_menu = true;
ChannelCallback g_callback;
extern int sock_send(int sockfd, void *buf, size_t len, int flags);
extern void socket_server_init(int *port);
extern int socket_client_init(char *ipadd, int port);
// extern  int client_server_main(void);
extern void sock_close(int socket);
static int clientthread = 0;

enum WifiState
{
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
    "IFI_STATE_ENABLING",
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
    "HOSTAP_STATE_CLIENT_DISCONNECT"

};

uint64_t local_timestamp()
{
    uint64_t time;
    struct timespec ts = {0};
    clock_gettime(CLOCK_MONOTONIC, &ts);

    time = ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
    return time;
}

static void init_client_server(void)
{
    INFO(" init_client_server");
    //(void)client_server_main();
}
/*
/*  Wifi P2P 通道需要实现的事件：
/*  1、连接成功         ChannelStateChange   EVENT_CONNECT
/*  2、连接断开         ChannelStateChange   EVENT_DISCONNECT
/*  3、热点创建成功        ChannelStateChange  EVENT_CREAT_AP_SUCCESS, 需要上报数据 WifiChannelData
/*  4、设置本地IP信息成功 ChannelStateChange EVENT_CONFIG_IP_SUCCESS 需要上报数据 WifiChannelData
/*  5、接收到数据 ReceiveData 回调
*/

// 创建AP 热点,  wifi 信道,channel,wifi 频段frequency,会在wifiData下发，ssid 和 pwd 由车机侧生成。
static int initHostAp(WifiService *proxy)
{
    GError *error = NULL;
    GVariant *args;
    char ssid[24] = {
        0,
    };
    char psk[24] = {
        0,
    };

    args = g_variant_new("(ss)", g_pstWifiApInfo->ssid, g_pstWifiApInfo->key);

    wifi_service_call_start_host_ap_sync(proxy, args, NULL, &error);

    if (error)
    {
        ERROR("start hostap fail: %s", error->message);
    }
    else
    {
        INFO("start hostap succ!!");
    }
    return 0;
}

int getlocaistr(char *path, char *result)
{
    FILE *fp = NULL; //notice
    char *filePath = path;
    int filesize;

    fp = fopen(filePath, "at+");
    if (fp == NULL)
    {
        INFO("getlocaistr error");
        return;
    }

    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    fread(result, filesize, 1, fp);
    result[filesize] = '\0';
    INFO(" GET_DEV_MAC = %s", result);
    return filesize + 1;
}

void WifiStateChange(int state, char *addr)
{
    int custate = state;
    char *staMac = addr;
    int second_port = 40864;
    char *path = "/etc/mac";
    char buf[20];

    WifiParams para;
    memset(&para, 0, sizeof(para));

    switch (custate)
    {
    case HOSTAP_STATE_DISABLE:
        INFO(" WifiStateChange HOSTAP_STATE_DISABLE");

        break;
    case HOSTAP_STATE_ENABLED:
    {
        char buf[20];

        char *path = "/etc/mac";
        getlocaistr(path, buf);
        memcpy(g_pstWifiApInfo->localMac, buf, strlen(buf) + 1);

        WifiParams para;
        memset(&para, 0, sizeof(para));

        if (g_callback.ChannelStateChange)
        {
            memcpy(para.ssid, g_pstWifiApInfo->ssid, strlen(g_pstWifiApInfo->ssid) + 1);
            memcpy(para.key, g_pstWifiApInfo->key, strlen(g_pstWifiApInfo->key) + 1);
            memcpy(para.mac, g_pstWifiApInfo->localMac, strlen(g_pstWifiApInfo->localMac) + 1);

            para.frequency = g_pstWifiApInfo->frequency;
            INFO(" WifiCreateAP success ");

            g_callback.ChannelStateChange(CHANNEL_P2P, EVENT_CREATE_WIFI_SUCCESS, (void *)&para, sizeof(para));
        }
        else
        {
            INFO(" error no init g_callback");
        }
    }
    break;
    case HOSTAP_STATE_CLIENT_CONNECT:
        INFO(" WifiStateChange CONNECT mac: a cellphone connect the ap");

        // memcpy(g_pstWifiApInfo->client.mac,staMac,strlen(staMac));

        if (g_callback.ChannelStateChange)
        {
            memcpy(para.ssid, g_pstWifiApInfo->ssid, strlen(g_pstWifiApInfo->ssid) + 1);
            memcpy(para.key, g_pstWifiApInfo->key, strlen(g_pstWifiApInfo->key) + 1);

            para.frequency = g_pstWifiApInfo->frequency;

            INFO(" EVENT_STA_ON_LINE ");

            g_callback.ChannelStateChange(CHANNEL_P2P, EVENT_CONNECT, (void *)&para, sizeof(para));
        }
        else
        {
            INFO(" error no init g_callback");
        }
        break;
    case HOSTAP_STATE_CLIENT_DISCONNECT:
        INFO(" WifiStateChange DISCONNECT maa cellphone disconnect the ap");

        /* colse default socket */
        if (g_callback.ChannelStateChange)
        {
            memcpy(para.ssid, g_pstWifiApInfo->ssid, strlen(g_pstWifiApInfo->ssid) + 1);
            memcpy(para.key, g_pstWifiApInfo->key, strlen(g_pstWifiApInfo->key) + 1);

            para.frequency = g_pstWifiApInfo->frequency;
            INFO(" EVENT_STA_OFF_LINE ");

            g_callback.ChannelStateChange(CHANNEL_P2P, EVENT_DISCONNECT, (void *)&para, sizeof(para));
        }
        else
        {
            INFO(" error no init g_callback");
        }

        /* nearby call wifideinit */
        // WifiClientDisConnect(staMac);

        // memset(&g_pstWifiApInfo->client,0,sizeof(g_pstWifiApInfo->client));

        break;
    case HOSTAP_STATE_IP:
        INFO(" WifiStateChange HOSTAP_STATE_IP");

        break;
    default:
        break;
    }
}

static void on_wifi_state_changed_hostap(WifiClient *proxy, GVariant *currentState)
{
    gint curstate;
    gchar *staMac;

    INFO(" on_wifi_state_changed_hostap ok ok ok ok ");

    g_variant_get(currentState, "(is)", &curstate, &staMac);

    INFO("[STATE CHANGED] mac:%s currentState = %d", staMac, curstate);
    // if(g_pstWifiApInfo->connectstate == curstate)
    // {
    //     INFO(" app/ap always stay on this status");
    //     return;
    // }
    // g_pstWifiApInfo->connectstate = curstate;

    // WifiStateChange((int)curstate,(char *)staMac);

    INFO(" on_wifi_state_changed_hostap end ");

    return;
}

#define AP_EVENT_ENABLED "AP-ENABLED "
#define AP_EVENT_DISABLED "AP-DISABLED "
#define AP_STA_CONNECTED "AP-STA-CONNECTED "
#define AP_STA_DISCONNECTED "AP-STA-DISCONNECTED "

void handle_dispatch_pack(char *buf, int len)
{
    int curstate = 0;
    INFO("handle_dispatch_packet start ");

    if (!strcmp(buf, AP_EVENT_DISABLED))
    {
        curstate = HOSTAP_STATE_DISABLE;
    }
    else if (!strcmp(buf, AP_EVENT_ENABLED))
    {
        curstate = HOSTAP_STATE_ENABLED;
    }
    else if (!strcmp(buf, AP_STA_CONNECTED))
    {
        curstate = HOSTAP_STATE_CLIENT_CONNECT;
    }
    else if (!strcmp(buf, AP_STA_DISCONNECTED))
    {
        curstate = HOSTAP_STATE_CLIENT_DISCONNECT;
    }
    else
    {
        INFO("ERROR NO CMD,%s", buf);
    }

    if (g_pstWifiApInfo->connectstate == curstate && g_pstWifiApInfo->connectstate != 0)
    {
        INFO(" app/ap always stay on this status %d", curstate);
        return;
    }

    INFO("BBBBBBBBBBBBBBBBB hostapd send up:%s,%d", buf, curstate);
    if (curstate)
    {
        g_pstWifiApInfo->connectstate = curstate;
        WifiStateChange(curstate, NULL);
    }

    return;
}
int g_cmd = 0;
static void *thread_main(void *arg)
{
    WifiClient *client_proxy = NULL;

    /* init server */
    init_client_server();

    client_proxy = wifi_client_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                      G_DBUS_PROXY_FLAGS_NONE,
                                                      "com.hsaeyz.sal.WifiClient",
                                                      "/com/hsaeyz/sal/WifiClient",
                                                      NULL, NULL);
    if (!client_proxy)
    {
        ERROR("wifi_client_proxy_new_for_bus_sync failed");
        return NULL;
    }

    g_signal_connect((WifiClient *)client_proxy, "hostap-stat-change",
                     G_CALLBACK(on_wifi_state_changed_hostap), NULL);

    INFO(" thread_main end ");

    loop = g_main_loop_new(NULL, FALSE);

    g_main_loop_run(loop);

    return NULL;
}
bool g_clientinit = false;
static WifiService *wifi_proxy;
int WifiStopAP(void *priv, void *wifiData)
{

    if (!wifi_proxy)
    {
        wifi_proxy = wifi_service_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, "com.hsaeyz.sal.WifiService", "/com/hsaeyz/sal/WifiService", NULL, NULL);

        if (!wifi_proxy)
        {
            ERROR("wifi_service_proxy_new_for_bus_sync failed");
            return -1;
        }
    }
    GError *error = NULL;
    GVariant *args;

    char ssid[32] = {
        0,
    };
    char psk[32] = {
        0,
    };
    args = g_variant_new("(ss)", ssid, psk);

    wifi_service_call_stop_host_ap_sync(wifi_proxy, args, NULL, &error);

    if (error)
    {
        ERROR("start hostap fail: %s", error->message);
    }
    else
    {
        ERROR("start hostap succ!!");
    }
    return 0;
}

int thread_create_ap(void *s)
{
    wifi_proxy = wifi_service_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, "com.hsaeyz.sal.WifiService", "/com/hsaeyz/sal/WifiService", NULL, NULL);

    if (!wifi_proxy)
    {
        ERROR("wifi_service_proxy_new_for_bus_sync failed");
        return -1;
    }

    if (pthread_create(&g_pstWifiApInfo->client_tid, NULL, thread_main, wifi_proxy))
    {
        INFO(" pthread_create error");
        return -1;
    }
    else
    {
        INFO("thread_craete is ok");
    }

    // initHostAp(wifi_proxy);
    return 0;
}
static int system_check(int result)
{
    if ((-1 != result) &&
        (WIFEXITED(result)) && (!(WEXITSTATUS(result))))
        return 0;
    else
        return -1;
}

void WifiCreateSsidKey(char *ssid, char *key)
{
    int i = RAND_MAX;

    INFO("RAND_MAX=0x%x   time(0)=%d", RAND_MAX, (int)time(0));
    srand((int)time(NULL));
    /* 输出 10 个随机数. */

    sprintf(ssid, "HiCar-%4d", rand());
    sprintf(key, "%5d", rand());

#ifdef DEBUG
    // {
    //     char *defaultssid = "HiCar-12345678";
    //     char *defaultkey = "123456789";
    //     memcpy(ssid,defaultssid,strlen(defaultssid) + 1);
    //     memcpy(key,defaultkey,strlen(defaultkey) + 1);
    // }
#endif
    return;
}
int WifiSetLocalIp(const char *ip)
{
    int i;
    char stringcmd[100];
    char buf[20];
    FILE *fp = NULL; //notice
    char *filePath = "/etc/ip";

    sprintf(stringcmd, "cat /dev/null > %s", filePath);
    system(stringcmd);

    fp = fopen(filePath, "at+");
    if (fp == NULL)
    {
        INFO(" error");
        return -1;
    }

    fwrite(ip, strlen(ip), 1, fp);
    fflush(fp);

    INFO("file set start");

    fseek(fp, 0, SEEK_SET); //set start file
    fread(buf, strlen(ip), 1, fp);
    buf[strlen(ip) + 1] = '\0';
    INFO("buf = %s", buf);

    fclose(fp);
    return 0;
}

static void on_wifi_state_changed(WifiService *proxy, gint currentState,
                                  gint previousState)
{
    INFO("[STATE CHANGED]currentState = %s, previousState = %s", state_names[currentState], state_names[previousState]);
}

static void on_wifi_signal_level_changed(WifiService *proxy, gint currentLevel,
                                         gint previousLevel)
{
    INFO("[SIGNAL LEVEL CHANGED]currentLevel = %d, previousLevel = %d", currentLevel, previousLevel);
}

static int WifiGetParam(GetParamsType paramType, void *result)
{
    /**< 设备Wifi能力信息，2.4G ~ 0x02 , 5G ~ 0x01 , UNKOWN ~ 0x03*/
    //暂时只需要从Wifi模块获取这个值 FC_GET_WIFI_ABILITY

    if (paramType == GET_WIFI_ABILITY)
    {
        char *countrycode = "CN";
        char *path = "/etc/mac";

        WifiAbilityParam *macpara = (WifiAbilityParam *)result;

        memcpy(&macpara->countryCode, countrycode, strlen(countrycode) + 1);

        macpara->isSupportAband = 1;
        macpara->isSupportGO = 1;
        macpara->isSupportGC = 0;
        macpara->isSupportRSDB = 0;
        if (g_pstWifiApInfo)
        {
            macpara->currStaFreq = g_pstWifiApInfo->frequency;
            memcpy(&macpara->selfGcMac, g_pstWifiApInfo->client.mac, strlen(g_pstWifiApInfo->client.mac));
            getlocaistr(path, macpara->selfBssid);
        }
    }
    else if (paramType == GET_DEV_MAC)
    {

        char *mac = (char *)result;
        char *path = "/etc/mac";
        getlocaistr(path, mac);
    }
    else
    {
        INFO(" WifiGetParam paramType error ");
    }
    INFO(" WifiGetParam ");
    return 0;
}

static int WifiRegisterCallBack(const ChannelCallback *callback)
{
    g_callback.ChannelStateChange = callback->ChannelStateChange;
    g_callback.ReceiveData = callback->ReceiveData;

    INFO(" WifiRegisterCallBack");
    return 0;
}

int channel_to_freq(int channel)
{
    int freq=0;
    if((channel>=1)&&(channel<=13))
    {
        freq = channel*5+2407;
    }else if(channel >= 36 && channel < 49){
		freq = channel*5+5000;
    }else if(channel >= 100 && channel <= 169){
		freq = channel*5+5000;
    }else{
        INFO("errorerror channel :%d",channel);
        freq = 0;
    }
    return freq;

}
void sfind_out_str(char *path,char *str,char*buf)
{
    int i;
    FILE *fp = fopen(path,"r");
    if(!fp)
    {
        INFO("fopen error");
        return;
    }

    while(!feof(fp))
    {
        fgets(buf,50,fp);
        if(strncmp(buf,str,strlen(str)) == 0)
        {
            // INFO("%s",buf);
            break;
        } 
    }
    i = strlen(buf);
    buf[i-1] = '\0';

    fclose(fp);
}
void wifi_save_info(void)
{
    char *path = "/etc/hostap.conf";
    char *ssid = "ssid=";
    char *channel = "channel=";
    char *key = "wpa_passphrase=";
    char buf[50];

    //system("kill `cat /var/run/hostapd.pid`");
    memset(buf,0,sizeof(buf));
    
    sfind_out_str(path,channel,buf);
    INFO("%s",buf);
    //strcpy(g_pstWifiApInfo->channel,buf+strlen(ssid));
    int len = strlen(buf)-strlen(channel);
    int num=0,i,j=0;
    int temp,a=0;

    i=0;
    j=0;
    temp = 1;
    num=0;
    while(i<len)
    {
        temp = 1;
        while(j<i)
        {
            temp *= 10;
            j++;
        }
        j=0;
        a = buf[strlen(buf)-i-1] - '0';
        num += a*temp;
        i++;
    }
    g_pstWifiApInfo->channel = num;
    g_pstWifiApInfo->frequency = channel_to_freq(num);
    INFO("buf:%s,channel:%d,len:%d,freq:%d",buf,num,len,g_pstWifiApInfo->frequency);

    memset(buf,0,sizeof(buf));
    sfind_out_str(path,ssid,buf);
    strcpy(g_pstWifiApInfo->ssid,buf+strlen(ssid));
    INFO("ssid:%s,g_pstWifiApInfo->ssid:%s",buf,g_pstWifiApInfo->ssid);


    memset(buf,0,sizeof(buf));
    sfind_out_str(path,key,buf);
    strcpy(g_pstWifiApInfo->key,buf+strlen(key));
    INFO("key:%s,g_pstWifiApInfo->key:%s",buf,g_pstWifiApInfo->key);


    return;
}
int freq_to_channel(int freq)
{
    int channel = 0;

    if (freq >= 2412 && freq <= 2472)
    {
        channel = (freq - 2407) / 5;
    }
    else if (freq >= 4900 && freq < 5000)
    {
        channel = (freq - 4000) / 5;
    }
    else if (freq >= 5180 && freq <= 5240)
    {
        channel = (freq - 5000) / 5;
    }
    else if (freq >= 5745 && freq <= 5845)
    {
        channel = (freq - 5000) / 5;
    }
    else
    {
        INFO("errorerror freq :%d", freq);
        channel = 0;
    }

    return channel;
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
        pclose(fp);
    }
    printf("pid = %d \n", pid);
#endif
    return pid;
}
// #define BUFSIZ 100
struct msg_st
{  
    long int msg_type; 
    char text[BUFSIZ];
};
int msg_main(int *p)
{  
    int rv;
    int running = 1;   
    int msgid = -1;
    struct msg_st data;
    long int msgtype = 0; //注意1     //建立消息队列   
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT); 
    if(msgid == -1)
    {      
        INFO("msgget failed with error: %d\n", errno);
        return -1;
        // exit(EXIT_FAILURE);
    }   //从队列中获取消息，直到遇到end消息为止 
    while(running) 
    {  
        if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1)  
        {  
            INFO("msgrcv failed with errno: %d\n", errno);  
            // exit(EXIT_FAILURE);    
            break;
        }  
        // INFO("You wrote: %s\n",data.text);        //遇到end结束  
        
        INFO("msg recv");

        handle_dispatch_pack(data.text, BUFSIZ);

        if(strncmp(data.text, "end", 3) == 0)      
            running = 0;
    }//删除消息队列   

    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {      
        INFO("msgctl(IPC_RMID) failed\n");  
        // exit(EXIT_FAILURE);
    }  
    // exit(EXIT_SUCCESS);
	pthread_exit(&rv);
    return 0;
}
int WifiCreateAP(const void *context, const void *wifiData)
{
    INFO("==> %s", __func__);
    pthread_t tid;

    char *hostapd = "hostapd";
    char wuG[] = "/etc/test_5G.conf";
    char siG[] = "/etc/test_24G.conf";
    char *mode;
    char stringcmd[100];
    char stringip[20];
    int channel;
    int port = 9001;

    /* ap always init */
    int frequency = *((int *)wifiData);
    INFO(" frequency :%d", frequency);

    int kill_rc = pthread_kill(socket_tid, 0);
    if (kill_rc == ESRCH)
    {
        INFO("the socket thread did not exists or already quit/n");
        if (pthread_create(&socket_tid, NULL, (void *)msg_main, &port))
        {
            INFO("\r\n pthread_create error\r\n");
        }
        sleep(1);
    }
    else if (kill_rc == EINVAL)
        INFO("signal is invalid/n");
    else
        INFO("the socket thread is alive/n");

    if (wifiData == NULL)
    {
        INFO("error wifiData is Null ");
        return -1;
    }

    channel = freq_to_channel(frequency);
    if (channel == 0)
    {
        INFO("freq is invalid");
        return -1;
    }

    if (channel > 167 && channel < 1)
    {
        INFO(" channel set error  ");
        return -1;
    }

    memset(&WifiApConnectInfo, 0, sizeof(WifiApConnectInfo));
    g_pstWifiApInfo = &WifiApConnectInfo;
    sem_init(&g_pstWifiApInfo->sem, 0, 0); //初始化信号量

    g_pstWifiApInfo->frequency = frequency;

    // thread_create_ap(NULL);

    if((fopen("/var/run/hostapd.pid","r") != NULL) && (GetProcessidFromName(hostapd) > 0))
    // if(fopen("/var/run/hostapd.pid","r") != NULL) 
    {
        // system("kill `cat /var/run/hostapd.pid`");
        wifi_save_info();
        WifiStateChange(HOSTAP_STATE_ENABLED,NULL);
        // usleep(50000);
    }
    else
    {

        WifiCreateSsidKey(g_pstWifiApInfo->ssid, g_pstWifiApInfo->key);
        if (channel > 14)
        {
            mode = &wuG[0];
        }
        else
        {
            mode = &siG[0];
        }
        /*set ssid*/
        sprintf(stringcmd, "sed \"s/ssid=Ap123456789456/ssid=%s/g\" %s > /etc/test1.conf ", g_pstWifiApInfo->ssid, mode);
        INFO(" %s", stringcmd);
        system(stringcmd);

        /*set key*/
        sprintf(stringcmd, "sed \"s/wpa_passphrase=123456789/wpa_passphrase=%s/g\" /etc/test1.conf  > /etc/test2.conf ", g_pstWifiApInfo->key);
        INFO(" %s", stringcmd);
        system(stringcmd);

        /* set channel */
        sprintf(stringcmd, "sed \"s/CHANNELCHANNELID/channel=%d/g\" /etc/test2.conf  > /etc/hostap.conf ", channel);
        INFO(" %s", stringcmd);
        system(stringcmd);

        int i;
        int ret = -1;

        for (i = 0; i < 3; i++)
        {
            ret = system("/usr/bin/hostapd -B /etc/hostap.conf -dddd  -P /var/run/hostapd.pid -f /tmp/hostapd.log  -e /etc/entropy.bin ");

            if (system_check(ret))
            {
                INFO(",start ERROR! ERROR! ,%d,ret:%d BBBBBBBBB", i, ret);
                ret = 1;
            }
            else
            {
                ret = 0;
                INFO(" hostap start succ :%d ", i);
                break;
            }
        }
        sleep(1);
    }

    INFO(" WifiCreateAP end");

    return 0;
}

static int WifiSetParam(SetParamsType paramType, const void *priv)
{
    INFO(" WifiSetParam :%d", paramType);
    char stringcmd[50];
    pthread_t rectid;
    int tag = 0;

    int ret;
    int i;
    struct channel *pstChannelInfo;

    if (paramType == SET_WIFI_LOCAL_IP)
    {
        WifiSetIpParam *ipParam = (WifiSetIpParam *)priv;

        char *localIp = (char *)&ipParam->ipAddr;

        // 本地化IP
        WifiParams para;

        WifiSetLocalIp(localIp);

        sprintf(stringcmd, "ifconfig wlan0 %s", localIp);
        ret = system(stringcmd);

        if (fopen("/run/dnsmasq.pid", "r") != NULL)
        {
            system("kill `cat /run/dnsmasq.pid`");
        }

        if (fopen("/var/run/dnsmasq.wlan0.pid", "r") != NULL)
        {
            // system("kill `cat /var/run/dnsmasq.wlan0.pid`");
            // system("rm /var/run/dnsmasq.wlan0.pid");
        }
        else
        {
            INFO(" start dhcp server");
            system("dnsmasq -x /var/run/dnsmasq.wlan0.pid -l /var/run/dnsmasq-wlan0.leases -i wlan0 -5 --except-interface=lo --dhcp-range=192.168.43.2,192.168.43.2,12h --dhcp-option=3,192.168.43.1 --bind-interfaces");

            if (system_check(ret))
            {
                INFO(" ifconfig wlan0 ip error,ret:%d,ip:%s", ret, localIp);
            }
            usleep(200 * 1000);
        }

        memcpy(para.ssid, g_pstWifiApInfo->ssid, strlen(g_pstWifiApInfo->ssid) + 1);
        memcpy(para.key, g_pstWifiApInfo->key, strlen(g_pstWifiApInfo->key) + 1);

        para.frequency = g_pstWifiApInfo->frequency;

        if (g_callback.ChannelStateChange)
        {
            g_callback.ChannelStateChange(CHANNEL_P2P, EVENT_CONFIG_IP_SUCCESS, (void *)&para, sizeof(para));
        }
    }
    else
    {
        INFO(" paramType is INVAIL : %d", paramType);
    }
    return 0;
}

int WifiInit(const void *context, const char *ifName)
{
    int port = 9001;

    INFO("WifiInit:%s", ifName);

    if (strcmp(ifName, WIFI_DRIVER_NAME))
    {
        INFO(" do not kill me ,my name is %s,ifName:%d", WIFI_DRIVER_NAME, ifName);
    }
    if (pthread_create(&socket_tid, NULL, (void *)msg_main, &port))
    {
        INFO("\r\n pthread_create error\r\n");
    }
    sleep(1);

    return 0;
}
int WifiDeInit(const void *context)
{
    INFO(" WifiDeInit ");

    if (fopen("/var/run/dnsmasq.wlan0.pid", "r") != NULL)
    {
        system("kill `cat /var/run/dnsmasq.wlan0.pid`");
        system("rm /var/run/dnsmasq.wlan0.pid");
    }

    if (fopen("/var/run/hostapd.pid", "r") != NULL)
    {
        system("kill `cat /var/run/hostapd.pid`");
    }

    // sem_destroy(&g_pstWifiApInfo->sem); //使用结束后要把信号量给回收
    pthread_cancel(socket_tid);

    memset(g_pstWifiApInfo, 0, sizeof(WifiApConnectInfo));

    return 0;
}

const struct FcDriverAdaptOps g_fcDriverWifiOps = {
    .name = WIFI_DRIVER_NAME,
    .Init = WifiInit,
    .Deinit = WifiDeInit,
    .SetParam = WifiSetParam,
    .GetParam = WifiGetParam,
    .RegisterChannelCallback = WifiRegisterCallBack,
    .CreateWifiChannel = WifiCreateAP,
};
#ifdef __cplusplus
}
#endif

#endif
