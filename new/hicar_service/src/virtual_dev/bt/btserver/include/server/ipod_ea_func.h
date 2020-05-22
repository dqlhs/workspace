#ifndef _EA_FUN_H_
#define _EA_FUN_H_
#include<stdlib.h>
#include "ipod_def.h"

#define MAX_EA_SESSION_NUM  3

typedef enum
{
    EA_PROTOCOL_INDEX_RESVER = 0,
    HSAE_PROTOCOL_INDEX = 0x01,
    AUTO_NAVI_PROTOCOL_INDEX = 0x02
} EA_PROTOCOL_INDEX;

typedef struct
{
    EA_PROTOCOL_INDEX protocol_index;
    short sessionID ;
    int b_ea_datasession_open;
} EA_SESSION_PROTOCOL;

// ea session连接端 数组
EA_SESSION_PROTOCOL ea_seesion_protocol_array[MAX_EA_SESSION_NUM];

/*
 * 定义EA数据接收处理函数类型
 */
typedef void (*ipodEADataRecvProcess)(EA_PROTOCOL_INDEX protocol_index, int cDataLen , char * pData);

/*
 * 定义EA数据接收处理函数
 */
ipodEADataRecvProcess ea_recv_data_proc;
/*
 * 注册EA数据接收处理函数
 */
extern void ipod_register_ea_recived_data_proc(ipodEADataRecvProcess n_ea_recv_data_proc);
/*
 *	发送数据给ipod app端
 */
extern int ipod_send_data_to_app(int ipod_handle, EA_PROTOCOL_INDEX protocol_index, int data_len , char * data);
/*
 * 初始化EA数据发送函数
 */
void init_ea_recived_data_func(void);
/*
 *	启动ipod app
 */
int ipod_launcher_appliction(int ipod_handle, int stringLen, char * bundleIDString);
/*
 *	获取当前APP NAME
 */
int ipod_get_now_playing_appliction_name(int ipod_handle, char * name);

#if IPOD_IAP2_SUPPORT
typedef enum {
	EA_SESSION_STATUS_OK,
	EA_SESSION_CLOSE
}EASessionStatus;
#endif
#endif
