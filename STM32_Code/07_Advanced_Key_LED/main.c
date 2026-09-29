#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"

int main(void)
{
	LED_Init();
	KEY_Init();
	
	uint8_t KeyNum;
	uint8_t RunMode = 1;//1代表流水灯停止，0代表流水灯开始
	
	while(1)
	{
		KeyNum = KEY_GetNum();
		if(KeyNum==1)
		{
			if(RunMode==1)
			{
				RunMode=0;
			}
			else
			{
				Led_Close();
				RunMode=1;
			}
		}
		if(RunMode==0)
		{
			LED_Flow();
		}
	}
}
