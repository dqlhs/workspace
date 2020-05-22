#ifndef HICARLINK_H
#define HICARLINK_H
#include "string"
#include "HwRemoteControlEvent.h"
#include "hicar.h"
enum HwRemoteCtrlKeyCode
{
    KEYCODE_MEDIA_PLAY_PAUSE = 85,
    KEYCODE_MEDIA_STOP = 86,
    KEYCODE_MEDIA_NEXT = 87,
    KEYCODE_MEDIA_PREVIOUS = 88,
    KEYCODE_MEDIA_REWIND = 89,
    KEYCODE_MEDIA_FAST_FORWARD = 90,
    KEYCODE_MEDIA_PLAY = 126,
    KEYCODE_MEDIA_PAUSE = 127,
    KEYCODE_CALL = 5,
    KEYCODE_ENDCALL = 6,
    KEYCODE_DPAD_UP = 19,
    KEYCODE_DPAD_DOWN = 20,
    KEYCODE_ADV_ON = 2000,
    KEYCODE_ADV_OFF = 2001
};

class hicarListener
{
public:
    virtual void recvConnectState(int state) = 0;
    virtual void recvServerState(int state) = 0;
    virtual void recvPinCode(std::string pinCode) = 0;
    virtual void recvVideoState(bool state) = 0;
};

class hicarlink
{
public:
    hicarlink();
    virtual ~hicarlink();
    static hicarlink *getInstance();
    virtual void initHicar();
    virtual void addListener(hicarListener* plistener);
    virtual void startAdv();
    virtual void restartAdv();
    virtual void autoConnect(std::string btMac);
    virtual void startProjection();
    virtual void stopProjection();
    virtual void pauseProjection();
    virtual void voiceWakeupRequest(std::string hotWord);
    virtual void sendTouchEvent(HwRemoteCtrlTouchEventDetails touchEvt);
    virtual void sendKeyEvent(HwRemoteCtrlKeyEventType eventType,HwRemoteCtrlKeyCode keyCode);
};

#endif // HICARLINK_H
