#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	//开启TIM2定时器通道，开启GPIO输出PWM波形通道
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//选择内部时钟源模式
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 20000-1; //ARR
	TIM_InitStructure.TIM_Prescaler = 72-1; //PSC  //该舵机工作在周期20ms的波形下
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	//配置输出比较通道2
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; //CCR
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	//使能定时器
	TIM_Cmd(TIM2,ENABLE);
}

//调用此函数来改变2号通道的占空比
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}