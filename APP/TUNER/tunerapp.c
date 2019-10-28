#include "tunerapp.h"
#include "key.h"
#include "rpcapp.h"
#include "tunerfunc.h"


#define TUNER_RPC_SETFREQUENCE_OPECODE			(0x0001)
#define TUNER_RPC_MANUALSERACHUP_OPECODE		(0x0002)
#define TUNER_RPC_MANUALSERACHDOWN_OPECODE	(0x0004)
#define TUNER_RPC_GETFREQUENCE_OPECODE			(0x0003)

typedef enum{
	TUNER_RPC_SETFREQUENCE_INNEROPECODE=0x0000,
	TUNER_RPC_MANUALSERACHUP_INNEROPECODE,
	TUNER_RPC_MANUALSERACHDOWN_INNEROPECODE,
	TUNER_RPC_SOCREQ_INNEROPECODE_END
}TUNER_RPCSocReqInnerOpeCode_st;

typedef struct{
	u8 (*tunerProcFunc)(RPCCmdInfo_ts* astRPCCmd);
}TUNER_RPCSocReqProcTbl_st;

typedef struct{
	u16 opecode;
	TUNER_RPCSocReqInnerOpeCode_st inneropecode;
}TUNER_RPCSocOpeCodeTransTbl_st;

	
//任务堆栈	
OS_STK TUNER_TASK_STK[TUNER_STK_SIZE];
static OS_EVENT* nstTunerRpcMBox_p;

static TUNER_RPCSocReqInnerOpeCode_st nubGetRPCReqInnerOpeCode(u16 opecode);
static u8 nubRPCReqTunerProcEngine(RPCCmdInfo_ts* astRPCCmd);
static u8 nubPRCReqSetFrequnce(RPCCmdInfo_ts* astRPCCmd);
static u8 nubPRCReqManualSearchUp(RPCCmdInfo_ts* astRPCCmd);
static u8 nubPRCReqManualSearchDown(RPCCmdInfo_ts* astRPCCmd);

static TUNER_RPCSocOpeCodeTransTbl_st nstSocOpeCodeTransTbl[]=
{
	{TUNER_RPC_SETFREQUENCE_OPECODE, TUNER_RPC_SETFREQUENCE_INNEROPECODE},
	{TUNER_RPC_MANUALSERACHUP_OPECODE, TUNER_RPC_MANUALSERACHUP_INNEROPECODE},
	{TUNER_RPC_MANUALSERACHDOWN_OPECODE, TUNER_RPC_MANUALSERACHDOWN_INNEROPECODE},
};

#define TUNER_OPETRANS_TBL_SIZE(tbl) (sizeof(tbl)/sizeof(TUNER_RPCSocOpeCodeTransTbl_st))


static TUNER_RPCSocReqProcTbl_st nstRPCSocReqProcTbl[]=
{
	nubPRCReqSetFrequnce,
	nubPRCReqManualSearchUp,
	nubPRCReqManualSearchDown	
};

#define TUNER_RPCREQ_TBL_SIZE(tbl) (sizeof(tbl)/sizeof(TUNER_RPCSocReqProcTbl_st))

//收音机任务
void tuner_task(void *pdata)
{
	u8 err;
	RPCCmdInfo_ts* astRPCCmd_p=NULL;
	RPCCmdInfo_ts astRPCCmd;
	
	nstTunerRpcMBox_p = OSMboxCreate((void*)0);
	while(1)
	{
		astRPCCmd_p=(RPCCmdInfo_ts*)OSMboxPend(nstTunerRpcMBox_p,10,&err); 
		if(astRPCCmd_p!=NULL)
		{
			OS_MemCopy((u8*)&astRPCCmd, (u8*)astRPCCmd_p, sizeof(RPCCmdInfo_ts));
			OSMemPut(wstBufferPtr, (void*)astRPCCmd_p);
			nubRPCReqTunerProcEngine(&astRPCCmd);
		}
		
	}									 
} 

u8 wubSocTunerReq(RPCCmdInfo_ts* pstRPCCmdInfo)
{
	u8 err;
	
	if(NULL != pstRPCCmdInfo)
	{
		err = OSMboxPost(nstTunerRpcMBox_p, pstRPCCmdInfo);
		if(OS_ERR_NONE == err)
		{
			return TUNER_RET_OK;	
		}
		else
		{
			OSMemPut(wstBufferPtr, (void*)pstRPCCmdInfo);
			return TUNER_PARA_ERR;
		}
	}
	else
	{	
		return TUNER_PARA_ERR;
	}

}


u8 nubRPCReqTunerProcEngine(RPCCmdInfo_ts* astRPCCmd)
{
	TUNER_RPCSocReqInnerOpeCode_st astInnerOpeCode;
	if(NULL == astRPCCmd)
	{
		return TUNER_PARA_ERR;
	}

	astInnerOpeCode = nubGetRPCReqInnerOpeCode(astRPCCmd->uswOpeCode);

	if(astInnerOpeCode<= TUNER_RPC_SOCREQ_INNEROPECODE_END)
	{
		return nstRPCSocReqProcTbl[astInnerOpeCode].tunerProcFunc(astRPCCmd);
	}

	return TUNER_PARA_ERR;
}

u8 nubPRCReqSetFrequnce(RPCCmdInfo_ts* astRPCCmd)
{
	u32 freq = 0;
//	u8 stereo = 0;
	if(NULL == astRPCCmd)
	{
		return TUNER_PARA_ERR;
	}
	freq = (astRPCCmd->ubCmdData[0]<<16)|(astRPCCmd->ubCmdData[1]<<8)|(astRPCCmd->ubCmdData[2]);
	freq = freq&(0x00FFFFFF);
	//TUNERAPP_SetFrequence(freq);
	TUNERAPP_PresetUp(freq);
	//stereo = TEA5767_isStereoOn();
	return TUNER_RET_OK;
}

u8 nubPRCReqManualSearchUp(RPCCmdInfo_ts* astRPCCmd)
{
	u32 freq = 0;
	u32 auwSearchFreq = 0;
	//u8 step = 0;
	
	if(NULL == astRPCCmd)
	{
		return TUNER_PARA_ERR;
	}
	freq = (astRPCCmd->ubCmdData[0]<<16)|(astRPCCmd->ubCmdData[1]<<8)|(astRPCCmd->ubCmdData[2]);
	freq = freq&(0x00FFFFFF);
	//step = astRPCCmd->ubCmdData[3];
	auwSearchFreq = TUNERAPP_ManualSearch(freq, 1);
	TUNERAPP_PresetUp(auwSearchFreq);
	return TUNER_RET_OK;
}

u8 nubPRCReqManualSearchDown(RPCCmdInfo_ts* astRPCCmd)
{
	u32 freq = 0;
	u32 auwSearchFreq = 0;
//	u8 step = 0;
	
	if(NULL == astRPCCmd)
	{
		return TUNER_PARA_ERR;
	}
	freq = (astRPCCmd->ubCmdData[0]<<16)|(astRPCCmd->ubCmdData[1]<<8)|(astRPCCmd->ubCmdData[2]);
	freq = freq&(0x00FFFFFF);
	//step = astRPCCmd->ubCmdData[3];
	auwSearchFreq = TUNERAPP_ManualSearch(freq, 0);
	TUNERAPP_PresetUp(auwSearchFreq);
	return TUNER_RET_OK;
}

TUNER_RPCSocReqInnerOpeCode_st nubGetRPCReqInnerOpeCode(u16 opecode)
{
	u16 i = 0;
	for(i = 0; i<TUNER_RPCREQ_TBL_SIZE(nstSocOpeCodeTransTbl);i++)
	{
		if(opecode == nstSocOpeCodeTransTbl[i].opecode)
		{
			return nstSocOpeCodeTransTbl[i].inneropecode;
		}
	}
	return TUNER_RPC_SOCREQ_INNEROPECODE_END;
}
