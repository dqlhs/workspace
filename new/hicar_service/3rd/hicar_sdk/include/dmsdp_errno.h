/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: dmsdp errno
 * Author: dongyin
 * Create: 2019-11-20
 */

#ifndef DMSDP_ERRNO_H
#define DMSDP_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DMSDP_OK = 0,
    DMSDP_ERR_UNKNOWN_ERROR = -1,
    DMSDP_ERR_INVALID_PARAMETER = -2,
    DMSDP_ERR_NO_MEMORY = -3,
    DMSDP_ERR_INVALID_OPERATION = -4,
    DMSDP_ERR_BAD_VALUE = -5,
    DMSDP_ERR_BAD_TYPE = -6,
    DMSDP_ERR_ENTRY_EXIST = -7,
    DMSDP_ERR_ENTRY_NOEXIST = -8,
    DMSDP_ERR_MEMORY_OPER_FAIL = -9,
    DMSDP_ERR_NOTSUPPORT = -10,
    DMSDP_ERR_BUSY_RELEASING = -11,
    DMSDP_ERR_INIT_FAIL = -12,
    DMSDP_ERR_NULLPTR = -13,
    DMSDP_ERR_SESSION_EXIST = -14,

    /* dmsdp camera error */
    DMSDP_ERR_CAMERA_BASE = -1000,

    /* Open camera failed */
    DMSDP_ERR_CAMERA_OPEN_ERROR = -1001,

    /* Close camera failed */
    DMSDP_ERR_CAMERA_CLOSE_ERROR = -1002,

    /* capture preview frame error */
    DMSDP_ERR_CAMERA_CAPTURE_ERROR = -1003,

    /* config camera error, such as set the fps */
    DMSDP_ERR_CAMERA_CONFIG_ERROR = -1004,

    /* control camera error, such as control camera zoom and movement */
    DMSDP_ERR_CAMERA_CONTROL_ERROR = -1005,

    /* dmsdp audio error */
    DMSDP_ERR_AUDIO_BASE = -2000,

    /* dmsdp gps error */
    DMSDP_ERR_GPS_BASE = -3000,
    DMSDP_ERR_GPS_DATA_INVALID_FREQ_PARA = -3001,

    /* dmsdp display error */
    DMSDP_ERR_DISLAY_BASE = -4000,

    /* dmsdp remote control error */
    DMSDP_ERR_REMOTECONTROL_BASE = -5000,

    /* dmsdp codec error */
    DMSDP_ERR_AENC_INVALID_ABILITY_BUFFER_COUNT = -6001,
    DMSDP_ERR_AENC_INVALID_CAHNNEL = -6002,
    DMSDP_ERR_AENC_UNSUPPORT_CODEC = -6003,
    DMSDP_ERR_AENC_UNSUPPORT_PCM_STORE_TYPE = -6004,
    DMSDP_ERR_AENC_UNSUPPORT_SAMPLE_DEPTH = -6005,
    DMSDP_ERR_AENC_UNSUPPORT_SAMPLE_RATE = -6006,
    DMSDP_ERR_AENC_INVALID_BITRATE = -6007,

    DMSDP_ERR_ADEC_INVALID_ABILITY_BUFFER_COUNT = -7001,
    DMSDP_ERR_ADEC_INVALID_CAHNNEL = -7002,
    DMSDP_ERR_ADEC_UNSUPPORT_CODEC = -7003,
    DMSDP_ERR_ADEC_UNSUPPORT_PCM_STORE_TYPE = -7004,
    DMSDP_ERR_ADEC_UNSUPPORT_SAMPLE_DEPTH = -7005,
    DMSDP_ERR_ADEC_UNSUPPORT_SAMPLE_RATE = -7006,
    DMSDP_ERR_ADEC_INVALID_BITRATE = -7007,

    DMSDP_ERR_VENC_INVALID_ABILITY_BUFFER_COUNT = -8001,
    DMSDP_ERR_VENC_UNSUPPORT_CODEC = -8002,
    DMSDP_ERR_VENC_UNSUPPORT_RESOLUTION = -8003,
    DMSDP_ERR_VENC_UNSUPPORT_PROFILE = -8004,
    DMSDP_ERR_VENC_UNSUPPORT_COLOR_FORMAT = -8005,
    DMSDP_ERR_VENC_INVALID_BITRATE = -8006,
    DMSDP_ERR_VENC_TIMESTAMP_INVALID = -8007,
} DMSDPErrorCode;

#ifdef __cplusplus
}
#endif

#endif
