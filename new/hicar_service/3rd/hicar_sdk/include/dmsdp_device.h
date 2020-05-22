/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: hilink dmsdp device
 */

#ifndef DMSDP_DEVICE_H
#define DMSDP_DEVICE_H

#include <stdint.h>
#include "dmsdp_business.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DMSDP_UNAVAILABLE = 0,
    DMSDP_AVAILABLE = 1
} DMSDPState;

typedef enum {
    DEVICE_TYPE_NONE = 0,
    DEVICE_TYPE_PHONE = 1,
    DEVICE_TYPE_PAD = 2,
    DEVICE_TYPE_TV = 3,
    DEVICE_TYPE_CAR = 4,
    DEVICE_TYPE_CAMERA = 5,
    DEVICE_TYPE_VOICEBOX = 6,
    DEVICE_TYPE_PC = 7,
    DEVICE_TYPE_CAMERA_BLE_P2P = 51
} DMSDPDeviceType;

typedef enum {
    SERVICE_TYPE_NONE = 0,
    SERVICE_TYPE_CAMERA = 0x01,
    SERVICE_TYPE_MIC = 0x02,
    SERVICE_TYPE_SPEAKER = 0x04,
    SERVICE_TYPE_DISPLAY = 0x08,
    SERVICE_TYPE_GPS = 0x10,
    SERVICE_TYPE_BUTTON = 0x20
} DMSDPServiceType;

typedef enum {
    SERVICE_UNAVAILABLE = 0,
    SERVICE_AVAILABLE = 1
} DMSDPServiceState;

/*
 * get dmsdp state
 * state: DMSDPState
 */
int32_t DMSDPGetState(uint32_t *state);

/*
 * get dmsdp device type
 * type: DMSDPDeviceType
 */
int32_t DMSDPGetDeviceType(uint32_t *type);

/*
 * get dmsdp deivce service capability
 * serviceMask: DMSDPServiceType mask, each bit specify a service type
 * for example : if support MIC and Speaker, mask will be SERVICE_TYPE_MIC | SERVICE_TYPE_SPEAKER
 *
 * stateMask: DMSDPServiceState mask, each 2bit specify a service state match to the service type mask
 * for example : if support MIC and Speaker, mask will be SERVICE_TYPE_MIC | SERVICE_TYPE_SPEAKER,
 * if MIC state is AVAILABLE and speaker is AVAILABLE, the the state mask will be
 * 0000 0000 0000 0000 0000 0000 0001 0100 : 0x00000014
 */
int32_t DMSDPGetDeviceServiceCapability(uint32_t *serviceMask, uint32_t *stateMask);

#ifndef GV_DEVICE
/*
 * register listener
 * listener: DMSDPListener
*/
int32_t DMSDPDeviceRegisterListener(const DMSDPListener *listener);
#endif

#ifdef __cplusplus
}
#endif

#endif
