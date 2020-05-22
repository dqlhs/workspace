/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: log function implementation
 * Author: dongyin
 * Create: 2019-10-18
 */

#include "dmsdp_log.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

void DmsdpSdkLog(uint8_t level, const char* tag, const char *format, ...)
{
    char logInfo[DMSDP_SDK_DEBUG_BUFF_LEN];
    (void)level;
    va_list stVaList;
    va_start(stVaList, format);
    memset(logInfo, 0, sizeof(logInfo));
    struct timeval tv;  
    struct timezone tz;  
    struct tm *p;  

    gettimeofday(&tv, &tz);  
    p = localtime(&tv.tv_sec);  
    printf("%02d-%02d %02d:%02d:%02d.%03ld %5d %5ld", 1 + p->tm_mon, p->tm_mday,
        p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec / 1000l, getpid(), syscall(SYS_gettid));  
    
    printf(" [%s]", tag);
    vsprintf(logInfo, (const char *)format, stVaList);
    printf("%s\r\n", logInfo);
    va_end(stVaList);
    return;
}
