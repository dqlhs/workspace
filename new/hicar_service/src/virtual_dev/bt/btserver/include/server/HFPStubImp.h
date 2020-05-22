#ifndef _HFPSTUBIMP_H_
#define _HFPSTUBIMP_H_
//#include "com/hsae/bluetooth/HFP/PhoneStubDefault.h"
#include "src-gen/HFP/phone.h"
#include <iostream>
#include <memory>
#include <stdio.h>
//#include <strstream>
#include "DeviceStubImp.h"
#include"ctimer.h"
#include "HsaeTypes.h"
using namespace std;
using namespace com::hsae::bluetooth::util::HsaeTypes;

class HFPStubImp// : public PhoneStubDefault
{
private:

public:
    HFPStubImp();
    static std::shared_ptr< HFPStubImp > phoneInstance;
    static std::shared_ptr< HFPStubImp > getInstance();
    static Hfp* m_interface;
    static gboolean m_privateState;
    void inithfp();
    void play(bool state);
    ~HFPStubImp();
    //method
    static gboolean answerCall(Hfp *interface, GDBusMethodInvocation *invocation);         //answerCall
    static gboolean rejectCall(Hfp *interface, GDBusMethodInvocation *invocation);         //rejectCall
    static gboolean terminateCall(Hfp *interface, GDBusMethodInvocation *invocation);      //terminateCall
    static gboolean holdCall(Hfp *interface,GDBusMethodInvocation *invocation,gint action);    //holdCall(int action)
    static gboolean dialDTMF(Hfp *interface,GDBusMethodInvocation *invocation,const gchar* dtmf);//sendDTMF(char* dtmf)
    static gboolean dailCall(Hfp *interface,GDBusMethodInvocation *invocation,const gchar* number);//dailCall(char* number)
    static gboolean audioTransfer(Hfp *interface,GDBusMethodInvocation *invocation,gboolean iActive);          //audioTransfer(bool iActive)
    static gboolean setCallAutoAction(Hfp *interface,GDBusMethodInvocation *invocation,gint action);       //setCallAutoAction(int action)
    static gboolean playRingTone(Hfp *interface, GDBusMethodInvocation *invocation);         //playRingTone
    static gboolean stopRingTone(Hfp *interface, GDBusMethodInvocation *invocation);          //stopRingTone
   //add by lhm 20170907
    static gboolean mutemic(Hfp *interface, GDBusMethodInvocation *invocation,gboolean iActive);          //stopRingTone

    //signal
    void callInNumberChanged(std::string phoneNumber,int index);
    void callStateChanged(CallState callState,int index);
    void scoStateChanged(bool socState);      //PrivateStateChanged
    void chartostr(char *strmac, unsigned char *mac);
    //property
    //PrivateState

    static void on_bus_acquired (GDBusConnection *connection,const gchar *name,gpointer user_data);

    //------------------inner methods-------------------------------------------------
    void HfpConnect(char *path);
    void HfpDisConnect(char *path);
    //2017-4-20
    unsigned long GetTickCount();
    unsigned long m_dwDialMoment;
    static void timeout();

    static void HfpDisconnectResult(DBusPendingCall *call, void *user_data);
public:

    //QIAN_ZHONG ADD 2015-3-26
    static CallState callstate ;
    static string PHONE_STATE;
    CTimer m_downloadtimer;
    char  m_localnumer[20];
    bool m_needdownload;
private:
    void rejectandterminate();

    bool m_playRingTong;
    bool ringing;
    bool threeway;
    pthread_t playringtone_thread_id;
    static void* playringFunc(void* args);
    bool g_bRun;
    CALLSTATE call1;
    CALLSTATE call2;
    string getlocaltime();
//    string gettalktime(unsigned long starttime);
   string gettalktime(time_t starttime);


};
//}
//}
//}
//}

#endif
