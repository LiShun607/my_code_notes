#include "stm32f10x.h"                  // Device header

uint8_t Buffersize; //定义为全局变量，方便在不同的函数里都能用

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint8_t Size)
{
	Buffersize = Size; //将Size更新到Buffersize
	
	//开启DMA1的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//配置DMA1
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA; //外设站点地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设站点的数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable; //外设站点地址是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB; //存储站点的地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储站点的数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储站点地址是否自增
	DMA_InitStructure.DMA_BufferSize = Size; //设置转运的次数
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //选择转运方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; //选择软件触发还是硬件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //选择是否自动重装转运次数（自动重装和软件触发不能同时存在）
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //设置优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	//失能DMA1
	DMA_Cmd(DMA1_Channel1,DISABLE);
}

//该函数调用一次，DMA转换一轮
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);//在设置转运次数之前DMA必须处于失能状态
	DMA_SetCurrDataCounter(DMA1_Channel1,Buffersize); //设置转运次数
	DMA_Cmd(DMA1_Channel1,ENABLE); //使能DMA，三个条件满足，DMA开始转运
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); //等待DMA转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1); //清除标志位
}
