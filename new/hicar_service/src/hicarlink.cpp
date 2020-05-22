#include "hicarlink.h"
#include "hicarlinkImpl.h"
#include "log/oxygen_log.h"
//#include <mutex>
#define BT_INI_PATH "/etc/bluetooth/TL.INI"
static hicarlinkImpl *m_instance;
//std::mutex hicar_mutex;

hicarlink::hicarlink()
{
}

hicarlink::~hicarlink()
{
}

hicarlink * hicarlink::getInstance()
{
    if(NULL == m_instance)
    {
//        std::lock_guard<std::mutex> guard(hicar_mutex);
        if(NULL == m_instance)
        {
            m_instance = new hicarlinkImpl();
        }
    }
    return m_instance;
}

void hicarlink::initHicar()
{

}

void hicarlink::addListener(hicarListener * plistener)
{
    TRACE("hicarlink::addListener");
}

void hicarlink::startAdv()
{

}

void hicarlink::restartAdv()
{

}

void hicarlink::autoConnect(string btMac)
{

}

void hicarlink::startProjection()
{

}

void hicarlink::stopProjection()
{

}

void hicarlink::pauseProjection()
{

}

void hicarlink::voiceWakeupRequest(string hotWord)
{

}

void hicarlink::sendTouchEvent(HwRemoteCtrlTouchEventDetails touchEvt)
{

}

void hicarlink::sendKeyEvent(HwRemoteCtrlKeyEventType eventType, HwRemoteCtrlKeyCode keyCode)
{

}

void hicarListener::recvVideoState(bool state)
{

}
