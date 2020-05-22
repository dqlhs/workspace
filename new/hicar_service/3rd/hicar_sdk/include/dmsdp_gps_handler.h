/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: dmsdp gps handler
 * Author: wangguangbing
 * Create: 2020-02-04
 */

#ifndef DMSDP_GPS_HANDLER_H
#define DMSDP_GPS_HANDLER_H

#include "dmsdp_business.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* stop call function(CarBaseInfo) when time sync received */
    GPS_STOP_CALLBACK_BASE_INFO = 0,
} GpsControlType;

typedef struct {
    /* callback location data */
    void (*GPSLocation)(char *location, int32_t len);

    /* callback driving data */
    void (*GPSDriving)(char *driving, int32_t len);

    /* callback sensor data */
    void (*GPSSensor)(char *sensor, int32_t len);

    /* callback car base info,modelId,deviceId,brand */
    void (*CarBaseInfo)(char *info, int32_t len);
}DMSDPGPSCallback;

typedef struct {
    /* DMSDP registers callback with 3thParty, start send gps data back to DMSDP */
    int32_t (*RegisterGPSDataCallback)(DMSDPGPSCallback* callback);

    /* stop sending data when this function is called */
    int32_t (*UnRegisterGPSDataCallback)();

    /* query report frequency from device */
    int32_t (*GetGPSDataReportFreq)(int32_t* locationFreq, int32_t* drivingFreq, int32_t* sensorFreq);

    /* set report frequency to device */
    int32_t (*SetGPSDataReportFreq)(int32_t locationFreq, int32_t drivingFreq, int32_t sensorFreq);

    /* listener for service status monitoring. Reserved interface */
    int32_t (*RegisterListener)(const DMSDPListener *listener);

    /* Data interaction between DMSDP and 3thParty */
    int32_t (*BusinessControl)(uint32_t cmd, char *inputPara, uint32_t inLen, char *outputPara, uint32_t outLen);
} DMSDPGpsHandler;

/* get gps handler interface */
int32_t DMSDPGetGpsHandler(DMSDPGpsHandler *gpsHandler);

#ifdef __cplusplus
}
#endif

#endif
