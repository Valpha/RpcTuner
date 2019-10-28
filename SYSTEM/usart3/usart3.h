#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h" 
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


#define USART3_REC_LEN 64

extern u8 USART3_RX_BUF[USART3_REC_LEN]; 		//���ջ���,���64���ֽ�
extern u8 USART3_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define USART3_TX_EN		PDout(8)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART3_RX 	1			//0,������;1,����.


void USART3_init(u32 bound);
void USART3_Send_Data(u8 *buf,u8 len);
void USART3_Receive_Data(u8 *buf,u8 *len);

extern OS_EVENT * sem_rpcover;		//

#endif


