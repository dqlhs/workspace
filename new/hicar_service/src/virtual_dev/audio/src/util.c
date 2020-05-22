//
// Created by prow on 2019/11/22.
//

#include "util.h"

#include <string.h>
#include <stdlib.h>
#include "dmsdp_log.h"

uint32_t getAudioParamFromDMSDPProfiles(const DMSDPProfiles *profiles, const char *key) {
    int i = 0;
    char key_dump[64] = {0};
    int key_dump_len = sizeof(key_dump) / sizeof(key_dump[0]);

    for (i = 0; i < profiles->num; i++) {
        if (profiles->profiles[i].keyLen > key_dump_len - 1) {
            DmsdpSdkLog(DMSDP_SDK_DBG_LEVEL_ERROR, "AudioHALUtil", "input key len is greater than 63.");
            return 0;
        }
        memcpy(key_dump, profiles->profiles[i].key, profiles->profiles[i].keyLen);
        key_dump[key_dump_len] = '\0';

        if (0 == strcmp(key_dump, key)) /* key found */{
            return profiles->profiles[i].value;
        }
        memset(key_dump, 0, key_dump_len);
    }
    return 0;
}
