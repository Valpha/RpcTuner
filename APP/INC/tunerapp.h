#ifndef _TUNERAPP_H_
#define _TUNERAPP_H_

#include "sys.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"			
#endif
#include "rpcappcommon.h"
#include "rpcsocreq.h"
#include "rpccmdparser.h"

//����������
//�����������ȼ�
#define TUNER_TASK_PRIO       				7
//���������ջ��С
#define TUNER_STK_SIZE  					64
//�����ջ	
extern OS_STK TUNER_TASK_STK[TUNER_STK_SIZE];
//������
extern void tuner_task(void *pdata);

#define TUNER_PARA_ERR 	(0xFF)
#define TUNER_RET_OK		(0)

extern u8 wubSocTunerReq(RPCCmdInfo_ts* pstRPCCmdInfo);

#endif
