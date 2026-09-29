#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"
#include "LightSensor.h"

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	
	uint8_t LightSensorNum;
	
	while(1)
	{
		LightSensorNum = LightSensor_GetNum();
		if(LightSensorNum==1)
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
}
