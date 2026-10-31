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
	
	while(1)
	{
		//在主循环中不断开始转换转运，并不断读取转运到ADValue数组里的数据
		AD_Transfer();
		
		OLED_ShowNum(1, 10, ADValue[0], 4);
		OLED_ShowNum(2, 10, ADValue[1], 4);
		OLED_ShowNum(3, 10, ADValue[2], 4);
		
		Delay_ms(100);
	}
}
