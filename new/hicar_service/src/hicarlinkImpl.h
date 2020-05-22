#ifndef HICARLINKIMPL_H
#define HICARLINKIMPL_H
#include <string.h>
#include "stdio.h"
#include "hicarlink.h"
#include "hicar.h"
#include "HicarStream.h"
#include "driver_adapt.h"
using namespace std;
extern  FcDriverAdaptOps  g_fcAdaptBleOps;
extern  FcDriverAdaptOps  g_fcDriverBrOps;

extern  FcDriverAdaptOps  g_fcDriverWifiOps;

extern  FcDriverAdaptOps  g_fcDriverUsbOps;

class hicarlinkImpl:public hicarlink
{
public:
    hicarlinkImpl();
    virtual ~hicarlinkImpl();
    void initHicar();
    void addListener(hicarListener* plistener);
    void startAdv();
    void restartAdv();
    void autoConnect(std::string btMac);
    void startProjection();
    void stopProjection();
    void pauseProjection();
    void voiceWakeupRequest(std::string hotWord);
    void sendTouchEvent(HwRemoteCtrlTouchEventDetails touchEvt);
    void sendKeyEvent(HwRemoteCtrlKeyEventType eventType,HwRemoteCtrlKeyCode keyCode);

private:

};

#endif // HICARLINKIMPL_H
