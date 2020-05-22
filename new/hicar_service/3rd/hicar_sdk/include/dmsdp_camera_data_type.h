/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: dmsdp camera type
 */

#ifndef DMSDP_CAMERA_DATA_TYPE_H
#define DMSDP_CAMERA_DATA_TYPE_H

#include <stdint.h>

/* Camera parameter for operate camera */
typedef struct {
    /* id of camera */
    const char *id;
    uint32_t idLen;

    /* frame width */
    uint32_t width;

    /* frame height */
    uint32_t height;

    /* fps 0: means use default fps */
    uint32_t fps;

    /* DMSDPCameraDataType */
    uint32_t dataFormat;
} DMSDPCameraParam;

/*
 * fps must multiply by 1000, such as android.
 * For example, the min fps is 20 per seconds, max is 30 per seconds,
 * the struct should be {20000, 30000}
 */
typedef struct {
    uint32_t min;
    uint32_t max;
} DMSDPFpsRange;

typedef struct {
    uint32_t width;
    uint32_t height;
} DMSDPSize;

typedef enum {
    H264 = 1,
    H265 = 2,
    RGB565 = 4,
    NV21 = 17
} DMSDPCameraDataType;

typedef enum {
    RELEASE_TYPE_QUERY_ABILITY = 1
} DMSDPCameraReleaseType;

typedef enum {
    FPS = 1
} DSMDPCameraConfigKey;

typedef struct {
    /*
     * supported fps ranges, each item has min and max value, ref:
     * https://developer.android.com/reference/android/hardware/Camera.Parameters.html#getSupportedPreviewFpsRange()
     */
    DMSDPFpsRange *supportFpsRanges;

    /* the number of support fps ranges */
    uint32_t fpsRangeNum;

    /* supported resolution sizes */
    DMSDPSize *supportSizes;
    /* the number of supported resolution sizes */
    uint32_t sizeNum;

    /* camera data type */
    DMSDPCameraDataType dataType;
} DMSDPCameraAbility;

typedef struct {
    char *key;
    uint32_t keyLen;
    char *value;
    uint32_t valueLen;
} DMSDPExtendInfo;

typedef struct {
    DMSDPExtendInfo *infos;
    uint32_t num;
} DMSDPExtendInfos;

typedef struct {
    /* range: 8-256 */
    char *id;
    uint32_t idLen;
    DMSDPCameraAbility *abilities;
    /*
     * the number of supported frame abilityes,
     * such as NV21 and RGB565 with different fps or
     * resolution, it will be 2 ability
     */
    uint32_t abilityNum;
    DMSDPExtendInfos extendInfos;
} DMSDPCameraCapabilities;

#endif
