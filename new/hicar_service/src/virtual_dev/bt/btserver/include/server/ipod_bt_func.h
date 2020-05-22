#ifndef _BT_FUN_H_
#define _BT_FUN_H_
#include<stdlib.h>

#include "ipod_def.h"

/*
 * ����������ݷ��ͺ�������
 */
typedef void (*BtSendData)(int cDataLen , unsigned char * pData);

/*
 * �����������ͺ���
 */
extern BtSendData bt_send_data;
/*
 * ע��������ݷ��ͺ���
 */
extern void ipodRegistBtSppSendData(BtSendData n_bt_spp_send_data);
/*
 * ����RFCOMM��ݽ��մ��?��
 */
int ipod_BT_spp_data_pro(int cDataLen, unsigned char * pData);


typedef enum __BtSppState
{
    ipod_SppState_ReadyNoInitialized = 0X00,
    ipod_SppState_Ready = 0X01,
    ipod_SppState_Connecting = 0X02,
    ipod_SppState_Connected = 0X03,
    ipod_SppState_ErrorSlcAlreadyEstablished = 0X92
} BtSppState;

/*
 *	����spp����״̬�ı�֪ͨ����
 */
void ipod_bt_spp_state_change(BtSppState state);

/*
 *	��⵱ǰ����״̬
 *	���� ��0 ��BT�Ѿ�����   -1��BTδ����
 */
int ipod_check_current_bt_is_connected(void);

extern void ipod_set_bluetooth_mac_address(char *address);
#endif
