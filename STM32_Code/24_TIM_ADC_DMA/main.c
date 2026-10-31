#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	
	//显示固定语句
	OLED_ShowString(1, 1, "ADC1_IN0:");
	OLED_ShowString(2, 1, "ADC1_IN1:");
	OLED_ShowString(3, 1, "ADC1_IN2:");
	
	//在while循环中不断取出数组中的数据
	while(1)
	{
		OLED_ShowNum(1, 10, ADValue[0], 4);
		OLED_ShowNum(2, 10, ADValue[1], 4);
		OLED_ShowNum(3, 10, ADValue[2], 4);
	}
}
