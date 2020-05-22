#include "HiCarAudioHal.h"
#include "dmsdp_log.h"
#include "util.h"
#include "container_of.h"
#include "config.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>


static HsaeAudioHandle mediaCard;
static HsaeAudioHandle navi_card;
static HsaeAudioHandle mic_card;
static HicarAudioSource phone_src;
static HicarAudioSource media_src;
static GDBusConnection *conn = NULL;


#define MALLOC_CHECK(v) \
do { \
    if (NULL == v) { \
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "No memory for this test!\n"); \
        exit(-1);\
    } \
} while(0)

#define PROFILES_KEY_CODEC "codec"
#define PROFILES_KEY_SAMPLERATE "sampleRate"
#define PROFILES_KEY_FORMAT "format"
#define PROFILES_KEY_CHANNEL "channel"



int32_t SetAudioParameter(const char *id, uint32_t idLen, int32_t sessionId, uint32_t streamType,
                          const DMSDPProfiles *profiles);
void set_sw_params(snd_pcm_t* pcm,
                   snd_pcm_uframes_t period_size, snd_pcm_uframes_t buffer_size);
static void init_mic();
static int32_t init_audiopolicy_srv();
static int32_t set_audiosrc_state(HicarAudioSource *s, bool acquire);

static int32_t set_audiosrc_state(HicarAudioSource *s, bool acquire) {
    GDBusMessage *msg = NULL;
    GError *error = NULL;
    GDBusMessage *reply = NULL;
    GVariant *body = NULL;

    msg = g_dbus_message_new_method_call(AUDIO_POLICY_SERVER_ADDR, AUDIO_POLICY_PATH_BASE, MANAGER_IFACE, AM_SET_STATE);
    if (!msg){
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Can not connect to system bus");
        return DMSDP_ERR_AUDIO_BASE;
    }
    g_dbus_message_set_body(msg, g_variant_new("(sii)", s->src, acquire, -1));
    reply = g_dbus_connection_send_message_with_reply_sync(conn, msg, G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                           500, NULL, NULL, &error);
    if (error) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Dbus Msg Send Error");
        return DMSDP_ERR_AUDIO_BASE;
    }
    g_dbus_connection_flush_sync (conn, NULL, &error);
    if (error) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Dbus Msg Send Error");
        return DMSDP_ERR_AUDIO_BASE;
    }

    return DMSDP_OK;
}


static int32_t init_audiopolicy_srv() {
    GDBusMessage *msg = NULL;
    GError *error = NULL;
    GDBusMessage *reply = NULL;
    GVariant *body = NULL;
    const gchar *rvalue = NULL;

    conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Can not connect to system bus");
        return DMSDP_ERR_AUDIO_BASE;
    }

    /*  create resource set */
    msg = g_dbus_message_new_method_call(AUDIO_POLICY_SERVER_ADDR, AUDIO_POLICY_PATH_BASE, MANAGER_IFACE, AM_CREATE_RESOURCE);
    if (!msg){
        goto msg_create_error;
    }
    g_dbus_message_set_body(msg, g_variant_new("(iii)", 100, 20, 1));
    reply = g_dbus_connection_send_message_with_reply_sync(conn, msg, G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                           500, NULL, NULL, &error);
    if (error) {
        goto msg_send_error;
    }
    g_dbus_connection_flush_sync (conn, NULL, &error);
    if (error) {
        goto msg_send_error;
    }

    body = g_dbus_message_get_body (reply);
    g_variant_get (body, "(&s)", &(phone_src.src));
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Audio Res %s", phone_src.src);

    /*  create media resource set */
    msg = g_dbus_message_new_method_call(AUDIO_POLICY_SERVER_ADDR, AUDIO_POLICY_PATH_BASE, MANAGER_IFACE, AM_CREATE_RESOURCE);
    if (!msg){
        goto msg_create_error;
    }
    g_dbus_message_set_body(msg, g_variant_new("(iii)", 100, 21, 1));
    reply = g_dbus_connection_send_message_with_reply_sync(conn, msg, G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                           500, NULL, NULL, &error);
    if (error) {
        goto msg_send_error;
    }
    g_dbus_connection_flush_sync (conn, NULL, &error);
    if (error) {
        goto msg_send_error;
    }

    body = g_dbus_message_get_body (reply);
    g_variant_get (body, "(&s)", &(media_src.src));
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Audio Res %s", media_src.src);

    return DMSDP_OK;

msg_create_error:
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Create Dbus Msg Error");
    return DMSDP_ERR_AUDIO_BASE;
msg_send_error:
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Dbus Msg Send Error");
    return DMSDP_ERR_AUDIO_BASE;


    return 0;
}

snd_pcm_t ** getAudioHandleById(const char *cardid) {
    if (strcmp(cardid, HSAE_MEDIA_CARD) == 0 ||
        strcmp(cardid, HSAE_VIR_MEDIA) == 0) {
        return &mediaCard.pcm;
    } else if (strcmp(cardid, HSAE_NAVI_CARD) == 0) {
        return &navi_card.pcm;
    } else if (strcmp(cardid, HSAE_MIC_CARD) == 0 ||
               strcmp(cardid, HSAE_VIR_MIC) == 0) {
        return &mic_card.pcm;
    } else {
        return NULL;
    }
}

static void make_tmpid(char *s, const char *cardid, int32_t len) {
    memcpy(s, cardid, len);
    s[len] = '\0';
}

bool write_all(int fd, const void *data, size_t size)
{
    while (size) {
        ssize_t done;

        done = write(fd, data, size);
        if (done < 0 && errno == EINTR)
            continue;
        if (done <= 0)
            return false;
        data = (const char *)data + done;
        size -= done;
    }

    return true;
}

bool read_all(int fd, void *data, size_t size)
{
    while (size) {
        ssize_t done;

        done = read(fd, data, size);
        if (done < 0 && errno == EINTR)
            continue;
        if (done <= 0)
            return false;
        data = (char *)data + done;
        size -= done;
    }

    return true;
}


static void init_mic() {
    char *cardid = strdup(HSAE_MIC_CARD);
    DMSDPProfiles params;
    size_t cardid_len = strlen(cardid);
    int profileIndex = 0;
    int32_t r = DMSDP_OK;

    params.num = 3;
    params.profiles = (DMSDPProfile *) malloc(sizeof(DMSDPProfile) * params.num);
    MALLOC_CHECK(params.profiles);

    params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_FORMAT);
    params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
    MALLOC_CHECK(params.profiles[profileIndex].key);
    memcpy(params.profiles[profileIndex].key, PROFILES_KEY_FORMAT, params.profiles[profileIndex].keyLen);
    params.profiles[profileIndex].value = AUDIO_FORMAT_PCM_16_BIT;

    profileIndex++;

    params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_CHANNEL);
    params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
    MALLOC_CHECK(params.profiles[profileIndex].key);
    memcpy(params.profiles[profileIndex].key, PROFILES_KEY_CHANNEL, params.profiles[profileIndex].keyLen);
    params.profiles[profileIndex].value = AUDIO_CHANNEL_IN_MONO;

    profileIndex++;

    params.profiles[profileIndex].keyLen = strlen(PROFILES_KEY_SAMPLERATE);
    params.profiles[profileIndex].key = malloc(sizeof(char) * params.profiles[profileIndex].keyLen);
    MALLOC_CHECK(params.profiles[profileIndex].key);
    memcpy(params.profiles[profileIndex].key, PROFILES_KEY_SAMPLERATE, params.profiles[profileIndex].keyLen);
    params.profiles[profileIndex].value = AUDIO_SAMPLE_RATE_16K;

    r = SetAudioParameter(cardid, cardid_len, 0, 0, &params);
 }

int32_t GetAudioCapability(DMSDPAudioCapabilities **capabilities,
                           uint32_t *num) {
    static int32_t audio_policy_init_guard = 0;
    int32_t r = 0;

    if (!audio_policy_init_guard) {
        r = init_audiopolicy_srv();
        if (r != DMSDP_OK) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail to connect Audio Policy Server!");
            return DMSDP_ERR_AUDIO_BASE;

        }
        audio_policy_init_guard = 1;
    }

    /* we have 3 devs, 1 media card, 1 navi card and 1 mic */
    uint32_t audiodevNum = 4;
    *num = audiodevNum;

    *capabilities = (DMSDPAudioCapabilities *) malloc(
            (sizeof(DMSDPAudioCapabilities)) * audiodevNum);
    if (*capabilities == NULL) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
        return DMSDP_ERR_NO_MEMORY;
    }

    memset(*capabilities, 0, (sizeof(DMSDPAudioCapabilities)) * audiodevNum);
    DMSDPAudioCapabilities *audioDevs = *capabilities;
    uint32_t devIndex = 0;

    /*Card 0: Media*/
    {
        uint32_t profileNum = 1;
        const char *mediaCardName = HSAE_MEDIA_CARD;
        uint32_t nameLength = strlen(mediaCardName);
        char *id = malloc(VENDOR_AUDIO_ID_MAX_LEN);
        if (id == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }

        memset(id, 0, VENDOR_AUDIO_ID_MAX_LEN);
        memcpy(id, mediaCardName, nameLength);
        audioDevs[devIndex].id = id;
        audioDevs[devIndex].idLen = nameLength;
        audioDevs[devIndex].type = AUDIO_DEVICE_TYPE_SPEAKER;

        /* set the profile */
        audioDevs[devIndex].profiles.num = profileNum;
        DMSDPAudioProfile *profile =
                (DMSDPAudioProfile *) malloc((sizeof(DMSDPAudioProfile)) * profileNum);
        if (profile == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(profile, 0, (sizeof(DMSDPAudioProfile)) * profileNum);

        profile[0].sampleRates = AUDIO_SAMPLE_RATE_44_1K;
        profile[0].channelMask = AUDIO_CHANNEL_OUT_STEREO;
        profile[0].format = AUDIO_FORMAT_PCM_16_BIT;
        audioDevs[devIndex].profiles.profiles = profile;

        /* set codec */
        uint32_t codecNum = 1;
        audioDevs[devIndex].codecs.num = codecNum;
        DMSDPAudioCodec *codecs =
                (DMSDPAudioCodec *) malloc((sizeof(DMSDPAudioCodec)) * codecNum);
        if (codecs == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(codecs, 0, (sizeof(DMSDPAudioCodec)) * codecNum);
        codecs[0] = FORMAT_PCM;
        audioDevs[devIndex].codecs.codecs = codecs;
    }

    devIndex++;

    /*Card 2: Mic*/
    {
        uint32_t profileNum = 1;
        const char *naviCardName = HSAE_MIC_CARD;
        uint32_t nameLength = strlen(naviCardName);
        char *id = malloc(VENDOR_AUDIO_ID_MAX_LEN);
        if (id == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }

        memset(id, 0, VENDOR_AUDIO_ID_MAX_LEN);
        memcpy(id, naviCardName, nameLength);
        audioDevs[devIndex].id = id;
        audioDevs[devIndex].idLen = nameLength;
        audioDevs[devIndex].type = AUDIO_DEVICE_TYPE_MIC;

        /* set the profile */
        audioDevs[devIndex].profiles.num = profileNum;
        DMSDPAudioProfile *profile =
                (DMSDPAudioProfile *) malloc((sizeof(DMSDPAudioProfile)) * profileNum);
        if (profile == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(profile, 0, (sizeof(DMSDPAudioProfile)) * profileNum);

        profile[0].sampleRates = AUDIO_SAMPLE_RATE_16K;
        profile[0].channelMask = AUDIO_CHANNEL_IN_MONO;
        profile[0].format = AUDIO_FORMAT_PCM_16_BIT;
        audioDevs[devIndex].profiles.profiles = profile;

        /* set codec */
        uint32_t codecNum = 1;
        audioDevs[devIndex].codecs.num = codecNum;
        DMSDPAudioCodec *codecs =
                (DMSDPAudioCodec *) malloc((sizeof(DMSDPAudioCodec)) * codecNum);
        if (codecs == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(codecs, 0, (sizeof(DMSDPAudioCodec)) * codecNum);
        codecs[0] = FORMAT_PCM;
        audioDevs[devIndex].codecs.codecs = codecs;
    }

    devIndex++;

    /*  virtual speaker */
    {
        uint32_t profileNum = 1;
        const char *mediaCardName = HSAE_VIR_MEDIA;
        uint32_t nameLength = strlen(mediaCardName);
        char *id = malloc(VENDOR_AUDIO_ID_MAX_LEN);
        if (id == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }

        memset(id, 0, VENDOR_AUDIO_ID_MAX_LEN);
        memcpy(id, mediaCardName, nameLength);
        audioDevs[devIndex].id = id;
        audioDevs[devIndex].idLen = nameLength;
        audioDevs[devIndex].type = AUDIO_DEVICE_TYPE_MODEM_SPEAKER;

        /* set the profile */
        audioDevs[devIndex].profiles.num = profileNum;
        DMSDPAudioProfile *profile =
                (DMSDPAudioProfile *) malloc((sizeof(DMSDPAudioProfile)) * profileNum);
        if (profile == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(profile, 0, (sizeof(DMSDPAudioProfile)) * profileNum);

        profile[0].sampleRates = AUDIO_SAMPLE_RATE_44_1K;
        profile[0].channelMask = AUDIO_CHANNEL_OUT_STEREO;
        profile[0].format = AUDIO_FORMAT_PCM_16_BIT;
        audioDevs[devIndex].profiles.profiles = profile;

        /* set codec */
        uint32_t codecNum = 1;
        audioDevs[devIndex].codecs.num = codecNum;
        DMSDPAudioCodec *codecs =
                (DMSDPAudioCodec *) malloc((sizeof(DMSDPAudioCodec)) * codecNum);
        if (codecs == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(codecs, 0, (sizeof(DMSDPAudioCodec)) * codecNum);
        codecs[0] = FORMAT_PCM;
        audioDevs[devIndex].codecs.codecs = codecs;
    }

    devIndex++;

    {
        uint32_t profileNum = 1;
        const char *naviCardName = HSAE_VIR_MIC;
        uint32_t nameLength = strlen(naviCardName);
        char *id = malloc(VENDOR_AUDIO_ID_MAX_LEN);
        if (id == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }

        memset(id, 0, VENDOR_AUDIO_ID_MAX_LEN);
        memcpy(id, naviCardName, nameLength);
        audioDevs[devIndex].id = id;
        audioDevs[devIndex].idLen = nameLength;
        audioDevs[devIndex].type = AUDIO_DEVICE_TYPE_MODEM_MIC;

        /* set the profile */
        audioDevs[devIndex].profiles.num = profileNum;
        DMSDPAudioProfile *profile =
                (DMSDPAudioProfile *) malloc((sizeof(DMSDPAudioProfile)) * profileNum);
        if (profile == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(profile, 0, (sizeof(DMSDPAudioProfile)) * profileNum);

        profile[0].sampleRates = AUDIO_SAMPLE_RATE_16K;
        profile[0].channelMask = AUDIO_CHANNEL_IN_MONO;
        profile[0].format = AUDIO_FORMAT_PCM_16_BIT;
        audioDevs[devIndex].profiles.profiles = profile;

        /* set codec */
        uint32_t codecNum = 1;
        audioDevs[devIndex].codecs.num = codecNum;
        DMSDPAudioCodec *codecs =
                (DMSDPAudioCodec *) malloc((sizeof(DMSDPAudioCodec)) * codecNum);
        if (codecs == NULL) {
            Release(RELEASE_TYPE_QUERY_ABILITY, *capabilities, audiodevNum);
            *capabilities = NULL;
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "No enough memory!");
            return DMSDP_ERR_NO_MEMORY;
        }
        memset(codecs, 0, (sizeof(DMSDPAudioCodec)) * codecNum);
        codecs[0] = FORMAT_PCM;
        audioDevs[devIndex].codecs.codecs = codecs;
    }

    return DMSDP_OK;
}

const char *GetAudioCapabilityString(uint32_t *len) {
    /* root object */
    json_object *jobj = json_object_new_object();
    json_object *capa_array = json_object_new_array();

    /* construct media card info */
    json_object *jmcobj = json_object_new_object();
    json_object *jmcname = json_object_new_string(HSAE_MEDIA_CARD);
    json_object *jmcnamelen = json_object_new_int(strlen(HSAE_MEDIA_CARD));
    json_object *jmctype = json_object_new_int(AUDIO_DEVICE_TYPE_SPEAKER);

    json_object *jmcprofiles = json_object_new_object();
    json_object *jmcpnum = json_object_new_int(1);
    json_object *jmcparray = json_object_new_array();
    json_object *jmcprofile = json_object_new_object();
    json_object *jmcsamplerates = json_object_new_int(AUDIO_SAMPLE_RATE_48K);
    json_object *jmcchannel = json_object_new_int(AUDIO_CHANNEL_OUT_STEREO);
    json_object *jmcformat = json_object_new_int(AUDIO_FORMAT_PCM_16_BIT);

    json_object_object_add(jmcprofile, "sampleRates", jmcsamplerates);
    json_object_object_add(jmcprofile, "channelMask", jmcchannel);
    json_object_object_add(jmcprofile, "format", jmcformat);
    json_object_array_add(jmcparray, jmcprofile);
    json_object_object_add(jmcprofiles, "num", jmcpnum);
    json_object_object_add(jmcprofiles, "profiles", jmcparray);

    json_object *jmccodecs = json_object_new_object();
    json_object *jmccodecnum = json_object_new_int(1);
    json_object *jmccodecarray = json_object_new_array();
    json_object *jmccodec1info = json_object_new_int(FORMAT_PCM);
    json_object_array_add(jmccodecarray, jmccodec1info);
    json_object_object_add(jmccodecs, "num", jmccodecnum);
    json_object_object_add(jmccodecs, "codecs", jmccodecarray);

    json_object_object_add(jmcobj, "id", jmcname);
    json_object_object_add(jmcobj, "idLen", jmcnamelen);
    json_object_object_add(jmcobj, "type", jmctype);
    json_object_object_add(jmcobj, "profiles", jmcprofiles);
    json_object_object_add(jmcobj, "codecs", jmccodecs);

    json_object_array_add(capa_array, jmcobj);

    /* construct navi card info */
    json_object *jnvobj = json_object_new_object();
    json_object *jnvname = json_object_new_string(HSAE_NAVI_CARD);
    json_object *jnvnamelen = json_object_new_int(strlen(HSAE_NAVI_CARD));
    json_object *jnvtype = json_object_new_int(AUDIO_DEVICE_TYPE_SPEAKER);

    json_object *jnvprofiles = json_object_new_object();
    json_object *jnvpnum = json_object_new_int(1);
    json_object *jnvparray = json_object_new_array();
    json_object *jnvprofile = json_object_new_object();
    json_object *jnvsamplerates = json_object_new_int(AUDIO_SAMPLE_RATE_16K);
    json_object *jnvchannel = json_object_new_int(AUDIO_CHANNEL_OUT_MONO);
    json_object *jnvformat = json_object_new_int(AUDIO_FORMAT_PCM_16_BIT);

    json_object_object_add(jnvprofile, "sampleRates", jnvsamplerates);
    json_object_object_add(jnvprofile, "channelMask", jnvchannel);
    json_object_object_add(jnvprofile, "format", jnvformat);
    json_object_array_add(jnvparray, jnvprofile);
    json_object_object_add(jnvprofiles, "num", jnvpnum);
    json_object_object_add(jnvprofiles, "profiles", jnvparray);

    json_object *jnvcodecs = json_object_new_object();
    json_object *jnvcodecnum = json_object_new_int(1);
    json_object *jnvcodecarray = json_object_new_array();
    json_object *jnvcodec1info = json_object_new_int(FORMAT_PCM);
    json_object_array_add(jnvcodecarray, jnvcodec1info);
    json_object_object_add(jnvcodecs, "num", jnvcodecnum);
    json_object_object_add(jnvcodecs, "codecs", jnvcodecarray);

    json_object_object_add(jnvobj, "id", jnvname);
    json_object_object_add(jnvobj, "idLen", jnvnamelen);
    json_object_object_add(jnvobj, "type", jnvtype);
    json_object_object_add(jnvobj, "profiles", jnvprofiles);
    json_object_object_add(jnvobj, "codecs", jnvcodecs);

    json_object_array_add(capa_array, jnvobj);

    /* construct mic info */
    json_object *jMicobj = json_object_new_object();
    json_object *jMicname = json_object_new_string(HSAE_MIC_CARD);
    json_object *jMicnamelen = json_object_new_int(strlen(HSAE_MIC_CARD));
    json_object *jMictype = json_object_new_int(AUDIO_DEVICE_TYPE_MIC);

    json_object *jMicprofiles = json_object_new_object();
    json_object *jMicpnum = json_object_new_int(1);
    json_object *jMicparray = json_object_new_array();
    json_object *jMicprofile = json_object_new_object();
    json_object *jMicsamplerates = json_object_new_int(AUDIO_SAMPLE_RATE_16K);
    json_object *jMicchannel = json_object_new_int(AUDIO_CHANNEL_IN_MONO);
    json_object *jMicformat = json_object_new_int(AUDIO_FORMAT_PCM_16_BIT);

    json_object_object_add(jMicprofile, "sampleRates", jMicsamplerates);
    json_object_object_add(jMicprofile, "channelMask", jMicchannel);
    json_object_object_add(jMicprofile, "format", jMicformat);
    json_object_array_add(jMicparray, jMicprofile);
    json_object_object_add(jMicprofiles, "num", jMicpnum);
    json_object_object_add(jMicprofiles, "profiles", jMicparray);

    json_object *jMiccodecs = json_object_new_object();
    json_object *jMiccodecnum = json_object_new_int(1);
    json_object *jMiccodecarray = json_object_new_array();
    json_object *jMiccodec1info = json_object_new_int(FORMAT_PCM);
    json_object_array_add(jMiccodecarray, jMiccodec1info);
    json_object_object_add(jMiccodecs, "num", jMiccodecnum);
    json_object_object_add(jMiccodecs, "codecs", jMiccodecarray);

    json_object_object_add(jMicobj, "id", jMicname);
    json_object_object_add(jMicobj, "idLen", jMicnamelen);
    json_object_object_add(jMicobj, "type", jMictype);
    json_object_object_add(jMicobj, "profiles", jMicprofiles);
    json_object_object_add(jMicobj, "codecs", jMiccodecs);

    json_object_array_add(capa_array, jMicobj);
    json_object_object_add(jobj, "capabilities", capa_array);

    const char *jStr = strdup(json_object_to_json_string(jobj));

    json_object_put(jobj);

    *len = strlen(jStr);
    return jStr;
}

int32_t SetAudioParameter(const char *id, uint32_t idLen, int32_t sessionId, uint32_t streamType,
                          const DMSDPProfiles *profiles) {

    int32_t hret = DMSDP_OK;
    int r = -1;
    uint32_t buffer_time = 0;
    uint32_t period_time = 0;
    uint32_t sample_rate_except = 0;
    uint32_t sample_rate = 0;
    uint32_t pcm_format = 0;
    uint32_t channels = 0;
    ulong period_size = 0;
    ulong buffer_size = 0;
    uint32_t bits_per_sample;
    uint32_t bits_per_frame;
    uint32_t period_byte;

    char tmpid[64] = {0};
    make_tmpid(tmpid, id, idLen);


    snd_pcm_t **pcm = NULL;
    snd_pcm_hw_params_t *hwparam = NULL;

    if (strcmp(tmpid, HSAE_MEDIA_CARD) == 0 ||
        strcmp(tmpid, HSAE_VIR_MEDIA) == 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "MediaCard Config Begin");
        r = snd_pcm_open(&mediaCard.pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail open media card!");
            return hret;
        }
        pcm = &mediaCard.pcm;

#ifdef ENABLE_AUDIO_CAPTURE
        mediaCard.pcmfd = open(OUT_PCM_PATH, O_WRONLY | O_CREAT, 0644);
#endif

    } else if (strcmp(tmpid, HSAE_NAVI_CARD) == 0) {
        r = snd_pcm_open(&navi_card.pcm, "card1_dm", SND_PCM_STREAM_PLAYBACK, 0);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail open navigation card!");
            return hret;
        }
        pcm = &navi_card.pcm;
    } else if (strcmp(tmpid, HSAE_MIC_CARD) == 0 ||
               strcmp(tmpid, HSAE_VIR_MIC) == 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "Mic Card Config Begin1");

        r = snd_pcm_open(&mic_card.pcm, "default", SND_PCM_STREAM_CAPTURE, 0);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail open mic card!");
            return hret;
        }
        pcm = &mic_card.pcm;
        
#ifdef ENABLE_AUDIO_CAPTURE
        mic_card.pcmfd = open(IN_PCM_PATH, O_WRONLY | O_CREAT, 0644);
#endif

    } else {
        return hret;
    }


    snd_pcm_hw_params_alloca(&hwparam);
    r = snd_pcm_hw_params_any(*pcm, hwparam);
    if (r < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail to set default params!");
        return hret;
    }

    r = snd_pcm_hw_params_set_access(*pcm, hwparam, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (r < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL", "Fail to set params access!");
        return hret;
    }

    switch(getAudioParamFromDMSDPProfiles(profiles, "format")) {
        case AUDIO_FORMAT_PCM_16_BIT:
            pcm_format = SND_PCM_FORMAT_S16_LE;
            break;
        case AUDIO_FORMAT_PCM_8_BIT:
            pcm_format = SND_PCM_FORMAT_S8;
            break;
        case AUDIO_FORMAT_PCM_8_24_BIT:
            pcm_format = SND_PCM_FORMAT_S24_LE;
            break;
        case AUDIO_FORMAT_PCM_32_BIT:
            pcm_format = SND_PCM_FORMAT_S32_LE;
            break;
        default:
            pcm_format = 0;
            break;
    }

    if (pcm_format != 0) {
        r = snd_pcm_hw_params_set_format(*pcm, hwparam, pcm_format);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Fail to set format failed!");
        }
    } else {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "No valid pcm format in profiles!");
    }

    switch (getAudioParamFromDMSDPProfiles(profiles, "channel")) {
        case AUDIO_CHANNEL_IN_MONO:
        case AUDIO_CHANNEL_OUT_MONO:
            channels = 1;
            break;
        case AUDIO_CHANNEL_OUT_STEREO:
        case AUDIO_CHANNEL_IN_STEREO:
            channels = 2;
            break;
        default:
            channels = 0;
            break;
    }

    if (channels != 0) {
        r = snd_pcm_hw_params_set_channels(*pcm, hwparam, channels);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Fail to set channels");
        }
    } else {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "No valid channels in profiles!");
    }

    sample_rate_except = getAudioParamFromDMSDPProfiles(profiles, "sampleRate");
    sample_rate = sample_rate_except;
    if (sample_rate_except != 0) {
        r = snd_pcm_hw_params_set_rate_near(*pcm, hwparam, &sample_rate, 0);
        if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Fail to set sample rate");
        }
    } else {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "No valid sample rates in profiles!");
    }

    if (sample_rate != sample_rate_except) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Sample rate is not supported by hardware!");
    }

    r = snd_pcm_hw_params_get_buffer_time_max(hwparam, &buffer_time, 0);
    if(r < 0)
    {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL","snd_pcm_hw_params_get_buffer_time_max failed");
        return hret;
    }

    buffer_time =  buffer_time > 500000 ? 512000 : buffer_time;
    period_time = 10000;

    r = snd_pcm_hw_params_set_buffer_time_near(*pcm, hwparam, &buffer_time, 0);
    if(r < 0)
    {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL","snd_pcm_hw_params_get_buffer_time_max failed");
        return hret;
    }

    r = snd_pcm_hw_params_set_period_time_near(*pcm, hwparam, &period_time, 0);
    if(r < 0)
    {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL","snd_pcm_hw_params_set_period_time_near failed\n");
        return hret;
    }

    r = snd_pcm_hw_params(*pcm, hwparam);
    if(r < 0)
    {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL",  "snd_pcm_hw_params failed\n");
        return hret;
    }

    snd_pcm_hw_params_get_period_size(hwparam, &period_size,0);
    snd_pcm_hw_params_get_buffer_size(hwparam, &buffer_size);

    if(buffer_size == period_size) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "the period_size can't equal buffer_size");
        return hret;
    }

    bits_per_sample = snd_pcm_format_physical_width(pcm_format);
    bits_per_frame = bits_per_sample * channels;
    period_byte = period_size * bits_per_frame / 8;
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "the period_size is:%d", period_size);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "the buffer_size is:%d", buffer_size);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "the period_byte is:%d", period_byte);




    HsaeAudioHandle *audioHandle = container_of(pcm, HsaeAudioHandle, pcm);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "MediaCard address:%p", &mediaCard);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "navicard address:%p", &navi_card);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "miccard address:%p", &mic_card);
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "MediaCard address:%p", audioHandle);
    audioHandle->bits_per_frame = bits_per_frame;
    audioHandle->period_size = period_size;
    audioHandle->channels = channels;
    audioHandle->pcm_format = pcm_format;
    audioHandle->period_byte = period_byte;

    audioHandle->buf = (unsigned char *)malloc(period_byte);
    if (audioHandle->buf == NULL) {
        hret = DMSDP_ERR_NO_MEMORY;
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "No Memory for Audio Buffer!");
    } else {
        audioHandle->len = period_byte;
        hret = DMSDP_OK;
    }

    set_sw_params(*pcm, period_size, buffer_size);

    audioHandle->opened = 1;

    return hret;
}

void set_sw_params(snd_pcm_t* pcm,
                   snd_pcm_uframes_t period_size, snd_pcm_uframes_t buffer_size) {
    //
    snd_pcm_sw_params_t* sw_params = NULL;
    snd_pcm_sw_params_alloca(&sw_params);

    // initialize sw_params
    if (snd_pcm_sw_params_current(pcm, sw_params) < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "snd_pcm_sw_params_current");
    }

    // set start threshold to buffer_size, so that ALSA starts playback only
    // after circular buffer becomes full first time
    if (snd_pcm_sw_params_set_start_threshold(pcm, sw_params, buffer_size) < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "snd_pcm_sw_params_set_start_threshold");
    }

    // set minimum number of samples that can be read by ALSA, so that it'll
    // wait until there are at least 'period_size' samples in circular buffer
    if (snd_pcm_sw_params_set_avail_min(pcm, sw_params, period_size) < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "snd_pcm_sw_params_set_avail_min");
    }

    // send sw_params to ALSA
    if (snd_pcm_sw_params(pcm, sw_params) < 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "snd_pcm_sw_params");
    }
}


int32_t WriteStreamBuffer(const char *id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData *data) {
    ssize_t r = 0;
    size_t  buflen = data->header->length;
    ssize_t result = 0;
    uint8_t *temp_buf = data->data;
    char tmpid[64] = {0};



    make_tmpid(tmpid, id, idLen);
    snd_pcm_t **pcm = getAudioHandleById(tmpid);
    HsaeAudioHandle *handle = container_of(pcm, HsaeAudioHandle, pcm);
    int count = (buflen/handle->period_byte)+((buflen%handle->period_byte)?1:0);

    if (!handle->opened) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Card %s is not ready!", tmpid);
        return DMSDP_ERR_AUDIO_BASE;
    }

    int leftLen = 0;
    int rc = 0;

    int i = 0;
    for(i=0; i < count; i++){
        leftLen = (i == count - 1) ? (buflen - (handle->period_byte * i)) : handle->period_byte;

        rc = snd_pcm_writei(handle->pcm, temp_buf + handle->period_byte * i, leftLen >> 2);
        if (rc == -EAGAIN || (rc >= 0 && (size_t)rc < (leftLen >> 2)))
        {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "=====again: %s(%d)=====\n", snd_strerror(rc), rc);
            snd_pcm_wait(handle->pcm, 100);
        }
        else if (rc == -EPIPE)
        {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "=====Underrun: %s(%d)=====\n", snd_strerror(rc), rc);
            snd_pcm_prepare(handle->pcm);
        }
        else if (rc == -ESTRPIPE)
        {
            while ((rc = snd_pcm_resume(handle->pcm)) == -EAGAIN)
            {
                DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "=====snd_pcm_resume, sleep =====\n");
                sleep(1);  /* wait until resume flag is released */
            }
            if (rc < 0)
                snd_pcm_prepare(handle->pcm);
        }
    }

#ifdef ENABLE_AUDIO_CAPTURE
    write_all(mediaCard.pcmfd, temp_buf, buflen);
#endif

    return DMSDP_OK;
}

int32_t CloseAudioTrack(const char *id, uint32_t idLen, int32_t sessionId) {

    char tmpid[64] = {0};

    make_tmpid(tmpid, id, idLen);

    snd_pcm_t **pcm = getAudioHandleById(tmpid);
    HsaeAudioHandle *audioHandle = container_of(pcm, HsaeAudioHandle, pcm);

    if (audioHandle->opened) {
        snd_pcm_close(*pcm);
        audioHandle->opened = 0;
#ifdef ENABLE_AUDIO_CAPTURE
        close(audioHandle->pcmfd);
#endif

    }
    return DMSDP_OK;
}

int32_t
OpenAudioRecord(const char *id, uint32_t idLen, int32_t sessionId, int32_t inputSource, const DMSDPProfiles *profiles) {

    return DMSDP_OK;
}

int32_t ReadStreamBuffer(const char *id, uint32_t idLen, int32_t sessionId, DMSDPVirtualStreamData *data) {

    ssize_t r;
    size_t result = 0;
    size_t buflen = data->header->length;
    uint8_t *buf = data->data;
    char tmpid[64] = {0};

    make_tmpid(tmpid, id, idLen);
    snd_pcm_t **pcm = getAudioHandleById(tmpid);
    HsaeAudioHandle *audioHandle = container_of(pcm, HsaeAudioHandle, pcm);
    
    if (!audioHandle->opened) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "Card %s is not ready!", tmpid);
        init_mic();
    }

    size_t frame_count = buflen * 8 / audioHandle->bits_per_frame;

    while (frame_count > 0) {

        r = snd_pcm_readi(audioHandle->pcm, buf,
                          (frame_count < audioHandle->period_size) ? frame_count: audioHandle->period_size);

        if (r == -EAGAIN || (r >= 0 && (size_t)r < frame_count)) {
            snd_pcm_wait(audioHandle->pcm, 1000);
        } else if (r == -EPIPE) {
            snd_pcm_prepare(audioHandle->pcm);
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "<<<<<<<<<<<<<<< Buffer overrun >>>>>>>>>>>>>>>");
        } else if (r == -ESTRPIPE) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL",  "<<<<<<<<<<<<<<< Need suspend >>>>>>>>>>>>>>>");
        } else if (r < 0) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHAL",  "Error snd_pcm_writei: [%s]", snd_strerror(r));
            return DMSDP_ERR_UNKNOWN_ERROR;
        }

        if (r > 0) {
            frame_count -= r;
#ifdef ENABLE_AUDIO_CAPTURE
            write_all(mic_card.pcmfd, buf, r * audioHandle->bits_per_frame / 8 );
#endif
            buf += r * audioHandle->bits_per_frame / 8;
        }
    }



    return DMSDP_OK;
}

int32_t CloseAudioRecord(const char *id, uint32_t idLen, int32_t sessionId) {
    char tmpid[64] = {0};

    make_tmpid(tmpid, id, idLen);

    snd_pcm_t **pcm = getAudioHandleById(tmpid);
    HsaeAudioHandle *audioHandle = container_of(pcm, HsaeAudioHandle, pcm);

    if (audioHandle->opened) {
        snd_pcm_close(*pcm);
        audioHandle->opened = 0;
#ifdef ENABLE_AUDIO_CAPTURE
        close(audioHandle->pcmfd);
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "close mic fd %d", audioHandle->pcmfd);
#endif
    }
    return DMSDP_OK;
}

void Release(int32_t type, void *ptr, uint32_t num) {

}

int32_t BusinessControl(uint32_t cmd, void *inputPara, uint32_t inLen, void *outputPara, uint32_t outLen) {


    return DMSDP_OK;
}

int32_t RegisterListener(const DMSDPListener* listener) {
    return DMSDP_OK;
}

int32_t RequestAudioFocus(int32_t streamType, int32_t focusType) {
    DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_WARNING, "AudioHAL", "focus %d---%d", streamType, focusType);
    if (streamType == 0) {
        set_audiosrc_state(&phone_src, true);
    } else {
        set_audiosrc_state(&media_src, true);
    }
    return DMSDP_OK;
}

int32_t AbandonAudioFocus() {
    set_audiosrc_state(&phone_src, false);
    return DMSDP_OK;
}


int32_t DMSDPGetAudioHandler(DMSDPAudioHandler *audioHandler) {

    memset(audioHandler, 0, sizeof(DMSDPAudioHandler));
    audioHandler->GetAudioCapability = GetAudioCapability;
    audioHandler->GetAudioCapabilityString = GetAudioCapabilityString;
    audioHandler->SetAudioParameter = SetAudioParameter;
    audioHandler->WriteStreamBuffer = WriteStreamBuffer;
    audioHandler->CloseAudioTrack = CloseAudioTrack;
    audioHandler->OpenAudioRecord = OpenAudioRecord;
    audioHandler->ReadStreamBuffer = ReadStreamBuffer;
    audioHandler->CloseAudioRecord = CloseAudioRecord;
    audioHandler->Release = Release;
    audioHandler->RegisterListener = RegisterListener;
    audioHandler->BusinessControl = BusinessControl;
    audioHandler->RequestAudioFocus = RequestAudioFocus;
    audioHandler->AbandonAudioFocus = AbandonAudioFocus;
    return DMSDP_OK;
}

long getPeriodBytes(const char *cardid) {
    if (strcmp(cardid, HSAE_MEDIA_CARD) == 0) {
        DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_INFO, "AudioHAL", "MediaCard");
        return mediaCard.period_byte;
    } else if (strcmp(cardid, HSAE_NAVI_CARD) == 0) {
        return navi_card.period_byte;
    } else if (strcmp(cardid, HSAE_MIC_CARD) == 0) {
        return mic_card.period_byte;
    } else {
        return -1;
    }
}
