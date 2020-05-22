#include "hicarlinkImpl.h"
#include "log/oxygen_log.h"
#include "RrConfig.h"
#include "stdarg.h"
#include "unique_config.h"

using namespace hsae;
using namespace oxygen;

#define HICHAIN_FILE_PATH "/home/root"
#define LOG_BUFFER_LEN (1024*5)

HICAR_HANDLE g_hicarHandle = NULL;
char *g_logBuffer = NULL;
hicarListener* m_listener;
bool m_firstVideoData;
bool m_hicarVideoState;//sdk记录hicar视频开关状态

void GetHiCarHandleCB(HICAR_HANDLE handle) {
    if (handle != NULL) {
        INFO("GetHiCarHandleCB");
        g_hicarHandle = handle;
    }
}

void DeviceChangeCB(const char* deviceId, uint32_t idLen, int state, int errorCode) {
    INFO("hicarlinkImpl::DeviceChangeCB : %d", state);
    if(state == 101)
    {
        m_hicarVideoState = true;
    }
    else if(state == 106 || state == 110)
    {
        m_hicarVideoState = false;
        if(m_firstVideoData)
        {
            if(m_listener != NULL)
            {
                m_listener->recvVideoState(false);
            }
            m_firstVideoData = false;
        }
    }
    if(m_listener != NULL)
    {
        m_listener->recvConnectState(state);
    }
}

void DeviceServiceChangeCB(const char* serviceId, uint32_t idLen, int state) {
    INFO("hicarlinkImpl::serviceId : %s ,DeviceServiceChangeCB : %d", serviceId , state);
    if(m_listener != NULL)
    {
        m_listener->recvServerState(state);
    }
//    if (serviceId != NULL && strcmp(serviceId, "display1") == 0) {
//        if (state == HICAR_DEVICE_SERVICE_PAUSE) {
//            // HiCar HMI exit to backgroud
//        } else if (state == HICAR_DEVICE_SERVICE_RESUME) {
//            // HiCar HMI jump to foreground from backgroud
//        }
//    }

//    switch (state) {
//        case 202:
//            if (strcmp(serviceId, "display1") == 0) {
//                HiCarPauseProjection(g_hicarHandle);
//            }
//        break;
//        case 203:
//            if (strcmp(serviceId, "display1") == 0) {
//                HiCarStartProjection(g_hicarHandle);
//            }
//        break;
//        // another service and different state handle
//    }

    return;
}

void DataReceiveCB(const char* deviceId, uint32_t idLen, int dataType, char* data, uint32_t dataLen) {

    return;
}

int32_t GetSecureFileSizeCB(const char* filename) {
    FILE* fp;
    int32_t size = 0;

    if (filename == NULL) {
        INFO("GetSecureFileSize param null");
        return -1;
    }

    INFO("Enter GetSecureFileSize, fileName:%s", filename);

    char filenameInBox[256] = {0};
    sprintf(filenameInBox, "%s/%s", HICHAIN_FILE_PATH, filename);
    fp = fopen(filenameInBox, "rb");
    if (fp == NULL) {
        INFO("fopen null");
        return 0;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        INFO("fseek error");
        fclose(fp);
        return -1;
    }

    size = ftell(fp);
    fclose(fp);

    INFO("Leave GetSecureFileSize, size:%d", size);
    return size;
}

int32_t ReadSecureFileCB(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len) {
    FILE* fp;
    int32_t size = 0;

    (void)offset;

    if (filename == NULL || buf == NULL) {
        INFO("ReadSecureFile param null");
        return -1;
    }

    INFO("ReadSecureFile, fileName:%s, len:%d\n", filename, len);

    char filenameInBox[256] = {0};
    sprintf(filenameInBox, "%s/%s", HICHAIN_FILE_PATH, filename);
    fp = fopen(filenameInBox, "rb");
    if (fp == NULL) {
        INFO("fopen null");
        return 0;
    }

    size = fread(buf, sizeof(uint8_t), (size_t)len, fp);
    fclose(fp);

    INFO("ReadSecureFile, size:%d\n", size);

    return size;
}

int32_t WriteSecureFileCB(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len) {
    (void)offset;
    FILE* fp;
    size_t size;

    if (filename == NULL || buf == NULL) {
        INFO("WriteSecureFile param null.\n");
        return -1;
    }

    INFO("WriteSecureFile, filename:%s, len:%d\n", filename, len);

    char filenameInBox[256] = {0};
    sprintf(filenameInBox, "%s/%s", HICHAIN_FILE_PATH, filename);
    fp = fopen(filenameInBox, "wb");
    if (fp == NULL) {
        INFO("fopen null");
        return 0;
    }

    size = fwrite(buf, sizeof(uint8_t), (uint32_t)len, fp);
    fflush(fp);
    fclose(fp);

    INFO("WriteSecureFile, size:%d", size);

    if (size != len) {
        INFO("WriteSecureFile error");
        return -1;
    }

    return 0;
}

int32_t RemoveSecureFileCB(const char* filename) {
    INFO("RemoveSecureFile");
    return 0;
}

int32_t GetVideoDataCB(const uint8_t *pPacket, uint32_t nPktSize, int64_t timeStamp, int64_t duration, VideoFrameInfo *info) {
//    INFO("GetVideoDataCB");
    if(m_hicarVideoState && !m_firstVideoData)
    {
        if(m_listener != NULL)
        {
            m_listener->recvVideoState(true);
        }
        m_firstVideoData = true;
    }
    handleVideoData(pPacket,nPktSize,timeStamp,duration,info);
    return 0;
}

void CarLogDebugCB(const char* tag, const char* funcName, const char* format, ...) {
    uint32_t uResult = 0;
    uint32_t len = 0;
    va_list stVaList;
    va_start(stVaList, format);
    memset(g_logBuffer, 0, LOG_BUFFER_LEN);

    uResult = sprintf(g_logBuffer, "[%s-%s-%s]", tag, funcName, "Debug");
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }

    len += uResult;
    uResult = vsprintf(g_logBuffer + len, (const char *)format, stVaList);
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }
    INFO("%s", g_logBuffer);
    va_end(stVaList);
    return;
}

void CarLogInfoCB(const char* tag, const char* funcName, const char* format, ...) {
    uint32_t uResult = 0;
    uint32_t len = 0;
    va_list stVaList;
    va_start(stVaList, format);
    memset(g_logBuffer, 0, LOG_BUFFER_LEN);

    uResult = sprintf(g_logBuffer, "[%s-%s-%s]", tag, funcName, "Info");
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }

    len += uResult;
    uResult = vsprintf(g_logBuffer + len, (const char *)format, stVaList);
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }
    INFO("%s", g_logBuffer);
    va_end(stVaList);
    return;
}

void CarLogWarningCB(const char* tag, const char* funcName, const char* format, ...) {
    uint32_t uResult = 0;
    uint32_t len = 0;
    va_list stVaList;
    va_start(stVaList, format);
    memset(g_logBuffer, 0, LOG_BUFFER_LEN);

    uResult = sprintf(g_logBuffer, "[%s-%s-%s]", tag, funcName, "Warn");
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }

    len += uResult;
    uResult = vsprintf(g_logBuffer + len, (const char *)format, stVaList);
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }
    WARN("%s", g_logBuffer);
    va_end(stVaList);
    return;
}

void CarLogErrorCB(const char* tag, const char* funcName, const char* format, ...) {
    uint32_t uResult = 0;
    uint32_t len = 0;
    va_list stVaList;
    va_start(stVaList, format);
    memset(g_logBuffer, 0, LOG_BUFFER_LEN);

    uResult = sprintf(g_logBuffer, "[%s-%s-%s]", tag, funcName, "Error");
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }

    len += uResult;
    uResult = vsprintf(g_logBuffer + len, (const char *)format, stVaList);
    if (uResult < 0) {
        va_end(stVaList);
        return;
    }
    ERROR("%s", g_logBuffer);
    va_end(stVaList);
    return;
}

int32_t IsAllowReconnect() {
    INFO("IsAllowReconnect");
    return 1;
}

void setRemoteCtrlTouchEvent(HwRemoteCtrlEvent *obj)
{
    obj->header.version = 0;
    obj->header.dFlag = 1;
    obj->header.wFlag = 0;
    obj->header.wr1Flag = 0;
    obj->header.wr2Flag = 0;
    obj->header.rFlag = 0;
    obj->header.inputCategory = 0;
    obj->header.len = 10;
    obj->evtType = REMOTECTRL_TOUCH;
}

void setRemoteCtrlKeyEvent(HwRemoteCtrlEvent *obj)
{
    obj->header.version = 0;
    obj->header.dFlag = 1;
    obj->header.wFlag = 0;
    obj->header.wr1Flag = 0;
    obj->header.wr2Flag = 0;
    obj->header.rFlag = 0;
    obj->header.inputCategory = 0;
    obj->header.len = 10;
    obj->evtType = REMOTECTRL_KEY;
}

//void PinCodeCB(const char* pinCode, uint32_t codeLen) {
//    // notify UI thread to show
//    return;
//}

//int32_t IsAllowReconnectCB() {
//    return 1;
//}

hicarlinkImpl::hicarlinkImpl()
{
    m_firstVideoData = false;
    m_hicarVideoState = false;

    INFO("g_fcAdaptBleOps name %s",g_fcAdaptBleOps.name);
    INFO("g_fcDriverBrOps name %s",g_fcDriverBrOps.name);
    INFO("g_fcDriverWifiOps name %s",g_fcDriverWifiOps.name);
    INFO("g_fcDriverUsbOps name %s",g_fcDriverUsbOps.name);
}

hicarlinkImpl::~hicarlinkImpl()
{
    HiCarRelease();
}

void hicarlinkImpl::initHicar()
{
    INFO("Call HiCarInit.");
    g_logBuffer = (char*)malloc(sizeof(char)*LOG_BUFFER_LEN);
    unique_config & u_config = unique_config::instance();

    CarConfig hiarconfig = u_config.hicarconfig();
    INFO("hicarconfig: %s",hiarconfig.connectInfo.modelId);
    CarLogFuncGroup logGroup;
    logGroup.logD = CarLogDebugCB;
    logGroup.logI = CarLogInfoCB;
    logGroup.logW = CarLogWarningCB;
    logGroup.logE = CarLogErrorCB;
    HiCarInit(&hiarconfig,&logGroup);

    CarListener carListener = {0};

    carListener.GetHiCarHandle = GetHiCarHandleCB;
    carListener.DeviceChange = DeviceChangeCB;
    carListener.DeviceServiceChange = DeviceServiceChangeCB;
    carListener.DataReceive = DataReceiveCB;
    carListener.GetSecureFileSize = GetSecureFileSizeCB;
    carListener.ReadSecureFile = ReadSecureFileCB;
    carListener.WriteSecureFile = WriteSecureFileCB;
    carListener.RemoveSecureFile = RemoveSecureFileCB;
    carListener.GetVideoData = GetVideoDataCB;

    INFO("Call HiCarRegisterListener.");
    HiCarRegisterListener(&carListener);

    initPlayer();
}

void hicarlinkImpl::addListener(hicarListener * plistener)
{
    INFO("addListener");
    if(plistener != NULL)
    {
         m_listener = plistener;
    }
}

void hicarlinkImpl::startAdv()
{
    INFO("startAdv");
    char t_pinCode[8] = {0};
    if(HiCarStartAdv() == 0)
    {
        INFO("HiCarGetPinCode: %d",(int)HiCarGetPinCode((uint8_t*)t_pinCode,sizeof(t_pinCode)));
        string s = t_pinCode;
        if(m_listener != NULL)
            m_listener->recvPinCode(s);
    }
    else
    {
        ERROR("startAdv failed!");
    }
}

void hicarlinkImpl::restartAdv()
{
    INFO("restartAdv");
    char t_pinCode[8] = {0};
    if(HiCarStartAdv() == 0)
    {
        INFO("HiCarRestartGetPinCode: %d",(int)HiCarGetPinCode((uint8_t*)t_pinCode,sizeof(t_pinCode)));
        string s = t_pinCode;
        if(m_listener != NULL)
            m_listener->recvPinCode(s);
    }
    else
    {
        ERROR("startAdv failed!");
    }
}

void hicarlinkImpl::autoConnect(std::string btMac)
{
    INFO("autoConnect mac: %s ,result: %d", btMac.c_str() ,HiCarReconnect(btMac.c_str(),17));
}

void hicarlinkImpl::startProjection()
{
    INFO("startProjection");
    HiCarStartProjection(g_hicarHandle);
}

void hicarlinkImpl::stopProjection()
{
    INFO("stopProjection");
    HiCarStopProjection(g_hicarHandle);
}

void hicarlinkImpl::pauseProjection()
{
    INFO("pauseProjection");
    HiCarPauseProjection(g_hicarHandle);
}

void hicarlinkImpl::voiceWakeupRequest(string hotWord)
{
    char* t_hotWord = const_cast<char*>(hotWord.c_str());  // json格式字符串 语音唤醒命令
    int t_hotWordSize = strlen(t_hotWord);
    if(NULL != g_hicarHandle)
    {
      HiCarPostEvent(g_hicarHandle, HICAR_EVENT_VOICE_WAKEUP, t_hotWord, t_hotWordSize);
    }
}

void hicarlinkImpl::sendTouchEvent(HwRemoteCtrlTouchEventDetails touchEvt)
{
    // call HiCarPostEvent if user touch screen
    if(NULL != g_hicarHandle)
    {
        HwRemoteCtrlEvent *obj = (HwRemoteCtrlEvent*)HiCarNewObject(g_hicarHandle, HICAR_OBJECT_REMOTE_CTRL, sizeof(HwRemoteCtrlEvent));
        // fill content of HwRemoteCtrlEvent
        setRemoteCtrlTouchEvent(obj);
        obj->evtDetail.touchEvt = touchEvt;
        HiCarPostEvent(g_hicarHandle, HICAR_EVENT_REMOTE_CTRL, (char*)obj, sizeof(*obj));
        //    HiCarDeleteObject(g_hicarHandle, HICAR_OBJECT_REMOTE_CTRL, (void*)obj);
    }
}

void hicarlinkImpl::sendKeyEvent(HwRemoteCtrlKeyEventType eventType,HwRemoteCtrlKeyCode keyCode)
{
    INFO("sendKeyEvent eventType: [%d],keyCode: [%d]",eventType,keyCode);
    if(NULL != g_hicarHandle)
    {
        HwRemoteCtrlEvent *obj = (HwRemoteCtrlEvent*)HiCarNewObject(g_hicarHandle, HICAR_OBJECT_REMOTE_CTRL, sizeof(HwRemoteCtrlEvent));
        // fill content of HwRemoteCtrlEvent
        setRemoteCtrlKeyEvent(obj);
        HwRemoteCtrlKeyEventDetails touchEvt;
        touchEvt.type = eventType;
        touchEvt.keyCode1 = keyCode;
        touchEvt.keyCode2 = 0;
        touchEvt.metaState = 0;

        obj->evtDetail.keyEvt = touchEvt;
        HiCarPostEvent(g_hicarHandle, HICAR_EVENT_REMOTE_CTRL, (char*)obj, sizeof(*obj));
    }
}

