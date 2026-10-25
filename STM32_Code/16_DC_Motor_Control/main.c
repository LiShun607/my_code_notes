#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "KEY.h"

uint8_t KeyNum;
int16_t Speed = 0;

int main(void)
{
	OLED_Init();
	Motor_Init();
	KEY_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{
		KeyNum = KEY_GetNum();
		if(KeyNum == 1)
		{
			Speed = Speed + 20;
			//当达到最大占空比，也就是最大速度时，翻转电机方向
			if(Speed > 100)
			{
				Speed = -100;
			}
			Motor_Speed(Speed);
		}
		OLED_ShowSignedNum(1, 7, Speed, 4);
	}
}
