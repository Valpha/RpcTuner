#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h" 
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


#define USART3_REC_LEN 64

extern u8 USART3_RX_BUF[USART3_REC_LEN]; 		//接收缓冲,最大64个字节
extern u8 USART3_RX_CNT;   			//接收到的数据长度

//模式控制
#define USART3_TX_EN		PDout(8)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART3_RX 	1			//0,不接收;1,接收.


void USART3_init(u32 bound);
void USART3_Send_Data(u8 *buf,u8 len);
void USART3_Receive_Data(u8 *buf,u8 *len);

extern OS_EVENT * sem_rpcover;		//

#endif


