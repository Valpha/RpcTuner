#include "sys.h"
#include "usart3.h"
#include "delay.h"


OS_EVENT * sem_rpcover;
	  
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//���յ������ݳ���
u8 USART3_RX_CNT=0;  
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  
  
void USART3_init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOB,Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //PD8�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
#if 1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ
#endif
	
 #ifdef EN_USART3_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); ; //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
   
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
 #endif

  	USART3_TX_EN=0;			//Ĭ��Ϊ����ģʽ 

}

void USART3_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
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
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 	if(USART3_RX_STA != 0x8000)
		{			
			res =USART_ReceiveData(USART3); 	//��ȡ���յ�������
			if(USART3_RX_CNT<USART3_REC_LEN)
			{
				USART3_RX_BUF[USART3_RX_CNT]=res;		//��¼���յ���ֵ
				USART3_RX_CNT++;						//������������1 
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

#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif	
} 
#endif		

void USART3_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART3_TX_EN=1;			//����Ϊ����ģʽ
	for(t=0;t<len;t++)		//ѭ����������
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
	USART3_TX_EN=0;				//����Ϊ����ģʽ	
}

//buf:���ջ����׵�ַ
//len:���������ݳ���
void USART3_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=USART3_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	if(rxlen==USART3_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];
		}		
		*len=USART3_RX_CNT;	//��¼�������ݳ���
		USART3_RX_STA = 0;
		USART3_RX_CNT=0;		//����
	}
}

