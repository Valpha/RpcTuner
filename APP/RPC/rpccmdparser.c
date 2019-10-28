/*******************************************************************************
*	File Name				:	rpccmdparser.c
*	Model Name				:	RPCAPP
*	Create Date				:	2019/03/13
*	Author/Corporation			:	hegf/DSS(Dalian Smart Spirit)
*
*	Abstract Description		:	RPC Command Parser.
*
*------------------------------Revision History---------------------------------
*	No	Version		Date		Revised By		Item				Description
*	1	V0.01		19.03.13	hegf[DSS]		inital code	       inital create file
*
*******************************************************************************/

/*******************************************************************************
*	Include File Section
*******************************************************************************/
#include	"rpccmdparser.h"
#include 	"rpcappcommon.h"
#include  "rpcapp.h"


/*******************************************************************************
*	Macro Define Section
*******************************************************************************/

/*******************************************************************************
*	Struct Define Section
*******************************************************************************/
static RPCAPP_ModelCodeTransTab_ts nstModelCodeTransTbl[] = {
	{RPC_TUNER_MODELCODE,  RPC_TUNER_INNERMODELCODE},
	{RPC_LED_MODELCODE,  RPC_LED_INNERMODELCODE},
	{RPC_ALL_MODELCODE,  RPC_SOCALL_INNERMODELCODE},
};

#define RPCAPP_MODELCODETRANSTAB_SIZE(tbl) (sizeof(tbl)/sizeof(RPCAPP_ModelCodeTransTab_ts))

/*******************************************************************************
*	Static Prototype Declare Section
*******************************************************************************/

/*******************************************************************************
*	Global Variable Declare Section
*******************************************************************************/

/*******************************************************************************
*	File Static Variable Define Section
*******************************************************************************/

/*******************************************************************************
*	Function Definition Section
*******************************************************************************/
/*******************************************************************************
*	Function Name			:	wubGetPRCCmdInfo
*	Create Date				:	2019/03/13
*	Author/Corporation		 	:	hegf/DSS(Dalian Smart Spirit)
*
*	Description				:	Parse RPC  Cmd Info
*	Parameter				:	void
*	Return Code				:	RPC  Cmd Info
*
*------------------------------------------------------------------------------
*	Revision History
*	No.		Date			Revised by		Item		Description
*   001     2019/03/13        hegf         
*******************************************************************************/
RPCCmdInfo_ts* wubGetPRCCmdInfo(u8 *buf,u8 len)
{
	RPCCmdInfo_ts *astPRCCmdInfo_p = NULL;
	u8 err;
	if(NULL == buf || len <=0 || len>16)
	{
		return NULL;
	}

	astPRCCmdInfo_p = (RPCCmdInfo_ts*)OSMemGet(wstBufferPtr, &err);
	if(NULL != astPRCCmdInfo_p)
	{
		astPRCCmdInfo_p->ubModelCode = buf[0];
		astPRCCmdInfo_p->uswOpeCode = (buf[1]<<8)|buf[2];
		astPRCCmdInfo_p->uswCheckSum = (buf[14]<<8)|buf[15];
		OS_MemCopy(astPRCCmdInfo_p->ubCmdData, &(buf[3]), RPCDATALENGTH);
	}
	
	return astPRCCmdInfo_p;
}

/*******************************************************************************
*	Function Definition Section
*******************************************************************************/
/*******************************************************************************
*	Function Name			:	wubGetPRCInnerModelCode
*	Create Date				:	2019/03/15
*	Author/Corporation		 	:	hegf/DSS(Dalian Smart Spirit)
*
*	Description				:	Parse RPC  Cmd Info
*	Parameter				:	void
*	Return Code				:	RPC  Cmd Info
*
*------------------------------------------------------------------------------
*	Revision History
*	No.		Date			Revised by		Item		Description
*   001     2019/03/13        hegf         
*******************************************************************************/
RPCAppModelInnerCode_ts wubGetPRCInnerModelCode(u8 aubModelCode)
{
	u8 i = 0;

	for(i = 0; i<RPCAPP_MODELCODETRANSTAB_SIZE(nstModelCodeTransTbl); i++)
	{
		if(aubModelCode == nstModelCodeTransTbl[i].modecode)
		{
			return nstModelCodeTransTbl[i].inndercode;
		}
	}
	return RPC_INNERMODELCODE_END;
}

