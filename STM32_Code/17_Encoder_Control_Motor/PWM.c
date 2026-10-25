#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	//开启定时器TIM2时钟，开启GPIO口时钟（用来输出PWM波形）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//选择内部时钟源模式
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_Initstructure;
	TIM_Initstructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Initstructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_Initstructure.TIM_Period = 100-1; //ARR
	TIM_Initstructure.TIM_Prescaler = 36-1; //PSC
	TIM_Initstructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_Initstructure);
	
	//配置输出比较通道，这里选择的3号
	TIM_OCInitTypeDef TIM_OCInitstructure;
	TIM_OCStructInit(&TIM_OCInitstructure);
	TIM_OCInitstructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitstructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_Pulse = 0; //CCR
	TIM_OC3Init(TIM2,&TIM_OCInitstructure);
	
	//使能定时器
	TIM_Cmd(TIM2,ENABLE);
}

//该函数可以改变CCR的值，从而改变占空比
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}
