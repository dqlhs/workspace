#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HiCarAudioHal.h"

#define PROFILES_KEY_CODEC "codec"
#define PROFILES_KEY_SAMPLERATE "sampleRate"
#define PROFILES_KEY_FORMAT "format"
#define PROFILES_KEY_CHANNEL "channel"


#define MALLOC_CHECK(v) \
do { \
	if (NULL == v) { \
        printf("No memory for this test!\n"); \
        exit(-1);\
    } \
} while(0)


int main(int argc, char *argv[]) {
    char *cardid = strdup(HSAE_MIC_CARD);
    DMSDPProfiles params;
    size_t cardid_len = strlen(cardid);
    int profileIndex = 0;
    int32_t r = DMSDP_OK;
    int fd = -1;
    int i;

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

    fd = open("test.pcm", O_WRONLY | O_CREAT, 0644);

    DMSDPVirtualStreamDataHeader header;
    DMSDPVirtualStreamData data;
    data.data = (uint8_t *) malloc(1024);
    header.length = 1024;
    data.header = &header;
    for(i = 0; i < 128; i++) {
        memset(data.data, 0, 1024);
        r = ReadStreamBuffer(cardid, cardid_len, 0, &data);
        write(fd, data.data, data.header->length);
        if (r != DMSDP_OK) {
            printf("record error!\n");
            break;
        }
    }
    close(fd);
    CloseAudioRecord(HSAE_MIC_CARD, strlen(HSAE_MIC_CARD), 0);
    return 0;
}
