#ifndef DRIVER_WIFILOCAL_H
#define DRIVER_WIFILOCAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver_adapt.h>
#include <semaphore.h>


#include <stdint.h>
#include <inttypes.h>

#define NUM_CHANNEL 4
#define USED 1
#define FREE 0


extern uint64_t local_timestamp();

#define FLUSH() fflush(stdout)

#define INFO(msg, arg...) \
    do { \
        printf("[ INFO][%" PRIu64 "][%s][%d] -> {" msg "}\r\n", local_timestamp(), __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define ERROR(msg, arg...) \
    do { \
        printf("[ERROR][%" PRIu64 "][%s][%d] -> {" msg "}\r\n", local_timestamp(), __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#ifndef DEBUG
// #define DEBUG 
#endif
struct channel
{
    /* data */
    int used; /*  */
    int sessionId;
    int port;
    int initok;
    int socketlisten;
    int socketfd;
    int socketserver_times;//socket connect error times
    pthread_t sockserverthread;
    pthread_t rectid;
};

struct WifiClientInfo
{
    /* data */
    int num;
    char ip[16];         /**< 对端IP */
    char mac[20];        /**< 对端设备mac地址 */
    struct channel channelinfo[NUM_CHANNEL]; /* new four datachannel */
};

struct WifiApConnectInfo
{
    /* data */
    char ssid[128];            /**< wifi ssid */
    char key[128];             /**< wifi key */
    int channel;               /**< wifi 信道 */
    int frequency;             /**< wifi 频段*/
    int connectstate;          /**< wifi ap status*/
    char localIp[16];          /**< 本端IP */
    char localMac[20];         /**< 本端设备mac地址 */
    struct WifiClientInfo client; /**< one client info*/
    ChannelCallback callbackwifi; /**< call back*/
    pthread_t hostapd_recv;
    pthread_t client_tid;
    sem_t sem;
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DRIVER_ADAPT_H */