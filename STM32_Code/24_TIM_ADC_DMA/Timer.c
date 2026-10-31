#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	//开启TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置为内部时钟源模式
	TIM_InternalClockConfig(TIM3);
	
	//配置TIM3
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 10000 - 1; //ARR
	TIM_InitStructure.TIM_Prescaler = 7200 - 1; //PSC
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	
	//先清除一次标记为，减小 +-1 误差
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	//配置为产生更新时发起TRGO信号，至ADC
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	
	//使能TIM3定时器
	TIM_Cmd(TIM3,ENABLE);
}
