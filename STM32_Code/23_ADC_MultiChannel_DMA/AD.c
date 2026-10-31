#include "stm32f10x.h"                  // Device header

uint16_t ADValue[3];

void AD_Init(void)
{
	//开启GPIO、ADC和DMA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//开启ADC预分配，这里选择6分频，ADCCLK = 72MHz/6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置GPIO,因为是多通道，故同时配置多个Pin口
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置规则组通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
	
	//配置ADC（单次转换扫描模式）
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //无硬件触发，即软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 3;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//配置DMA（不自动重装转运次数）
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //硬件触发
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADValue;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	//使能ADC1、ADC_DMA以及DMA
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//校准ADC，固定代码
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) ==SET); //等待Reset校准完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); //等待Start校准完成
}

//调用一次该函数，ADC转换开始一轮，同时DMA开始配合ADC转运一轮数据
void AD_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE); //设置转运次数时DMA必须处于失能状态
	DMA_SetCurrDataCounter(DMA1_Channel1,3); //设置转运次数为3
	DMA_Cmd(DMA1_Channel1,ENABLE); //使能DMA,可以开始转运
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //软件触发一次ADC转换
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); //转运始终在转换之后，故这里只需要等待转运完成，那么整个过程就都完成了
	DMA_ClearFlag(DMA1_FLAG_TC1); //清除标志位
}
