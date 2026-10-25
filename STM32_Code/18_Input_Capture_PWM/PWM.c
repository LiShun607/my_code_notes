#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
	//打开TIM2定时器以及输出PWM波的GPIO口的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO口
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置TIM2为内部时钟源模式
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 100 - 1;    //ARR
	TIM_InitStructure.TIM_Prescaler = 720 - 1; //PSC   这里对应PWM的频率为1000Hz,分辨率为1%
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	
	//配置输出比较通道1
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;   //CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	//使能定时器
	TIM_Cmd(TIM2,ENABLE);
}

//修改输出比较通道1中CCR的值，目的是可以在运行中调用此函数不断修改CCR的值，从而改变占空比
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

//修改输出比较通道1中PSC的值，从而改变PWM波形频率
void PWM_SetPrescaler1(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler,TIM_PSCReloadMode_Update);
}
	