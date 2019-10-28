#include "sys.h"
#include "usart3.h"
#include "delay.h"


OS_EVENT * sem_rpcover;
	  
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收到的数据长度
u8 USART3_RX_CNT=0;  
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void USART3_init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);//使能GPIOB,D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //PD8端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
#if 1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//停止复位
#endif
	
 #ifdef EN_USART3_RX		  	//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

	USART_Init(USART3, &USART_InitStructure); ; //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
   
	USART_Cmd(USART3, ENABLE);                    //使能串口 
 #endif

  	USART3_TX_EN=0;			//默认为接收模式 

}

void USART3_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
#if 1
  if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
	{
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_PE);
		USART3_RX_STA = 0;
		USART3_RX_CNT = 0;
	}
    
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
 	{
 		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_ORE);
		USART3_RX_STA = 0;
		USART3_RX_CNT = 0;
	}
    
	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
	{
		USART_ReceiveData(USART3);
 		USART_ClearFlag(USART3, USART_FLAG_FE);
		USART3_RX_STA = 0;
		USART3_RX_CNT = 0;
	}
 #endif 	
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 	if(USART3_RX_STA != 0x8000)
		{			
			res =USART_ReceiveData(USART3); 	//读取接收到的数据
			if(USART3_RX_CNT<USART3_REC_LEN)
			{
				USART3_RX_BUF[USART3_RX_CNT]=res;		//记录接收到的值
				USART3_RX_CNT++;						//接收数据增加1 
				if(USART3_RX_CNT >= 17)
				{
					if((USART3_RX_STA == 0)&&(res == 0x0D || res == 0x0A))
					{
						USART3_RX_STA = 0x4000;
					}
					if((USART3_RX_STA == 0x4000)&&(res == 0x0D || res == 0x0A))
					{
						USART3_RX_STA = 0x8000;
						USART3_RX_CNT = 16;
						OSSemPost(sem_rpcover);
					}
					else
					{
						USART3_RX_STA = 0;
						USART3_RX_CNT = 0;
					}
				}
			} 
			else
			{
				USART3_RX_STA = 0;
				USART3_RX_CNT = 0;
			}
		}
	}  	

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif	
} 
#endif		

void USART3_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART3_TX_EN=1;			//设置为发送模式
	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	
#if 1
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	USART_SendData(USART3, 0x0D);	
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	USART_SendData(USART3, 0x0A);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	USART_SendData(USART3, 0x0D);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	USART_SendData(USART3, 0x0A);
#endif
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	USART3_RX_CNT=0;	  
	USART3_TX_EN=0;				//设置为接收模式	
}

//buf:接收缓存首地址
//len:读到的数据长度
void USART3_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=USART3_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	if(rxlen==USART3_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];
		}		
		*len=USART3_RX_CNT;	//记录本次数据长度
		USART3_RX_STA = 0;
		USART3_RX_CNT=0;		//清零
	}
}

