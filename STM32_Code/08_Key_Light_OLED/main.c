#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "OLED.h"
#include "LightSensor.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	OLED_Init();
	OLED_Clear();
	LightSensor_Init();
	
	OLED_ShowString(1, 1, "Key:");
	OLED_ShowString(2, 1, "LightSensor:");
	
	uint8_t KeyNum;
	uint8_t LightSensorNum;
	
	while(1)
	{
		KeyNum = KEY_GetNum();
		LightSensorNum = LightSensor_GetNum();
		if(KeyNum!=0)
		{
			OLED_ShowNum(1, 5, KeyNum, 1);
			if(KeyNum == 1)
			{
				LED1_Turn();		
			}
			if(KeyNum == 2)
			{
				LED2_Turn();
			}			
		}
		OLED_ShowNum(2, 13, LightSensorNum, 1);
	}
}
