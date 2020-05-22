#ifndef __HICARAUDIOHAL_H_
#define __HICARAUDIOHAL_H_
#include "dmsdp_audio_handler.h"
#include "config.h"
#include <alsa/asoundlib.h>
#include <gio/gio.h>


#define HSAE_MEDIA_CARD "MediaCard1"
#define HSAE_NAVI_CARD "NaviCard1"
#define HSAE_MIC_CARD "Mic1"
#define HSAE_VIR_MEDIA "VirtualMediaCard1"
#define HSAE_VIR_MIC   "VirtualMicCard1"

#define VENDOR_AUDIO_ID_MAX_LEN 256
#define AUDIO_POLICY_SERVER_ADDR "com.hsaeyz.AudioManager.Server"
#define AUDIO_POLICY_PATH_BASE "/"
#define MANAGER_IFACE "com.hsaeyz.AudioManager.If"
#define RSET_IFACE "org.murphy.resourceset"
#define RESOURCE_IFACE "org.murphy.resource"
#define MANAGER_CREATE_RESOURCE_SET "createResourceSet"
#define AM_CREATE_RESOURCE          "CreateResource"
#define AM_SET_STATE                "SetState"


#define RSET_SET_PROPERTY           "setProperty"
#define RSET_GET_PROPERTIES         "getProperties"
#define RSET_ADD_RESOURCE           "addResource"
#define RSET_REQUEST                "request"
#define RSET_RELEASE                "release"
#define RSET_DELETE                 "delete"
#define PROP_CLASS                  "class"

#define HICAR_PHONE                 "HicarPhone"

typedef struct __HsaeAudioHandle {
    snd_pcm_t *pcm;
    unsigned char *buf;
    uint32_t len;
    uint32_t period_size;
    uint32_t bits_per_frame;
    uint32_t pcm_format;
    uint32_t channels;
    uint32_t opened;
#ifdef ENABLE_AUDIO_CAPTURE
    int32_t pcmfd;
#endif
    long period_byte;
} HsaeAudioHandle;

typedef struct {
    const gchar *src;
    uint32_t focused;
} HicarAudioSource;

typedef enum {false = 0, true} bool;

int32_t GetAudioCapability(DMSDPAudioCapabilities **capabilities,
                            uint32_t *num);
int32_t CloseAudioRecord(const char *id, uint32_t idLen, int32_t sessionId);
const char *GetAudioCapabilityString(uint32_t *len);
int32_t SetAudioParameter(const char *id, uint32_t idLen, int32_t sessionId,
                          uint32_t streamType, const DMSDPProfiles *profiles);
int32_t WriteStreamBuffer(const char *id, uint32_t idLen, int32_t sessionId,
                          DMSDPVirtualStreamData *data);
int32_t CloseAudioTrack(const char *id, uint32_t idLen, int32_t sessionId);
int32_t OpenAudioRecord(const char *id, uint32_t idLen, int32_t sessionId,
                        int32_t inputSource, const DMSDPProfiles *profiles);
int32_t ReadStreamBuffer(const char *id, uint32_t idLen, int32_t sessionId,
                         DMSDPVirtualStreamData *data);
int32_t CloseAudioRecord(const char *id, uint32_t idLen, int32_t sessionId);
void Release(int32_t type, void *ptr, uint32_t num);
int32_t BusinessControl(uint32_t cmd, void *inputPara, uint32_t inLen,
                        void *outputPara, uint32_t outLen);
int32_t RequestAudioFocus(int32_t streamType, int32_t focusType);
int32_t AbandonAudioFocus();
int32_t DMSDPGetAudioHandler(DMSDPAudioHandler *audioHandler);
long getPeriodBytes(const char *cardid);

#endif // __HICARAUDIOHAL_H_
