#ifndef _SPPSTUBIMP_H_
#define _SPPSTUBIMP_H_
#include "../src-gen/Spp/spp.h"
#include <semaphore.h>
#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "HsaeTypes.h"
#include "DeviceStubImp.h"
#include<queue>
#include <map>
using namespace com::hsae::bluetooth::util::HsaeTypes;
using namespace std;
struct SPP_DATA
{
    char * fisrtdata ;
    int len;
    int profile;
    SPP_DATA(){}
    SPP_DATA(char* data,int len,int id)
    {
        if (len>0)
        {
            this->fisrtdata = new char[len];
            memcpy(fisrtdata,data,len); 
            this->len=len;
        }
        profile =id;

    }
    ~SPP_DATA()
    {
//         printf("~~~~delete ~~~\r\n");
//    if (fisrtdata!= NULL)
//        delete[]fisrtdata;
}

};
class SPPStubImp {//: public SettingStubDefault {

public:
    SPPStubImp();
    static std::shared_ptr<SPPStubImp> btSppInstance;
    static std::shared_ptr<SPPStubImp> getInstance();
    ~SPPStubImp();
    static Spp* m_interface;
    static gboolean  usersenddata (Spp *object,GDBusMethodInvocation *invocation,  GVariant *arg_data,  gint arg_datalen);
    static gboolean  setparams (Spp *object,GDBusMethodInvocation *invocation,  GVariant *arg_data,  gint arg_datalen);

    static void iapsenddata(int cDataLen , unsigned char * pData );
//    ipod_t handle;
    //method
    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);

    bool createSession();
    bool closeSession();
    static void timeout();
    CTimer m_createtimer;
    bool setsppsate(bool sate);
    bool m_mfi;
    std::queue<SPP_DATA>g_sppqueue;
    sem_t spp_wait_sem;
    char btmac[20];
    void chartostr(char *strmac, unsigned char *mac);

private:

    pthread_t autoconnect_device_thread_id;
    static void* autoConnectDeviceFunc(void* args);
    bool g_bRun;
    string auto_addr;
   void  str_to_char( const char *addr, unsigned char * b );
   int IsFileExist(const char* path);
   bool m_ios;
   static pthread_t dealdata_id;
   static void* dealdataFunc(void* args);
   static pthread_mutex_t mutex_for_data;
   static pthread_cond_t datacond;

public:
    bool m_benableconnect;
    bool isIos();
    bool setios(bool state);
    bool senddata( char *addr, char * b,int len,int rfcommid);
    bool startauthentication();
    int m_startpos;
};
//}
//}
//}
//}

#endif

