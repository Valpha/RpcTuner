#include "TEA5767.h"
#include "myiic.h"
#include "delay.h"
#include "usart.h"

static tea5767_writebytes_st radio_data={0x29,0xC2,0x60,0x11,0x00};
static tea5767_readbytes_st read_data;

//初始化IIC接口
void TEA5767_Init(void)
{
	IIC_Init();
	TEA5767_Write();
}
	    
/**********************************************************
** 函数名:TEA5767_Write
** 功能描述:向TEA5767写入5个字节数据
** 输入参数:无
** 输出参数:无
** 返    回:无
***********************************************************/
void TEA5767_Write(void)
{
    unsigned char i;
    IIC_Start(); //发送起始信号
    IIC_Send_Byte(0xc0);        //TEA5767写地址
    IIC_Wait_Ack();	  //等待应答
    for(i=0;i<5;i++)
    {
        IIC_Send_Byte(((u8*)&radio_data)[i]);//连续写入5个字节数据
        IIC_Ack(); //发送应答
    }
    IIC_Stop(); //发送停止信号   
}

/**********************************************************
** 函数名:Get_PLL
** 功能描述:由频率值计算PLL
** 输入参数:无
** 输出参数:pll--得到PLL值
** 返    回:无
***********************************************************/
u32 TEA5767_GetPLL(u32 frequency, u8 hlsi)
{
    u32 pll = 0;
    if (hlsi)
        pll=(unsigned int)((float)((frequency+225)*4)/(float)32.768);    //频率单位:k
    else
        pll=(unsigned int)((float)((frequency-225)*4)/(float)32.768);    //频率单位:k

    return pll;
}


/**********************************************************
** 函数名:Get_Frequency
** 功能描述:由PLL计算频率
** 输入参数:无
** 输出参数:frequency--得到频率值
** 返    回:无
***********************************************************/
u32 TEA5767_GetFrequencybyPll(u32 npll, u8 hlsi)
{
    u32 frequency = 0;
    if(hlsi)
        frequency=(unsigned long)((float)(npll)*(float)8.192-225);    //频率单位:KHz
    else
        frequency=(unsigned long)((float)(npll)*(float)8.192+225);    //频率单位:KHz

    return frequency;
}

u32 TEA5767_GetFrequency(u8 hlsi)
{
    u8 temp_l,temp_h;
    u32 pll=0;
    u32 fre;
    TEA5767_Read();
    temp_l=((u8*)&read_data)[1];
    temp_h=((u8*)&read_data)[0];
    temp_h&=0x3f;
    pll=temp_h*256+temp_l;
    fre = TEA5767_GetFrequencybyPll(pll, hlsi);
    return fre;
}

/**********************************************************
** 函数名:Get_Frequency
** 功能描述:由PLL计算频率
** 输入参数:无
** 输出参数:frequency--得到频率值
** 返    回:无
***********************************************************/
void TEA5767_Read(void)
{
    unsigned char i;
    IIC_Start();
    IIC_Send_Byte(0xc1);        //TEA5767读地址
    IIC_Wait_Ack();
    for(i=0;i<5;i++)   //读取5个字节数据
    {
        ((u8*)&read_data)[i]=IIC_Read_Byte(1);//读取数据后，发送应答
    }
    IIC_Stop();
}


/**********************************************************
** 函数名:Set_Frequency
** 功能描述:设置电台频率
** 输入参数:fre--写入的电台频率值（kHz）
** 输出参数:无
** 返    回:无
***********************************************************/
void TEA5767_SetFrequency(u32 fre)
{
	u32 pll=0;
	pll = TEA5767_GetPLL(fre, 0);
	((u8*)&radio_data)[0]=pll/256;
	((u8*)&radio_data)[1]=pll%256;
 	TEA5767_Write();
}

void TEA5767_MuteOnSetFrequency(u32 fre, u8 hlsi)
{
	u32 pll=0;
	radio_data.byte3.wrbyte3bits.MS = 1;
	pll = TEA5767_GetPLL(fre, hlsi);
	((u8*)&radio_data)[0]=pll/256;
	((u8*)&radio_data)[1]=pll%256;
	radio_data.byte1.wrbyte1bits.MUTE = 0;
	radio_data.byte4.wrbyte4bits.SMUTE = 1;
	radio_data.byte3.wrbyte3bits.MR = 1;
	radio_data.byte3.wrbyte3bits.ML = 1;
 	TEA5767_Write();
}

void TEA5767_MuteOffSetFrequency(u32 fre, BOOLEAN hlsi)
{
	u32 pll=0;
	radio_data.byte3.wrbyte3bits.HLSI = hlsi;
	radio_data.byte3.wrbyte3bits.MS = 1;
	pll = TEA5767_GetPLL(fre, hlsi);
	((u8*)&radio_data)[0]=pll/256;
	((u8*)&radio_data)[1]=pll%256;
	radio_data.byte1.wrbyte1bits.MUTE = 0;
	radio_data.byte4.wrbyte4bits.SMUTE = 0;
	radio_data.byte3.wrbyte3bits.MR = 0;
	radio_data.byte3.wrbyte3bits.ML = 0;
 	TEA5767_Write();
}

u8 TEA5767_isRFOn(void)
{
	TEA5767_Read();
	if(read_data.byte1.rdbyte1bits.RF == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 TEA5767_isStereoOn(void)
{
	OSTimeDlyHMSM(0,0,0,200);
	TEA5767_Read();
	if(read_data.byte3.rdbyte3bits.STEREO == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 TEA5767_isBLFOn(void)
{
	TEA5767_Read();
	if(read_data.byte1.rdbyte1bits.BLF== 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 TEA5767_isHLSIOn(void)
{
	if(radio_data.byte3.wrbyte3bits.HLSI== 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


u8 TEA5767_getADCLevel(void)
{
	u8 level = 0;
	TEA5767_Read();
	level = (read_data.byte4.rdbyte4data>>4)&0x0F;
	return level;
}

void TEA5767_ManualSerachOnSetFrequency(u32 fre, u8 hlsi, u8 sud)
{
	u32 pll=0;
	radio_data.byte3.wrbyte3bits.HLSI = hlsi;
	radio_data.byte3.wrbyte3bits.SUD = sud;
	pll = TEA5767_GetPLL(fre, hlsi);
	((u8*)&radio_data)[0]=pll/256;
	((u8*)&radio_data)[1]=pll%256;
	radio_data.byte1.wrbyte1bits.MUTE = 0;
	radio_data.byte4.wrbyte4bits.SMUTE = 1;
	radio_data.byte3.wrbyte3bits.MR = 1;
	radio_data.byte3.wrbyte3bits.ML = 1;
	radio_data.byte1.wrbyte1bits.SM = 1;
 	TEA5767_Write();
}


void TEA5767_ManualSerachOffSetFrequency(u32 fre, u8 hlsi)
{
	u32 pll=0;
	radio_data.byte3.wrbyte3bits.HLSI = hlsi;
	radio_data.byte3.wrbyte3bits.SUD = 0;
	pll = TEA5767_GetPLL(fre, hlsi);
	((u8*)&radio_data)[0]=pll/256;
	((u8*)&radio_data)[1]=pll%256;
	radio_data.byte1.wrbyte1bits.MUTE = 0;
	radio_data.byte1.wrbyte1bits.SM = 0;
	radio_data.byte4.wrbyte4bits.SMUTE = 1;
	radio_data.byte3.wrbyte3bits.MR = 1;
	radio_data.byte3.wrbyte3bits.ML = 1;
 	TEA5767_Write();

}

u8 TEA5767_GetIF(void)
{

	u8 iflevel = 0;
	TEA5767_Read();
	iflevel = read_data.byte3.rdbyte3data&0x7F;
	return iflevel;

}

