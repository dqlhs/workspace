A   hicar_demo.c D:\Project\Linux车机\接口文档\hicar_demo.c    2   C:\Users\Y00499~1\AppData\Local\Temp\hicar_demo.c �  #include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hicar.h"
#include "securec.h"

volatile sig_atomic_t flag = 0;

static void my_handler(int sig) {
    flag = 1;
}

// Callback
void GetHiCarHandle(HICAR_HANDLE handle)
{
    printf("GetHiCarHandle\n");
}

void DeviceChange(const char *deviceId, uint32_t idLen, int state, int errorCode)
{
    printf("DeviceChange\n");
}

void DeviceServiceChange(const char *serviceId, uint32_t idLen, int state)
{
    printf("DeviceServiceChange\n");
}

void DataReceive(const char *deviceId, uint32_t idLen, int dataType, char *data, uint32_t dataLen)
{
    printf("DataReceive\n");
}

int32_t IsAllowReconnect() {
    printf("IsAllowReconnect\n");
    return 1;
}

int32_t GetSecureFileSize(const char *filename)
{
    printf("GetSecureFileSize\n");
    return 0;
}

int32_t ReadSecureFile(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len)
{
    printf("ReadSecureFile\n");
    return 0;
}

int32_t WriteSecureFile(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len)
{
    printf("WriteSecureFile\n");
    return 0;
}

int32_t RemoveSecureFile(const char *filename)
{
    printf("RemoveSecureFile\n");
    return 0;
}

int32_t GetVideoData(const uint8_t *pPacket, uint32_t nPktSize, int64_t timeStamp, int64_t duration, VideoFrameInfo *info)
{
    printf("GetVideoData\n");
    return 0;
}

void LogDebug(const char *tag, const char *func_name, const char *format, ...)
{

}

void LogInfo(const char *tag, const char *func_name, const char *format, ...)
{

}

void LogWarning(const char *tag, const char *func_name, const char *format, ...)
{

}

void LogError(const char *tag, const char *func_name, const char *format, ...)
{

}

int main()
{
    printf("Hello Linux HiCar Demo!\n");
    signal(SIGINT, my_handler);

    CarConfig config;
    
    config.deviceInfo.modelId[0] = '0';
    config.deviceInfo.modelId[1] = '1';
    config.deviceInfo.modelId[2] = '4';
    config.deviceInfo.modelId[3] = '1';

    memset_s(&(config.deviceInfo.brMac[0]), BR_MAC_MAX_LEN, 0, BR_MAC_MAX_LEN);
    memcpy_s(&(config.deviceInfo.brMac[0]), BR_MAC_MAX_LEN, "AA:BB:CC:DD:EE:FF", 17);

    config.deviceInfo.connectAllowType = 0x3;
    config.deviceInfo.carDeviceId = "BMW-7-2019-ADJBJDEKEKDE";
    config.deviceInfo.idLen = strlen(config.deviceInfo.carDeviceId);

    config.displayCapability.codecs = 1;
    config.displayCapability.fps = 30;
    config.displayCapability.gop = -1;
    config.displayCapability.bitrate = 5*1000*1000;
    config.displayCapability.minBitrate = 3*1000*1000;
    config.displayCapability.maxBitrate = 10*1000*1000;
    config.displayCapability.dpi = 0;
    config.displayCapability.profile = 0;
    config.displayCapability.level = 0;
    config.displayCapability.screenWidth = 1920;
    config.displayCapability.screenHeight = 720;
    config.displayCapability.width = 1920;
    config.displayCapability.height = 720;

    printf("Call HiCarInit.\n");
    HiCarInit(config);

    CarListener carListener = {0};

    carListener.GetHiCarHandle = GetHiCarHandle;
    carListener.DeviceChange = DeviceChange;
    carListener.DeviceServiceChange = DeviceServiceChange;
    carListener.DataReceive = DataReceive;
    carListener.GetSecureFileSize = GetSecureFileSize;
    carListener.ReadSecureFile = ReadSecureFile;
    carListener.WriteSecureFile = WriteSecureFile;
    carListener.RemoveSecureFile = RemoveSecureFile;
    carListener.GetVideoData = GetVideoData;
    
    carListener.logGroup.logD = LogDebug;
    carListener.logGroup.logI = LogInfo;
    carListener.logGroup.logW = LogWarning;
    carListener.logGroup.logE = LogError;

    printf("Call HiCarRegisterListener.\n");
    HiCarRegisterListener(&carListener);

    printf("Call HiCarStartAdv.\n");
    HiCarStartAdv();
	
	// call HiCarPostEvent if user touch screen
	//HwRemoteCtrlEvent *obj = (HwRemoteCtrlEvent*)HiCarNewObject(g_hicarHandle, HICAR_OBJECT_REMOTE_CRTL, sizeof(HwRemoteCtrlEvent));
	// fill content of HwRemoteCtrlEvent
	//	...
	//HiCarPostEvent(g_hicarHandle, HICAR_EVENT_REMOTE_CTRL, (char*)obj, sizeof(*obj));
	//HiCarDeleteObject(g_hicarHandle, HICAR_OBJECT_REMOTE_CRTL, (void*)obj);

    // Exit 
	//HiCarRelease();

    while (1) 