#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	Serial_Init();
	
	OLED_ShowString(1, 1, "Sensor_Value:");
	
	while(1)
	{
		if(AD_Flag == 1)
		{
			uint16_t ADValue = ADC_GetConversionValue(ADC1);
			OLED_ShowNum(2, 1, ADValue, 4);
			Serial_Printf("Sensor_Value:%d\r\n",ADValue);
			
			AD_Flag = 0;
		}
	}
}
