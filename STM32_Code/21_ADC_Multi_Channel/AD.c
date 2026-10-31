#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
	//开启GPIO和ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//开启ADC预分配，这里选择6分频，ADCCLK = 72MHz/6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置GPIO,因为是多通道，故同时配置多个Pin口
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//使能ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//校准ADC，固定代码
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) ==SET); //等待Reset校准完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); //等待Start校准完成
}

//该函数可以改变需要ADC转换的通道，并且获得ADC转换后的转换值
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5); //改变需要转换的通道
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //软件触发转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET); //等待转换完成
	return ADC_GetConversionValue(ADC1); //返回ADC转换后的值
}