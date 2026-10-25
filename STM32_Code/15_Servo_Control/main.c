#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "KEY.h"
#include "OLED.h"
#include "Servo.h"

uint8_t KeyNum;
float Angle = 0;

int main(void)

{
	OLED_Init();
	KEY_Init();
	Servo_Init();
	
	OLED_ShowString(1, 1, "Angle:");
	Servo_SetAngle(Angle); //将舵机位置恢复默认
	
	while(1)
	{
		KeyNum = KEY_GetNum(); //不断获取键码，来判断是否按下按键
		if(KeyNum == 1)
		{
			Angle = Angle+30;
			if(Angle >180) //该舵机角度范围为0°~180°
			{
				Angle = 0;
			}
			Servo_SetAngle(Angle); //将角度传给函数，经过线性换算从而改变占空比
		}
		OLED_ShowNum(1, 7, Angle, 4);	
	}
}
