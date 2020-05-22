/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: dmsdp camera handler
 */

#ifndef DMSDP_CAMERA_HANDLER_H
#define DMSDP_CAMERA_HANDLER_H

#include "dmsdp_camera_data_type.h"
#include "dmsdp_business.h"

#ifdef __cplusplus
extern "C" {
#endif

/* function callback for send data back */
typedef int32_t (*DMSDPSendBackDataCB)(const char *id, uint32_t idLen, const uint8_t *buffer, uint32_t len,
    int32_t type);

typedef struct {
    /* DMSDP component will register send data back function pointer by this function */
    int32_t (*RegisterSendDataCallback)(const char *id, uint32_t idLen, const DMSDPSendBackDataCB cb);

    /* open camera */
    int32_t (*OpenCamera)(const DMSDPCameraParam *param);

    /* close camera by camera id */
    int32_t (*CloseCamera)(const char *id, uint32_t idLen);

    /*
     * update configs in running
     * camId: camera id
     * configs: configs need update
     *
     * supported updated configs, key for config key£¬value for config value£¬type for value data type:
     * key | value desc     | value type
     * ------------------------------
     * 1   | fps           | int
     */
    int32_t (*UpdateConfig)(const char *id, uint32_t idLen, int32_t key, char *value, uint32_t valLen);

    /* query camera capability */
    int32_t (*GetCameraCapability)(DMSDPCameraCapabilities **capabilities, uint32_t *num);

    /* query camera capability string */
    const char *(*GetCameraCapabilityString)(uint32_t *len);

    /*
     * release
     * type: DMSDPCameraReleaseType
     */
    void (*Release)(int32_t type, void *ptr, uint32_t num);

    /*
     * reg listener
     * listener: DMSDPLinstener
     */
    int32_t (*RegisterListener)(const DMSDPListener *listener);

    /*
     * business control
     * cmd: command DMSDPBusinessControlCmd
     * para: parameter
     * length: parameter length
     */
    int32_t (*BusinessControl)(uint32_t cmd, void *inputPara, uint32_t inLen, void *outputPara, uint32_t outLen);
} DMSDPCameraHandler;

/* get camera handler interface */
int32_t DMSDPGetCameraHandler(DMSDPCameraHandler *cameraHandler);

#ifdef __cplusplus
}
#endif

#endif
