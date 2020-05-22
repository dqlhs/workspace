/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2014

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
using namespace std;
extern int SHOW_LOG;
extern bool MODEL_DEVICE;
extern bool MODEL_HFP;
extern bool MODEL_MEDIA;
extern bool MODEL_PB;
extern bool MODEL_SET;
extern bool MODEL_SQL;
#define BT_MODULE_DEVICE   "[BtServer][DEVICE]:"
#define BT_MODULE_HFP      "[BtServer][HFP]:"
#define BT_MODULE_MEDIA    "[BtServer][MEDIA]:"
#define BT_MODULE_PB       "[BtServer][PB]:"
#define BT_MODULE_SET      "[BtServer][SET]:"
#define BT_MODULE_SQL      "[BtServer][SQL]:"
#define BT_LOG_DEVICE 1
#define BT_LOG_HFP 2
#define BT_LOG_MEDIA 3
#define BT_LOG_PB 4
#define BT_LOG_SET 5
#define BT_LOG_SQL 6

#define LOG(module,fmt, ...)\
if (module==BT_LOG_DEVICE)\
{\
    if (MODEL_DEVICE)        \
    {                                   \
        printf("\e[33m");          \
        printf(BT_MODULE_DEVICE);          \
        printf(__FUNCTION__);          \
        printf(" ");          \
        printf(fmt, ##__VA_ARGS__);  \
        printf("\e[0m");          \
        printf("\n");          \
        fflush(stdout); \
    }                                   \
 }\
if (module == BT_LOG_HFP)\
{\
        if (MODEL_HFP)        \
        {                                   \
            printf("\e[33m");          \
            printf(BT_MODULE_HFP);          \
            printf(__FUNCTION__);          \
            printf(" ");          \
            printf(fmt, ##__VA_ARGS__);  \
            printf("\e[0m");          \
            printf("\n");          \
            fflush(stdout); \
        }                                   \
}\
if (module == BT_LOG_MEDIA)\
{\
       if (MODEL_MEDIA)        \
       {                                   \
                printf("\e[33m");          \
                printf(BT_MODULE_MEDIA);          \
                printf(__FUNCTION__ );          \
                printf(" ");          \
                printf(fmt, ##__VA_ARGS__);  \
                printf("\e[0m");          \
                printf("\n");          \
                fflush(stdout); \
        }                                   \
 }\
if (module == BT_LOG_PB)\
 {\
            if (MODEL_PB)        \
            {                                   \
                printf("\e[33m");          \
                printf(BT_MODULE_PB);          \
                printf(__FUNCTION__ );          \
                printf(" ");          \
                printf(fmt, ##__VA_ARGS__);  \
                printf("\e[0m");          \
                printf("\n");          \
                fflush(stdout); \
            }                                   \
 }\
if (module == BT_LOG_SET)\
    {\
            if (MODEL_SET)        \
            {                                   \
                printf("\e[33m");          \
                printf(BT_MODULE_SET);          \
                printf(__FUNCTION__);          \
                printf(" ");          \
                printf(fmt, ##__VA_ARGS__);  \
                printf("\e[0m");          \
                printf("\n");          \
                fflush(stdout); \
            }                                   \
    }\
if (module == BT_LOG_SQL)\
 {\
                if (MODEL_SQL)        \
                {                                   \
                    printf("\e[33m");          \
                    printf(BT_MODULE_SQL);          \
                    printf(__FUNCTION__);          \
                     printf(" ");          \
                    printf(fmt, ##__VA_ARGS__);  \
                    printf("\e[0m");          \
                    printf("\n");          \
                    fflush(stdout); \
                }                                   \
 }

#endif // COMMON_H
