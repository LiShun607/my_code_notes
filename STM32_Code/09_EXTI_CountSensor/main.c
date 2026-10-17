#include "stm32f10x.h"                  // Device header
#include "CountSensor.h"
#include "OLED.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	CountSensor_Init();
	
	OLED_ShowString(1,1,"Count:");
	
	while (1)
	{
		if(CountSensor_Flag == 1)  
		{
			//关键逻辑：确认现在确实处于遮挡状态（高电平1）
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==1)
			{
				Delay_ms(20);        //延迟消抖
				CountSensor_Count ++;//计数加一
				CountSensor_Flag = 0;//清除标记
			}
		}
		//屏幕上一直显示当前计数
		OLED_ShowNum(1,7,CountSensor_GetCount(),5);
	}
}
