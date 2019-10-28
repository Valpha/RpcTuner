#ifndef _TUNERAPP_H_
#define _TUNERAPP_H_

#include "sys.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"			
#endif
#include "rpcappcommon.h"
#include "rpcsocreq.h"
#include "rpccmdparser.h"

//收音机任务
//设置任务优先级
#define TUNER_TASK_PRIO       				7
//设置任务堆栈大小
#define TUNER_STK_SIZE  					64
//任务堆栈	
extern OS_STK TUNER_TASK_STK[TUNER_STK_SIZE];
//任务函数
extern void tuner_task(void *pdata);

#define TUNER_PARA_ERR 	(0xFF)
#define TUNER_RET_OK		(0)

extern u8 wubSocTunerReq(RPCCmdInfo_ts* pstRPCCmdInfo);

#endif
