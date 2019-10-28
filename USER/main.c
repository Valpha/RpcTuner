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

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO 10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE 64
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//////////////////////////////////////////////////////////////////////////////

int main(void)
{
	delay_init();									//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	USART3_init(115200);

	LED_Init();																							 //��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();																						 //��������ʼ��
	KEY_Init();																							 //������ʼ��
	TEA5767_Init();																						 //Tuner��ʼ��
	OSInit();																							 //��ʼ��UCOSII
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO); //������ʼ����
	OSStart();
}

////////////////////////////////////////////////////////////////////////////////
//��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;

	sem_rpcover = OSSemCreate(0); //�����ź���
	OSStatInit();				  //��ʼ��ͳ������.�������ʱ1��������
	OS_ENTER_CRITICAL();		  //�����ٽ���(�޷����жϴ��)
	OSTaskCreate(led_task, (void *)0, (OS_STK *)&LED_TASK_STK[LED_STK_SIZE - 1], LED_TASK_PRIO);
	OSTaskCreate(tuner_task, (void *)0, (OS_STK *)&TUNER_TASK_STK[TUNER_STK_SIZE - 1], TUNER_TASK_PRIO);
	OSTaskCreate(rpc_task, (void *)0, (OS_STK *)&RPC_TASK_STK[RPC_STK_SIZE - 1], RPC_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}
