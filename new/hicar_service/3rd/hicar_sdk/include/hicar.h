/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: hicar header
 * Author: yangchao
 * Create: 2019-11-27
 */

#ifndef HICAR_H
#define HICAR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIB_HICAR_API __attribute__((visibility("default")))
#define HICAR_HANDLE void*

#define PHONE_ID_MAX_LEN 256
#define PHONE_DATA_MAX_LEN 256
#define PHONE_NAME_MAX_LEN 128
#define PHONE_TRUST_MAX_LIST 16
#define PHONE_SESSION_KEY_LEN 128
#define MODEL_ID_MAX_LEN 8
#define BR_MAC_MAX_LEN 20
#define CAR_DAY_NIGHT_MAX_LEN 12
#define CAR_BRAND_MAX_LEN 64
#define USB_CHANNEL_IP_STR_LEN 16

typedef enum {
    HICAR_DEVICE_CONNECTED         = 101,
    HICAR_DEVICE_CONNECTING        = 102,
    HICAR_DEVICE_DISCONNECTED      = 103,
    HICAR_DEVICE_DISCONNECTING     = 104,
    HICAR_DEVICE_NO_DATA           = 105,
    HICAR_DEVICE_DETECT_DISCONNECT = 106,
    HICAR_DEVICE_NET_UNSTABLE_MILD = 107,
    HICAR_DEVICE_TCP_CONNECTED     = 108,
    HICAR_DEVICE_CONNECT_TIMEOUT_FOR_REPORT = 109,
    HICAR_DEVICE_MANUAL_DISCONNECT = 110,
    HICAR_DEVICE_ADV_TIMEOUT       = 111,

    HICAR_DEVICE_SERVICE_UPDATE    = 201,
    HICAR_DEVICE_SERVICE_PAUSE     = 202,
    HICAR_DEVICE_SERVICE_RESUME    = 203,
    HICAR_DEVICE_SERVICE_START     = 205,
    HICAR_DEVICE_SERVICE_STOP      = 206,
} HiCarEvent;

typedef enum {
    HICAR_DRIVING_POS_LEFT = 0,
    HICAR_DRIVING_POS_RIGHT = 1,
} CarDrivingPosition;

typedef enum {
    HICAR_OBJECT_REMOTE_CTRL = 1,
} CarObjectType;

typedef enum {
    HICAR_EVENT_REMOTE_CTRL = 1,
    HICAR_EVENT_VOICE_WAKEUP,
    HICAR_EVENT_REQUEST_IDRFRAME,
} CarEventType;

typedef enum {
    HICAR_DATA_UNKNOWN = 0,
    HICAR_DATA_VEHICLE_CONTROL   = 500,
    HICAR_DATA_DAY_NIGHT         = 501,
    HICAR_DATA_BRAND_ICON_DATA   = 502,
    HICAR_DATA_NAV_FOCUS         = 503,
    HICAR_DATA_CALL_STATE_FOCUS  = 504,
    HICAR_DATA_VOICE_STATE       = 505,
    HICAR_DATA_DRIVING_MODE      = 506,
    HICAR_DATA_SENSOR_DATA       = 507,
    HICAR_DATA_SERVICE_CHANNEL   = 508,
    HICAR_DATA_PARKING_STATUS    = 509,
    HICAR_DATA_NAV_AA            = 510,
    HICAR_DATA_NAV_DATA          = 511,
} CarDataType;

typedef enum {
    HICAR_CONNECT_INVALID        = 0,
    /* 0000 0001, ble advertisement only */
    HICAR_CONNECT_ONLY_ADVERTISE = 1,
    /* 0000 0010, USB only */
    HICAR_CONNECT_ONLY_USB       = 2,
    /* 0000 0011, ble advertisement and USB */
    HICAR_CONNECT_ADVERTISE_USB  = 3,
    /* 0000 0100, scanning QR code only */
    HICAR_CONNECT_ONLY_QRCODE    = 4,
    /* 0000 0110, scanning QR code and USB */
    HICAR_CONNECT_QRCODE_USB     = 6,
} CarConnectAllowType;

typedef struct {
    char modelId[MODEL_ID_MAX_LEN];
    char brMac[BR_MAC_MAX_LEN];
    uint32_t connectAllowType;
    char *carDeviceId;
    uint32_t idLen;
} CarConnectInfo;

typedef struct {
    char carBrand[CAR_BRAND_MAX_LEN];
    uint32_t carBrandLen;
    char dayNightInfo[CAR_DAY_NIGHT_MAX_LEN];
    uint32_t carDayNightLen;

    uint32_t steeringWheelPos;
} CarDeviceInfo;

typedef struct {
    int32_t codecs;
    int32_t fps;
    int32_t gop;
    int32_t bitrate;
    int32_t minBitrate;
    int32_t maxBitrate;
    int32_t dpi;
    int32_t profile;
    int32_t level;
    int32_t screenWidth;
    int32_t screenHeight;
    int32_t width;
    int32_t height;
} CarDisplayCapability;

typedef struct {
    uint32_t length;
    uint8_t *value;
} DataInfo;

typedef struct {
    DataInfo carModel;
    DataInfo manufacturer;
    DataInfo model;
    DataInfo os;
    DataInfo cpu;
    DataInfo ram;
    DataInfo rom;
    DataInfo screenSize;
    DataInfo screenMetrics;
    DataInfo linuxKernel;
    DataInfo btChip;
    DataInfo wifiChip;
} CarFeatureInfo;

typedef struct {
    CarConnectInfo connectInfo;
    CarDeviceInfo deviceInfo;
    CarDisplayCapability displayCapability;
    CarFeatureInfo featureInfo;
} CarConfig;

typedef struct {
    uint16_t length;
    uint8_t screenRotation;
    uint8_t hasNavigationKey;
    char pack[1];
} VideoFrameInfo;

typedef struct {
    uint32_t phoneIdRealLen;
    uint8_t phoneId[PHONE_ID_MAX_LEN];
    uint8_t phoneNameRealLen;
    uint8_t phoneName[PHONE_NAME_MAX_LEN];
    int64_t lastConnectTime;
} TrustPhoneInfo;

typedef void (*CarLogFunc)(const char *tag, const char *funcName, const char *format, ...);

typedef struct {
    CarLogFunc logD;
    CarLogFunc logI;
    CarLogFunc logW;
    CarLogFunc logE;
} CarLogFuncGroup;

typedef struct {
    void (*GetHiCarHandle)(HICAR_HANDLE handle);
    void (*DeviceChange)(const char *deviceId, uint32_t idLen, int state, int errorCode);
    void (*DeviceServiceChange)(const char *serviceId, uint32_t idLen, int state);
    void (*DataReceive)(const char *deviceId, uint32_t idLen, int dataType, char *data, uint32_t dataLen);
    int32_t (*GetSecureFileSize)(const char *filename);
    int32_t (*ReadSecureFile)(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len);
    int32_t (*WriteSecureFile)(const char *filename, uint32_t offset, uint8_t *buf, uint32_t len);
    int32_t (*RemoveSecureFile)(const char *filename);
    int32_t (*GetVideoData)(const uint8_t *pPacket, uint32_t nPktSize, int64_t timeStamp,
                            int64_t duration, VideoFrameInfo *info);
} CarListener;

LIB_HICAR_API int32_t HiCarInit(CarConfig *carConfig, CarLogFuncGroup *logGroup);
LIB_HICAR_API int32_t HiCarRelease();

LIB_HICAR_API int32_t HiCarRegisterListener(CarListener *listener);

LIB_HICAR_API int32_t HiCarGetPinCode(uint8_t *pinCode, uint32_t maxLen);
LIB_HICAR_API int32_t HiCarReconnect(const char *bluetoothMac, uint32_t realLen);
LIB_HICAR_API int32_t HiCarGetConnectQRCode(uint8_t *url, uint32_t maxLen);

LIB_HICAR_API int32_t HiCarStartAdv();
LIB_HICAR_API int32_t HiCarStopAdv();

LIB_HICAR_API int32_t HiCarStartProjection(HICAR_HANDLE handle);
LIB_HICAR_API int32_t HiCarStopProjection(HICAR_HANDLE handle);
LIB_HICAR_API int32_t HiCarPauseProjection(HICAR_HANDLE handle);

LIB_HICAR_API void *HiCarNewObject(HICAR_HANDLE handle, CarObjectType objType, uint32_t size);
LIB_HICAR_API int32_t HiCarDeleteObject(HICAR_HANDLE handle, CarObjectType objType, void *obj);

LIB_HICAR_API int32_t HiCarPostEvent(HICAR_HANDLE handle, CarEventType eventType, const char *content, uint32_t size);
LIB_HICAR_API int32_t HiCarSendData(HICAR_HANDLE handle, CarDataType dataType, uint8_t *data, uint32_t size);

LIB_HICAR_API char *HiCarGetSdkVersion();

LIB_HICAR_API int32_t HiCarGetTrustDeviceList(HICAR_HANDLE handle, TrustPhoneInfo **list, int32_t listLen);
LIB_HICAR_API int32_t HiCarDeleteTrustDevice(HICAR_HANDLE handle, const char *phoneId, uint32_t idLen);

#ifdef __cplusplus
}
#endif


#endif
