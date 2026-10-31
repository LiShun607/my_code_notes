#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float Voltage;

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "ADC1:");
	OLED_ShowString(2, 1, "Vol:0.00V");
	
	while(1)
	{
		ADValue = AD_GetValue();
		Voltage = ((float)ADValue/4095)*3.3; //将ADC转换的值线性转换为电压值 0~4095 -> 0~3.3V
		OLED_ShowNum(1, 6, ADValue, 4);     //显示ADC转换值
		//由于该OLED代码里没有直接显示小数的函数，故用下面方法分别显示整数和小数
		OLED_ShowNum(2, 5, Voltage, 1);     //显示整数
		OLED_ShowNum(2, 7, (uint16_t)(Voltage*100)%100, 2);//显示小数
	} 
}
