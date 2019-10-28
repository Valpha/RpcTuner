/*******************************************************************************
*	File Name				:	rpcappcommon.h
*	Model Name				:	RPC
*	Create Date				:	2019/03/15
*	Author/Corporation		:	Hegf/DSS(Dalian SmartSpirit)
*
*	Abstract Description	:	Rpc Cmd Paser.
*
*------------------------------Revision History---------------------------------
*	No	Version		Date		Revised By		Item				Description
*	1	V0.01		19.03.15	hegf[DSS]		inital code	       inital create file
*
*******************************************************************************/

/*******************************************************************************
*	Multi-Include-Prevent Section
*******************************************************************************/
#ifndef	__RPCAPPCOMMON_H
#define	__RPCAPPCOMMON_H

/*******************************************************************************
*	Debug switch Section
*******************************************************************************/
/* Place some description here*/
#define	D_RPCAPP_BASE

/*******************************************************************************
*	Include File Section
*******************************************************************************/
#include "sys.h"

/*******************************************************************************
*	Macro Define Section
*******************************************************************************/
#define RPC_TUNER_MODELCODE					0x01U
#define RPC_LED_MODELCODE						0x02U
#define RPC_ALL_MODELCODE						0xFEU

#define RPCAPP_ERR								(0xFF)
#define RPCAPP_OK								(0)

#define RPCDATALENGTH							(11)
/*******************************************************************************
*	Struct Define Section
*******************************************************************************/

/*******************************************************************************
*	Extern Declaration	
*******************************************************************************/

/*******************************************************************************
*	Prototype Declare Section
*******************************************************************************/


#endif /*__RPCSOCREQ_H*/


