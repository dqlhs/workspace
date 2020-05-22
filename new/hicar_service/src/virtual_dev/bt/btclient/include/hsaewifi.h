#ifndef HSAEWIFI_H
#define HSAEWIFI_H
#include <string>
#include <gio/gio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <glib-2.0/glib.h>
//#include <driver_adapt.h>
#include "driver_adapt.h"
#define RET_SIGNAL_HANDLE_SIZE 3
#define HSAESUCCESS 0
#define HASEERROR 1
// ID of Bluetooth profiles
typedef enum HSAE_PROFILE_INDEX_e
{
    NPINDEX_HEADSET,
    NPINDEX_HANDFREE,
    NPINDEX_AUDIOSINK,
    NPINDEX_AVRCP,
    NPINDEX_PBAP,
    NPINDEX_MAP,
    NPINDEX_SPP,
    NPINDEX_SYNCML,
    NPINDEX_SPP_IAP,
    NPINDEX_HID,
    NPINDEX_PAN,
    NPINDEX_OPP,
    NPINDEX_RFCOMM_1,
    NPINDEX_RFCOMM_2,
    NPINDEX_RFCOMM_3,
    NPINDEX_RFCOMM_4,
    NPINDEX_RFCOMM_5,
    NPINDEX_RFCOMM_6,
    NPINDEX_RFCOMM_7,
    NPINDEX_RFCOMM_8,
    NPINDEX_RFCOMM_9,
    NPINDEX_ALL_RFCOMM,
    NPINDEX_MAX,
    NPINDEX_ALL_PROFILE = 0xffffffff
} HSAE_PROFILEINDEX;
typedef enum
{
    ALL_AUDIO   = 0x0000,
    HEADSET     = 1<<NPINDEX_HEADSET,
    HANDFREE    = 1<<NPINDEX_HANDFREE,
    AUDIOSINK   = 1<<NPINDEX_AUDIOSINK,
    AVRCP       = 1<<NPINDEX_AVRCP,
    PBAP        = 1<<NPINDEX_PBAP,
    MAP         = 1<<NPINDEX_MAP,
    SPP         = 1<<NPINDEX_SPP,
    SYNCML      = 1<<NPINDEX_SYNCML,
    SPP_IAP     = 1<<NPINDEX_SPP_IAP,
    HID         = 1<<NPINDEX_HID,
    PAN         = 1<<NPINDEX_PAN,
    OPP         = 1<<NPINDEX_OPP,
    RFCOMM_1    = 1<<NPINDEX_RFCOMM_1, /*4096*/
    RFCOMM_2    = 1<<NPINDEX_RFCOMM_2, /*8192*/
    RFCOMM_3    = 1<<NPINDEX_RFCOMM_3, /*16384*/
    RFCOMM_4    = 1<<NPINDEX_RFCOMM_4, /*32768*/
    RFCOMM_5    = 1<<NPINDEX_RFCOMM_5, /*65536*/
    RFCOMM_6    = 1<<NPINDEX_RFCOMM_6,
    RFCOMM_7    = 1<<NPINDEX_RFCOMM_7,
    RFCOMM_8    = 1<<NPINDEX_RFCOMM_8,
    RFCOMM_9    = 1<<NPINDEX_RFCOMM_9,
    ALL_RFCOMM  = 1<<NPINDEX_ALL_RFCOMM,
    MAX         = 1<<NPINDEX_MAX,
    ALL_PROFILE = 0xffffffff
} PROFILEID;
typedef enum CONNECTION_STATE_e
{
    CON_CONNECTION = 0,
    CON_DISCONNECTION,
    CON_NOT_SUPPORT,
    CON_CONNECTING
} CONNECTION_STATE;
struct RF_UUID{
    char uuid[BT_SERVICE_UUID_LEN];
};
class  Hsaewifi
{

public:
    Hsaewifi();
    static Hsaewifi*btInstance;
    static  Hsaewifi*getInstance();
    ~Hsaewifi();
   //ble method
    void sendbledata(unsigned char *data, int len);
    //device method
    int  connectdevice(char* mac,PROFILEID profile,bool state);
    static void connectCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
    static void disconnectCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
//    int  disConnectdevice();
     /*=============================sppmethod===========================================*/
    void sendbrdata(char* data ,int len);
    static void SetParamCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
    static void SendDataCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
     /*==============================end=================================================*/
    static void StartAdvCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
    static void StopAdvCallback(GObject* source_object, GAsyncResult* res, gpointer userdata);
    bool ExampleDBusServiceIsReady(void);
    bool InitDBusCommunication(void);
    bool DeInitDBusCommunication(void);
    //to public
    static char rfcommParams[128];
    RF_UUID rfcomm_id[2];
    char m_bleParams[100];
    int  m_bledatalen;
    std::string m_connectmac ;
//    char m_bleParams[12];

private:
    void sendbtdata(char *data,int len );


};

#endif // HSAEWIFI_H
