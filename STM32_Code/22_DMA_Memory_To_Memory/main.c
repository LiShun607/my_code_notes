#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

//定义两个数组DataA和DataB，目的将A中的数据转运到B中
uint16_t DataA[]= {0x01,0x02,0x03,0x04},DataB[] = {0,0,0,0};

int main(void)
{
	OLED_Init();
	MyDMA_Init((uint32_t)DataA,(uint32_t)DataB,4); //初始化DMA,并且确定外设站点和存储站点的地址以及转运多少次
	
	OLED_ShowString(1, 1, "DataA:");
	OLED_ShowString(3, 1, "DataB:");
	OLED_ShowHexNum(1, 7, (uint32_t)DataA, 8); //显示DataA的地址
	OLED_ShowHexNum(3, 7, (uint32_t)DataB, 8); //显示DataB的地址
	
	while(1)
	{
		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		
		Delay_ms(1000); //延迟1s方便观察
		
		MyDMA_Transfer(); //调用一次，DMA转换一轮
		
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000); //延迟1s方便观察
		
		//不断改变DataA元素的值
		for(int i =0;i<4;i++)
		{
			DataA[i] ++;
		}
    }
}
