/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: dmsdp audio type
 */

#ifndef DMSDP_AUDIO_DATA_TYPE_H
#define DMSDP_AUDIO_DATA_TYPE_H

#include <stdint.h>

typedef enum {
    AUDIO_DEVICE_TYPE_SPEAKER = 0u,
    AUDIO_DEVICE_TYPE_MIC = 1u,
    AUDIO_DEVICE_TYPE_MODEM_SPEAKER = 2u,
    AUDIO_DEVICE_TYPE_MODEM_MIC = 3u
} DMSDPAudioDevice;

typedef enum {
    AUDIO_SAMPLE_RATE_8K = 8000u,
    AUDIO_SAMPLE_RATE_11K = 11025u,
    AUDIO_SAMPLE_RATE_12K = 12000u,
    AUDIO_SAMPLE_RATE_16K = 16000u,
    AUDIO_SAMPLE_RATE_22K = 22050u,
    AUDIO_SAMPLE_RATE_24K = 24000u,
    AUDIO_SAMPLE_RATE_32K = 32000u,
    AUDIO_SAMPLE_RATE_44_1K = 44100u,
    AUDIO_SAMPLE_RATE_48K = 48000u,
    AUDIO_SAMPLE_RATE_96000 = 96000u,
    AUDIO_SAMPLE_RATE_192000 = 192000u
} DMSDPAudioSampleRates;

typedef enum {
    AUDIO_CHANNEL_OUT_MONO = 0x1u,
    AUDIO_CHANNEL_OUT_STEREO = 0x3u,
    AUDIO_CHANNEL_IN_MONO = 0x10u,
    AUDIO_CHANNEL_IN_STEREO = 0xCu
} DMSDPAudioChannelMasks;

typedef enum {
    AUDIO_FORMAT_PCM_16_BIT = 0x1u,
    AUDIO_FORMAT_PCM_8_BIT = 0x2u,
    AUDIO_FORMAT_PCM_32_BIT = 0x3u,
    AUDIO_FORMAT_PCM_8_24_BIT = 0x4u,
    AUDIO_FORMAT_PCM_24_BIT = 0x6u
} DMSDPAudioFormats;

typedef enum {
    FORMAT_AAC = 0,
    FORMAT_PCM = 1,
    FORMAT_G711A = 2,
} DMSDPAudioCodec;

typedef struct {
    uint32_t num;
    DMSDPAudioCodec *codecs;
} DMSDPAudioCodecs;

typedef enum {
    AUDIO_STREAM_VOICE_CALL = 0,
    AUDIO_STREAM_SYSTEM = 1,
    AUDIO_STREAM_RING = 2,
    AUDIO_STREAM_MUSIC = 3,
    AUDIO_STREAM_ALARM = 4,
    AUDIO_STREAM_NOTIFICATION = 5,
    AUDIO_STREAM_BLUETOOTH_SCO = 6,
    AUDIO_STREAM_SYSTEM_ENFORCED = 7,
    AUDIO_STREAM_DTMF = 8,
    AUDIO_STREAM_TTS = 9,
    AUDIO_STREAM_ACCESSIBILITY = 10,
} DMSDPAudioStreamType;

typedef enum {
    AUDIO_SOURCE_MIC = 1,
    AUDIO_SOURCE_VOICE_UPLINK = 2,
    AUDIO_SOURCE_VOICE_DOWNLINK = 3,
    AUDIO_SOURCE_VOICE_CALL = 4,
    AUDIO_SOURCE_CAMCORDER = 5,
    AUDIO_SOURCE_VOICE_RECOGNITION = 6,
    AUDIO_SOURCE_VOICE_COMMUNICATION = 7,
    AUDIO_SOURCE_REMOTE_SUBMIX = 8,
    AUDIO_SOURCE_UNPROCESSED = 9,
    AUDIO_SOURCE_FM_TUNER = 1998
} DMSDPAudioSource;

typedef enum {
    RELEASE_TYPE_QUERY_ABILITY = 1
} DMSDPAudioReleaseType;

typedef enum {
    AUDIOFOCUS_NONE = 0,
    AUDIOFOCUS_GAIN = 1,
    AUDIOFOCUS_GAIN_TRANSIENT = 2,
    AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK = 3,
    AUDIOFOCUS_GAIN_TRANSIENT_EXCLUSIVE = 4,
    AUDIOFOCUS_RELEASE = 5,
    AUDIOFOCUS_LOSS = -1,
    AUDIOFOCUS_LOSS_TRANSIENT = -2,
    AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK = -3
} DMSDPAudioFocus;

typedef enum {
    AUDIO_USAGE_UNKNOWN = 0,
    AUDIO_USAGE_MEDIA = 1,
    AUDIO_USAGE_VOICE_COMMUNICATION = 2,
    AUDIO_USAGE_VOICE_COMMUNICATION_SIGNALLING = 3,
    AUDIO_USAGE_ALARM = 4,
    AUDIO_USAGE_NOTIFICATION = 5,
    AUDIO_USAGE_NOTIFICATION_TELEPHONY_RINGTONE = 6,
    AUDIO_USAGE_NOTIFICATION_COMMUNICATION_REQUEST = 7,
    AUDIO_USAGE_NOTIFICATION_COMMUNICATION_INSTANT = 8,
    AUDIO_USAGE_NOTIFICATION_COMMUNICATION_DELAYED = 9,
    AUDIO_USAGE_NOTIFICATION_EVENT = 10,
    AUDIO_USAGE_ASSISTANCE_ACCESSIBILITY = 11,
    AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE = 12,
    AUDIO_USAGE_ASSISTANCE_SONIFICATION = 13,
    AUDIO_USAGE_GAME = 14,
    AUDIO_USAGE_VIRTUAL_SOURCE = 15,
    AUDIO_USAGE_ASSISTANT = 16,
    AUDIO_USAGE_TTS = 17
} DMSDPAudioType;

typedef struct {
    DMSDPAudioSampleRates sampleRates;
    DMSDPAudioChannelMasks channelMask;
    DMSDPAudioFormats format;
} DMSDPAudioProfile;

typedef struct {
    uint32_t num;
    DMSDPAudioProfile *profiles;
} DMSDPAudioProfiles;

typedef struct {
    char *key; /* key: "channel", "format", "sampleRate", "codec" */
    uint32_t keyLen;
    uint32_t value; /* value: AudioChannelMasks, AudioFormats, AudioSampleRates, AudioCodec */
} DMSDPProfile;

typedef struct {
    uint32_t num;
    DMSDPProfile *profiles;
} DMSDPProfiles;

typedef struct {
    char *key;
    uint32_t keyLen;
    char *value;
    uint32_t valueLen;
} DMSDPReserved;

typedef struct {
    uint32_t num;
    DMSDPReserved *reserveds;
} DMSDPReserveds;

typedef struct {
    char *id;
    uint32_t idLen;
    DMSDPAudioDevice type;
    DMSDPAudioProfiles profiles;
    DMSDPAudioCodecs codecs;
    DMSDPReserveds reserveds;
} DMSDPAudioCapabilities;

typedef struct {
    uint32_t streamType;
    uint32_t length;
    DMSDPReserveds reserveds;
} DMSDPVirtualStreamDataHeader;

typedef struct {
    uint8_t *data;
    int64_t timeStampUs;
    uint32_t usage;
    DMSDPVirtualStreamDataHeader *header;
} DMSDPVirtualStreamData;

#endif
