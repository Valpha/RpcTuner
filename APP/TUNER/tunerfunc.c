/*******************************************************************************
*	File Name				:	tunerfunc.c
*	Model Name				:	TUNERAPP
*	Create Date				:	2019/03/19
*	Author/Corporation			:	hegf/DSS(Dalian Smart Spirit)
*
*	Abstract Description		:	TUNER Call TEA5767 Driver IF.
*
*------------------------------Revision History---------------------------------
*	No	Version		Date		Revised By		Item				Description
*	1	V0.01		19.03.19	hegf[DSS]		inital code	       inital create file
*
*******************************************************************************/

/*******************************************************************************
*	Include File Section
*******************************************************************************/
#include "tunerfunc.h"
#include "TEA5767.h"
#include "delay.h"
#include "usart3.h"

/*******************************************************************************
*	Macro Define Section
*******************************************************************************/

/*******************************************************************************
*	Struct Define Section
*******************************************************************************/

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
u8 TUNERAPP_SetFrequence(u32 auwFrequence)
{
	u8 rpccmddata[]={0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	TUNERAPP_PresetUp(auwFrequence);
	
	rpccmddata[3] = (auwFrequence>>16)&0xFF;
	rpccmddata[4] = (auwFrequence>>8)&0xFF;
	rpccmddata[5] = (auwFrequence)&0xFF;
	USART3_Send_Data(rpccmddata, 16);
	return TUNER_RET_OK;
}

u8 TUNERAPP_PresetUp(u32 auwFrequence)
{
	u8 highlevel = 0, lowlevel = 0;
	u8 rpccmddata[]={0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	TEA5767_MuteOnSetFrequency(auwFrequence+450, 1);
	while(!TEA5767_isRFOn());

	highlevel = TEA5767_getADCLevel();

	TEA5767_MuteOnSetFrequency(auwFrequence-450, 1);
	while((!TEA5767_isRFOn()));

	lowlevel = TEA5767_getADCLevel();

	if(highlevel < lowlevel)
	{
		TEA5767_MuteOffSetFrequency(auwFrequence, 1);
	}
	else
	{
		TEA5767_MuteOffSetFrequency(auwFrequence, 0);
	}
	
	rpccmddata[3] = (auwFrequence>>16)&0xFF;
	rpccmddata[4] = (auwFrequence>>8)&0xFF;
	rpccmddata[5] = (auwFrequence)&0xFF;
	USART3_Send_Data(rpccmddata, 16);
	
	return TUNER_RET_OK;
}

u32 TUNERAPP_ManualSearch(u32 auwFrequence, u8 aubMode)
{
	u32 auwFre = auwFrequence;
	u8 rpccmddata[]={0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	if(aubMode) //Search Up
	{
		auwFre += 100;
		TEA5767_ManualSerachOnSetFrequency(auwFre, TEA5767_isHLSIOn() , 1);
	}
	else  //Search Down
	{
		auwFre -= 100;
		TEA5767_ManualSerachOnSetFrequency(auwFre, TEA5767_isHLSIOn() , 0);
	}

	while((!TEA5767_isRFOn()))
	{
		OSTimeDlyHMSM(0, 0, 0, 20);
		auwFre = TEA5767_GetFrequency(TEA5767_isHLSIOn());
		rpccmddata[3] = (auwFre>>16)&0xFF;
		rpccmddata[4] = (auwFre>>8)&0xFF;
		rpccmddata[5] = (auwFre)&0xFF;
		USART3_Send_Data(rpccmddata, 16);
	}

	auwFre = TEA5767_GetFrequency(TEA5767_isHLSIOn());

	if(TEA5767_isBLFOn())
	{
		if(aubMode) //Search Up
		{
			auwFre = MIN_FREQUENCE;
		}
		else  //Search Down
		{
			auwFre = MAX_FREQUENCE;
		}
		return auwFre;
	}

	TEA5767_ManualSerachOffSetFrequency(auwFre, TEA5767_isHLSIOn());
	return auwFre;
}

u32 TUNERAPP_AutoSearch(void)
{

	return 0;
}
