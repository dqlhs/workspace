/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: dmsdp business
 */

#ifndef DMSDP_BUSINESS_H
#define DMSDP_BUSINESS_H

#include <stdint.h>
#include "dmsdp_errno.h"

#define DMSDP_SDK_MAKE_VERSION(major, sub, patch, \
    beta) ((((major)&0xffffULL) << 48) | (((sub)&0xffffULL) << 32) | (((patch)&0xffffULL) << 16) | ((beta)&0xffffULL))

#define DMSDP_SDK_CURRENT_API_VERSION DMSDP_SDK_MAKE_VERSION(1, 0, 0, 0)

#define DMSDP_SDK_VERSION_MAJOR(version) (uint16_t)(((uint64_t)(version) >> 48) & 0xffffULL)
#define DMSDP_SDK_VERSION_SUB(version) (uint16_t)(((uint64_t)(version) >> 32) & 0xffffULL)
#define DMSDP_SDK_VERSION_PATCH(version) (uint16_t)(((uint64_t)(version) >> 16) & 0xffffULL)
#define DMSDP_SDK_VERSION_BETA(version) (uint16_t)((uint64_t)(version)&0xffffULL)

typedef enum {
    MSDP_BUSINESS_CTRL_RESERVED = 0
} DMSDPBusinessControlCmd;

typedef enum {
    PLUGIN = 1,
    UNPLUG,
    AVAILABLE,
    UNAVAILABLE
} DMSDPServiceStatus;

typedef enum {
    /* audio focus change */
    AUDIO_FOCUS_CHANGE = 1,
    /* add camera service info, using DMSDPCameraCapabilities struct */
    CAMERA_ADD_SERVICE = 2,
    /* add camera service info, using DMSDPCameraCapabilities struct json string */
    CAMERA_ADD_SERVICE_STRING = 3,
    /* add audio service info, using DMSDPAudioCapabilities struct */
    AUDIO_ADD_SERVICE = 4,
    /* add audio service info, using DMSDPAudioCapabilities struct json string */
    AUDIO_ADD_SERVICE_STRING = 5,
    /* device send common data to application */
    DEVICE_SEND_COMMON_DATA = 6,
    /* notify the hardware state change notify, such as the Speaker Driver broken */
    DEVICE_HARDWARE_STATE_NOTIFY = 7
} DMSDPServcieActionType;

typedef enum {
    DATA_TYPE_CAR_CTRL_CMD = 1,
    DATA_TYPE_HOTWORD = 2,
    DATA_TYPE_KEY_EVENT = 3,
    DATA_TYPE_HISIGHT_MOTION_EVENT = 4,
    DATA_TYPE_HISIGHT_KEY_EVENT = 5,
    DATA_TYPE_MSDP_PORT = 6,
    DATA_TYPE_AIRSHARING_PORT = 7,
    DATA_TYPE_AIRSHARING_INPUT = 8,
    DATA_TYPE_AIRSHARING_HISIGHT_EXT_WINI_NFO = 9,
    DATA_TYPE_VIRTUAl_KEY_EVENT = 10,
    DATA_TYPE_INPUT_DEVICE_EVNET = 11,
    DATA_TYPE_AUDIO_NET_UNSTABLE = 12,
    DATA_TYPE_DISPLAY_NET_UNSTABLE = 13,
    DATA_TYPE_AIRSHARING_INPUT_FOUCS = 14,
    DATA_TYPE_SCREEN_EVENT = 15,
    DATA_TYPE_DEVICE_POLICY_EVENT = 16,
    DATA_TYPE_TV_NETWORK_WEAK = 20,
} DMSDPDeviceDataType;

typedef enum {
    DMSDP_DV_DISCONNECT = 1,
    DMSDP_DV_AUDIO_MUTE = 2,
    DMSDP_DV_AUDIO_LATENCY = 3,
    DMSDP_DV_SCREEN_ON = 4,
    DMSDP_DV_SCREEN_OFF = 5,
    DMSDP_DV_AUDIO_DYNAMIC_SYNC = 6,
} DMSDPDeviceEventType;

typedef struct {
    int32_t dataType;
    char *data;
    uint32_t dataLen;
    int32_t extendInfo;
} DMSDPDeviceCommonData;

typedef struct {
    DMSDPServcieActionType type;
    void *value;
    uint32_t valLen;
} DMSDPServiceAction;

typedef struct {
    int32_t (*UpdateServiceStatus)(const char *id, uint32_t idLen, const DMSDPServiceStatus status);
    int32_t (*UpdateServiceAction)(const char *id, uint32_t idLen, DMSDPServiceAction *action);
} DMSDPListener;

typedef struct {
    char *key;
    char *valuestring;
} DMSDPPropItem;

typedef struct tag_DMSDPProps {
    int32_t nums;
    DMSDPPropItem* items;
    int32_t (*SetProp)(int32_t index, const char *key, const char *value, struct tag_DMSDPProps *props);
} DMSDPProps;

#endif
