/*
 * JenDebug.h
 *
 *  Created on: May 22, 2015
 *      Author: robin
 */

#ifndef __JEN_DEBUG_H_
#define __JEN_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE iGstAudioCaptureLogLevel //define valuable name !

extern int DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE;
//============================================================================================================// Debug logger
#define     ERROR_LEVEL 0
#define     WARN_LEVEL  1
#define     INFO_LEVEL  2
#define     DEBUG_LEVEL 3
#define     LOG_LEVEL   4

#define ERROR(format, ...)                                  \
    do{                                                     \
        if (DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE >= ERROR_LEVEL)\
        {                                                   \
            fprintf(stdout, "\e[31m[JEN ERROR]: \e[0m");    \
            fprintf(stdout, format, ##__VA_ARGS__);         \
            fflush(stdout);                                 \
        }                                                   \
    }while(0)

#define WARN(format, ...)                                   \
    do{                                                     \
        if (DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE >= WARN_LEVEL) \
        {                                                   \
            fprintf(stdout, "\e[35m[JEN WARN]: \e[0m");     \
            fprintf(stdout, format, ##__VA_ARGS__);         \
            fflush(stdout);                                 \
        }                                                   \
    }while(0)

#define INFO(format, ...)                                   \
    do{                                                     \
        if (DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE >= INFO_LEVEL) \
        {                                                   \
            fprintf(stdout, "\e[33m[JEN INFO]: \e[0m");     \
            fprintf(stdout, format, ##__VA_ARGS__);         \
            fflush(stdout);                                 \
        }                                                   \
    }while(0)

#define DEBUG(format, ...)                                  \
    do{                                                     \
        if (DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE >= DEBUG_LEVEL)\
        {                                                   \
            fprintf(stdout, "\e[32m[JEN DEBUG]: \e[0m");    \
            fprintf(stdout, format, ##__VA_ARGS__);         \
            fflush(stdout);                                 \
        }                                                   \
    }while(0)

#define LOG(format, ...)                                    \
    do{                                                     \
        if (DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE >= LOG_LEVEL)  \
        {                                                   \
            fprintf(stdout, "\e[36m");                      \
            fprintf(stdout, format, ##__VA_ARGS__);         \
            fprintf(stdout, "\e[0m");                       \
            fflush(stdout);                                 \
        }                                                   \
    }while(0)

#define PRINT(format, ...)                                  \
    do{                                                     \
        fprintf(stdout, "\e[36m");                          \
        fprintf(stdout, format, ##__VA_ARGS__);             \
        fprintf(stdout, "\e[0m");                           \
        fflush(stdout);                                     \
    }while(0)

#define SetDebugLogLevel(env, level)                        \
    do{                                                     \
        if((NULL != env) && (NULL != getenv(env)))          \
        {                                                   \
            DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE = atoi(getenv(env));\
        }													\
		else												\
		{													\
			DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE = level;		\
		}													\
        printf("LogLevel: %d!\n", DEF_JEN_DEBUG_LOG_LEVEL_VALUABLE);\
    }while(0)

#endif /* __JEN_DEBUG_H_ */

#ifdef __cplusplus
}
#endif
