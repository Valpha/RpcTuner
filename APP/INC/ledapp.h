#ifndef _LEDAPP_H_
#define _LEDAPP_H_
#include "includes.h"

//LED任务
//设置任务优先级
#define LED_TASK_PRIO       			3 
//设置任务堆栈大小
#define LED_STK_SIZE  		    		64

//任务堆栈	
extern OS_STK LED_TASK_STK[LED_STK_SIZE];

//任务函数
extern void led_task(void *pdata);


#endif
