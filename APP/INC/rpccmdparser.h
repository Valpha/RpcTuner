/*******************************************************************************
*	File Name				:	rpccmdparser.h
*	Model Name				:	RPC
*	Create Date				:	2019/03/13
*	Author/Corporation		:	Hegf/DSS(Dalian SmartSpirit)
*
*	Abstract Description	:	Rpc Cmd Paser.
*
*------------------------------Revision History---------------------------------
*	No	Version		Date		Revised By		Item				Description
*	1	V0.01		19.03.13	hegf[DSS]		inital code	       inital create file
*
*******************************************************************************/

/*******************************************************************************
*	Multi-Include-Prevent Section
*******************************************************************************/
#ifndef	__RPCCMDPARSER_H
#define	__RPCCMDPARSER_H

/*******************************************************************************
*	Debug switch Section
*******************************************************************************/
/* Place some description here*/
#define	D_RPCAPP_BASE

/*******************************************************************************
*	Include File Section
*******************************************************************************/
#include  "sys.h"
#include 	"rpcappcommon.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"			
#endif
/*******************************************************************************
*	Macro Define Section
*******************************************************************************/

/*******************************************************************************
*	Struct Define Section
*******************************************************************************/
typedef struct{
	u8   ubModelCode;
	u16 uswOpeCode;
	u8   ubCmdData[RPCDATALENGTH];
	u16 uswCheckSum;
}RPCCmdInfo_ts;

typedef enum RPCAppModelInnerCode{
	RPC_TUNER_INNERMODELCODE = 0x00U,
	RPC_LED_INNERMODELCODE,
	RPC_SOCALL_INNERMODELCODE,
	RPC_INNERMODELCODE_END
}RPCAppModelInnerCode_ts;

/*******************************************************************************
*	Extern Declaration	
*******************************************************************************/
typedef struct{
	u8 modecode;
	RPCAppModelInnerCode_ts inndercode;
}RPCAPP_ModelCodeTransTab_ts;
/*******************************************************************************
*	Prototype Declare Section
*******************************************************************************/
extern RPCAppModelInnerCode_ts wubGetPRCInnerModelCode(u8 aubModelCode);
extern RPCCmdInfo_ts* wubGetPRCCmdInfo(u8 *buf,u8 len);


#endif /*__RPCCMDPARSER_H*/


