#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

int main(void)
{
	OLED_Init();
	AD_Init();

	//显示固定语句
	OLED_ShowString(1, 1, "ADC1_IN0:");
	OLED_ShowString(2, 1, "ADC1_IN1:");
	OLED_ShowString(3, 1, "ADC1_IN2:");
	OLED_ShowString(4, 1, "ADC1_IN3:");
	
	while(1)
	{
		//以下通过调用AD_GetValue改变通道，并且在屏幕上对应位置显示转换后的值
		OLED_ShowNum(1, 10, AD_GetValue(ADC_Channel_0), 4);
		OLED_ShowNum(2, 10, AD_GetValue(ADC_Channel_1), 4);
		OLED_ShowNum(3, 10, AD_GetValue(ADC_Channel_2), 4);
		OLED_ShowNum(4, 10, AD_GetValue(ADC_Channel_3), 4);
		
		Delay_ms(100);
	}
}
