#include "stm32f10x.h"                  // Device header
#include "Timer.h"

uint8_t AD_Flag = 0;

void AD_Init(void)
{
	Timer_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入模式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	//校准ADC，固定的代码
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); //等待Reset校准完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); //等待Start校准完成
}

void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
	{
		AD_Flag = 1;
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	}
}
