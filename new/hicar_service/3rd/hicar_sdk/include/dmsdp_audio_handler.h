/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: dmsdp audio handler
 */

#ifndef DMSDP_AUDIO_HANDLER_H
#define DMSDP_AUDIO_HANDLER_H

#include "dmsdp_audio_data_type.h"
#include "dmsdp_business.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /*
     * get audio device capacity
     * capabilities: audio capacity struct
     */
    int32_t (*GetAudioCapability)(DMSDPAudioCapabilities **capabilities, uint32_t *num);

    /* query audio capabilities string */
    const char *(*GetAudioCapabilityString)(uint32_t *len);

    /*
     * set audio track proile
     * Id: speaker id
     * idLen: id length
     * sessionId: transport session id
     * profiles keys:"channel" "format" "sampleRate" "codec"
     */
    int32_t (*SetAudioParameter)(const char *id, uint32_t idLen, int32_t sessionId, uint32_t streamType,
        const DMSDPProfiles *profiles);

    /*
     * open audio track and write track data
     * id: speaker id
     * idLen: id length
     * sessionId: transport session id
     * data:audio datas struct
     */
    int32_t (*WriteStreamBuffer)(const char *id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData *data);

    /*
     * close audio track
     * id: speaker id
     * idLen: id length
     * sessionId: transport session id
     */
    int32_t (*CloseAudioTrack)(const char *id, uint32_t idLen, int32_t sessionId);

    /*
     * open audio record
     * id: mic id
     * idLen: id length
     * sessionId: transport session id
     * inputSource: the recording source
     * profiles keys:"channel" "format" "sampleRate" "codec"
     */
    int32_t (*OpenAudioRecord)(const char *id, uint32_t idLen, int32_t sessionId, int32_t inputSource,
        const DMSDPProfiles *profiles);

    /*
     * read audio record data
     * id: mic id
     * idLen: id length
     * sessionId: transport session id
     * data:audio datas struct
     */
    int32_t (*ReadStreamBuffer)(const char *id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData *data);

    /*
     * close audio record
     * id: mic id
     * idLen: id length
     * sessionId: transport session id
     */
    int32_t (*CloseAudioRecord)(const char *id, uint32_t idLen, int32_t sessionId);

    /*
     * release
     * type: DMSDPAudioReleaseType
     */
    void (*Release)(int32_t type, void *ptr, uint32_t num);

    /*
     * reg listener
     * listener: DMSDPListener
     */
    int32_t (*RegisterListener)(const DMSDPListener *listener);

    /*
     * business control
     * cmd: command DMSDPBusinessControlCmd
     * para: parameter
     * length: parameter length
     */
    int32_t (*BusinessControl)(uint32_t cmd, void *inputPara, uint32_t inLen, void *outputPara, uint32_t outLen);

    /*
     * request audio focus
     * streamType the main audio stream type affected by the focus request
     * focusType: focus type
     */
    int32_t (*RequestAudioFocus)(int32_t streamType, int32_t focusType);

    /* abandon audio focus */
    int32_t (*AbandonAudioFocus)();
} DMSDPAudioHandler;

/* get audio handler interface */
int32_t DMSDPGetAudioHandler(DMSDPAudioHandler *audioHandler);

#ifdef __cplusplus
}
#endif

#endif
