#include "ledapp.h"
#include "delay.h"
#include "led.h"

//�����ջ	
OS_STK LED_TASK_STK[LED_STK_SIZE];

//LED����
void led_task(void *pdata)
{
	u8 t;
	while(1)
	{
		t++;
		delay_ms(10);
		if(t==8)LED0=1;	//LED0��
		if(t==100)		//LED0��
		{
			t=0;
			LED0=0;
		}
	}									 
}
