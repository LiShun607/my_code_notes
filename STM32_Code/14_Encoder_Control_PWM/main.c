#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Encoder.h"

int16_t Count;

int main(void)
{
	OLED_Init();
	PWM_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "CCR/(ARR+1):");
	OLED_ShowChar(1, 16,'%');
	
	while(1)
	{
		Count = Encoder_GetCount();
		OLED_ShowSignedNum(1, 13, Count,3);
		if(Count < 0)
		{
			OLED_ShowString(2, 1, "min,LED close!");
		}
		else if(Count < 101)
		{
			PWM_SetCompare1(Count);
			OLED_ShowString(2, 1, "              ");
		}
		else
		{
			OLED_ShowString(2, 1, "              ");
			OLED_ShowString(2, 1, "max!");
		}
	}
}
