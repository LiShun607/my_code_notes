#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//开启TIM2 RCC内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置为内部时钟源模式
	TIM_InternalClockConfig(TIM2);
	
	//配置TIM2时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//上升计次
	TIM_InitStructure.TIM_Period = 10000-1;
	TIM_InitStructure.TIM_Prescaler = 7200-1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	//清除标记位，防止一通电就触发一次中断，从1开始计时
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//开启 TIM2 的更新中断
	//当计数器数满溢出时，触发中断，去执行 TIM2_IRQHandler 中断函数
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//使能TIM2定时器
	TIM_Cmd(TIM2,ENABLE);
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

//中断函数
//在中断函数里手动将TIM3的CNT清零，保证每次中断间隔时间内TIM3的CNT始终从0开始计数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_SetCounter(TIM3,0);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}