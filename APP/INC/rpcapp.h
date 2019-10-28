#ifndef _RPCAPP_H_
#define _RPCAPP_H_

#include "includes.h"
#include "delay.h"
#include "key.h"
#include "usart3.h"
#include "rpcappcommon.h"
#include "rpcsocreq.h"
#include "rpccmdparser.h"

//����ɨ������
//�����������ȼ�
#define RPC_TASK_PRIO       			6 
//���������ջ��С
#define RPC_STK_SIZE  					64
//�����ջ	
extern OS_STK RPC_TASK_STK[RPC_STK_SIZE];
//������
extern void rpc_task(void *pdata);
extern OS_EVENT * msg_rpc;

typedef struct {
	u8 receivebuffer[64];
	u8 length;
	u8 conbuffer[64];
}st_mb_rpc;

extern u8 wpvdBlock[16][sizeof(RPCCmdInfo_ts)];
extern OS_MEM* wstBufferPtr;

#endif
