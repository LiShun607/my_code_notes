#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_Clear();
	OLED_ShowString(1, 1, "TIM:");
	OLED_ShowString(2, 1, "CNT:");
	
	while(1)
	{
		OLED_ShowNum(1,5,Timer_GetCount(),5);
		OLED_ShowNum(2,5,TIM_GetCounter(TIM2),5);
	}
}
