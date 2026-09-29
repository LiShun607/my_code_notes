#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	
	uint8_t KeyNum;
	
	while(1)
	{
		KeyNum = KEY_GetNum();
		if(KeyNum == 1)
		{
			LED1_Turn();
		}
		if(KeyNum == 2)
		{
			LED2_Turn();
		}
	}
}
