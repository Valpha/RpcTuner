#ifndef _RPCAPP_H_
#define _RPCAPP_H_

#include "includes.h"
#include "delay.h"
#include "key.h"
#include "usart3.h"
#include "rpcappcommon.h"
#include "rpcsocreq.h"
#include "rpccmdparser.h"

//按键扫描任务
//设置任务优先级
#define RPC_TASK_PRIO       			6 
//设置任务堆栈大小
#define RPC_STK_SIZE  					64
//任务堆栈	
extern OS_STK RPC_TASK_STK[RPC_STK_SIZE];
//任务函数
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
