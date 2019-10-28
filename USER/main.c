#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "ledapp.h"
#include "rpcapp.h"
#include "tunerapp.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "TEA5767.h"
#include "usart3.h"
#include "rs485.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO 10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE 64
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//////////////////////////////////////////////////////////////////////////////

int main(void)
{
	delay_init();									//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级

	//uart_init(115200);	 	//串口初始化为115200
	USART3_init(115200);

	LED_Init();																							 //初始化与LED连接的硬件接口
	BEEP_Init();																						 //蜂鸣器初始化
	KEY_Init();																							 //按键初始化
	TEA5767_Init();																						 //Tuner初始化
	OSInit();																							 //初始化UCOSII
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO); //创建起始任务
	OSStart();
}

////////////////////////////////////////////////////////////////////////////////
//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;

	sem_rpcover = OSSemCreate(0); //创建信号量
	OSStatInit();				  //初始化统计任务.这里会延时1秒钟左右
	OS_ENTER_CRITICAL();		  //进入临界区(无法被中断打断)
	OSTaskCreate(led_task, (void *)0, (OS_STK *)&LED_TASK_STK[LED_STK_SIZE - 1], LED_TASK_PRIO);
	OSTaskCreate(tuner_task, (void *)0, (OS_STK *)&TUNER_TASK_STK[TUNER_STK_SIZE - 1], TUNER_TASK_PRIO);
	OSTaskCreate(rpc_task, (void *)0, (OS_STK *)&RPC_TASK_STK[RPC_STK_SIZE - 1], RPC_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO); //挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}
