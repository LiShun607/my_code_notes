#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//开启TIM3 RCC内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置为内部时钟源模式
	TIM_InternalClockConfig(TIM3);
	
	//配置TIM3时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//上升计次
	TIM_InitStructure.TIM_Period = 10000-1;
	TIM_InitStructure.TIM_Prescaler = 7200-1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	
	//清除标记位，防止一通电就触发一次中断，从1开始计时
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	//产生更新事件时，发送TRGO信号
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
		
	//使能TIM3定时器
	TIM_Cmd(TIM3,ENABLE);
}
