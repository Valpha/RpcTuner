#ifndef _LEDAPP_H_
#define _LEDAPP_H_
#include "includes.h"

//LED����
//�����������ȼ�
#define LED_TASK_PRIO       			3 
//���������ջ��С
#define LED_STK_SIZE  		    		64

//�����ջ	
extern OS_STK LED_TASK_STK[LED_STK_SIZE];

//������
extern void led_task(void *pdata);


#endif
