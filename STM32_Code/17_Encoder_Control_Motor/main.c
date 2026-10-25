#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"


int main(void)
{
	OLED_Init();
	Motor_Init();
	Encoder_Init();

	
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{	
		//下面两个if语句都是为了限制CCR超出范围（-100~100）
		if(Encoder_GetCount() > 100)
		{
			Motor_Speed(100);
			OLED_ShowString(2, 1, "Speed Max!");
			OLED_ShowSignedNum(1, 7, 100, 4);
		}
		else if(Encoder_GetCount() < -100)
		{
			Motor_Speed(-100);
			OLED_ShowString(2, 1, "Speed Max!");
			OLED_ShowSignedNum(1, 7, -100, 4);
		}
		else
		{
			Motor_Speed(Encoder_GetCount());
			OLED_ShowString(2, 1, "          "); //清除第二行，防止一直显示
			OLED_ShowSignedNum(1, 7, Encoder_GetCount(), 4);
		}
	}
}
