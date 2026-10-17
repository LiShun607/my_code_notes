#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint8_t i=0;

int main(void)
{
	OLED_Init();
	PWM_Init();
	
	OLED_ShowString(1, 1, "CCR/(ARR+1):");
	
	while(1)
	{
		for(i=0;i<=100;i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
			OLED_ShowNum(1, 13, i, 3);
		}
		for(i=0;i<=100;i++)
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
			OLED_ShowNum(1, 13, 100-i, 3);
		}
	}
}
