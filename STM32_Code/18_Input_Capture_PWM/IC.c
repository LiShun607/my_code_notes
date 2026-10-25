#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	//开启TIM定时器时钟，开启接受输入信号的GPIO口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU; //配置为上拉电阻输入模式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置内部时钟源模式
	TIM_InternalClockConfig(TIM3);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 65536 - 1;    //ARR  
	                                             /**
	                                               *因为采用的是测周法测量频率，所以ARR要尽可能大，
	                                               *否则如果被测频率太小，则CNT容易溢出
												 */
	TIM_InitStructure.TIM_Prescaler = 72 - 1; //PSC   PSC来决定标准频率（标准频率=72MHz/PSC）
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_InitStructure);
	
	//配置输入捕获通道
	TIM_ICInitTypeDef IC_InitStructure;
	IC_InitStructure.TIM_Channel = TIM_Channel_1; //通道1
	IC_InitStructure.TIM_ICFilter = 0xF;          //滤波器：数字越大，滤波效果越好
	IC_InitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //触发极性选择
	IC_InitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
	IC_InitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //直接和交叉
	TIM_ICInit(TIM3,&IC_InitStructure);
	
	TIM_PWMIConfig(TIM3,&IC_InitStructure); //PWMI模式可以直接配置同一个引脚的另一个通道
	
	//从模式触发源选择
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	
	//从模式选择
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	//使能定时器
	TIM_Cmd(TIM3,ENABLE);
}

//通过调用该函数获得频率
uint16_t IC_GetFreq1(void)
{
	return (1000000/TIM_GetCapture1(TIM3) - 1); //测周法：Freq=标准频率/N
}

//通过调用该函数获得占空比
uint16_t IC_GetCompare2(void)
{
	return ((TIM_GetCapture2(TIM3)+1)*100)/TIM_GetCapture1(TIM3); //占空比=高电平计数值*100/周期计数值，高电平计数值+1是补偿捕获误差
}
