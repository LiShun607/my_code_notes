#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "KEY.h"
#include "Timer.h"

uint8_t RunMode = 1;//1代表流水灯停止，0代表流水灯运行

int main(void)
{
	LED_Init();
	KEY_Init();
	Timer_Init();
	
	uint8_t KeyNum;
	
	//主循环里只负责读取按键并翻转灯的运行状态
	while(1)
	{
		KeyNum = KEY_GetNum();
		if(KeyNum==1) //说明按了一次按键
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
	}
}

//中断函数里，如果灯正在运行，就调用一次LED_Step函数，让灯移动一步
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		if(RunMode == 0)
		{
			LED_Step();
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
