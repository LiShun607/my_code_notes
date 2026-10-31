#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启GPIO时钟以及ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//开启ADC的预分配，选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入模式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置规则组
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5); //选择55个采样周期，ADC转换频率=12.5个固定周期+55.5=68*(1/12MHz)=5.6us）
	
	//配置ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//校准ADC，固定的代码
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); //等待Reset校准完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); //等待Start校准完成
}

//获得ADC转换后的值
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //开始软件触发，调用一次此函数，触发一次转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET); //等待转换完成，当转换未完成，一直在这个循环，不执行下面操作
	return ADC_GetConversionValue(ADC1);
}
