#include "stm32f10x.h"                  // Device header


void Encoder_Init(void)
{
	//开启TIM3时钟，开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置为内部时钟源模式
	TIM_InternalClockConfig(TIM3);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; //ARR
	                                                  //因为采用的测频法测速度，所以ARR要尽可能大，防止速度（频率）过快导致CNT计数值溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//同时配置输入捕获通道1、2的前两项
	TIM_ICInitTypeDef TIM_ICInitStrcuture;
	TIM_ICStructInit(&TIM_ICInitStrcuture);
	TIM_ICInitStrcuture.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStrcuture.TIM_ICFilter = 0xF;
	TIM_ICInitStrcuture.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInit(TIM3,&TIM_ICInitStrcuture);
	TIM_ICInitStrcuture.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStrcuture.TIM_ICFilter = 0xF;
	TIM_ICInitStrcuture.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInit(TIM3,&TIM_ICInitStrcuture);
	
	//配置编码器接口
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	//使能定时器
	TIM_Cmd(TIM3,ENABLE);
}

//该函数可以获得速度
//Speed=TIM3的CNT/TIM2定时中断时间
//正转时，CNT从0开始加，直接就是速度
//反转时，CNT从65535开始减，所以这里要用int16_t强制转换为速度（带负号）
int16_t Encoder_Speed(void)
{
	return (TIM_GetCounter(TIM3)/1);
}