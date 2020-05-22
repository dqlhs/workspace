/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: ALog header file
 * Author: yangleigang
 * Create: 2019-04-18
 */

#ifndef DMSDP_LOG_H
#define DMSDP_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    DMSDP_SDK_DBG_LEVEL_ERROR = 1,
    DMSDP_SDK_DBG_LEVEL_WARNING = 2,
    DMSDP_SDK_DBG_LEVEL_INFO = 3,
    DMSDP_SDK_DBG_LEVEL_DGB = 4
} DMSDPSdkDbgLevel;

#define DMSDP_SDK_DEBUG_BUFF_LEN  (1024)

void DmsdpSdkLog(uint8_t level, const char* tag, const char *format, ...);

#define LOGE(...) DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, LOG_TAG, __VA_ARGS__);
#define LOGW(...) DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, LOG_TAG, __VA_ARGS__);
#define LOGI(...) DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, LOG_TAG, __VA_ARGS__);
#define LOGD(...) DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_DGB, LOG_TAG, __VA_ARGS__);

#ifdef __cplusplus
}
#endif

#endif
