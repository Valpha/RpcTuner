#include "rpcapp.h"

OS_STK RPC_TASK_STK[RPC_STK_SIZE];

u8 wpvdBlock[16][sizeof(RPCCmdInfo_ts)];
OS_MEM *wstBufferPtr = NULL;

static u8 nubRPCProcEngine(RPCAppModelInnerCode_ts astInnermodecode, RPCCmdInfo_ts *astRPCCmdInfo_p);

//RPCÍ¨ÐÅTask
void rpc_task(void *pdata)
{
	// u8 key;
	u8 err;
	RPCCmdInfo_ts *astRPCCmdInfo_p;
	u8 aubRPCCmdInfo[16];
	RPCAppModelInnerCode_ts astInnermodelcode;
	u8 length;
	wstBufferPtr = OSMemCreate((void *)wpvdBlock, 16, sizeof(RPCCmdInfo_ts), &err);

	while (1)
	{

		OSSemPend(sem_rpcover, 100, &err);
		if (OS_ERR_NONE == err)
		{
			OS_MemClr(aubRPCCmdInfo, sizeof(aubRPCCmdInfo));
			USART3_Receive_Data((u8 *)(&aubRPCCmdInfo), &length);
			if (length > 0)
			{
				//rpccmd.receivebuffer -> RPCCmdInfo_ts cmdinfo
				astRPCCmdInfo_p = wubGetPRCCmdInfo((u8 *)&aubRPCCmdInfo, length);
				if (astRPCCmdInfo_p != NULL)
				{
					USART3_Send_Data(aubRPCCmdInfo, length);
					astInnermodelcode = wubGetPRCInnerModelCode(astRPCCmdInfo_p->ubModelCode);
					nubRPCProcEngine(astInnermodelcode, astRPCCmdInfo_p);
				}
			}
		}

		// key = KEY_Scan(0);

		// if (key == KEY0_PRES)
		// {
		// 	//USART3_Send_Data(cmdbuffer, sizeof(cmdbuffer));
		// }
	}
}

u8 nubRPCProcEngine(RPCAppModelInnerCode_ts astInnermodecode, RPCCmdInfo_ts *astRPCCmdInfo_p)
{

	if (astInnermodecode < RPC_INNERMODELCODE_END)
	{
		if (NULL != astRPCCmdInfo_p && NULL != nstSocReqTbl[astInnermodecode].socReqFunc)
		{
			return nstSocReqTbl[astInnermodecode].socReqFunc(astRPCCmdInfo_p);
		}
		else
		{
			return RPCAPP_ERR;
		}
	}
	return RPCAPP_ERR;
}
