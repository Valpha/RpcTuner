#include "ledapp.h"
#include "delay.h"
#include "led.h"

//任务堆栈	
OS_STK LED_TASK_STK[LED_STK_SIZE];

//LED任务
void led_task(void *pdata)
{
	u8 t;
	while(1)
	{
		t++;
		delay_ms(10);
		if(t==8)LED0=1;	//LED0灭
		if(t==100)		//LED0亮
		{
			t=0;
			LED0=0;
		}
	}									 
}
