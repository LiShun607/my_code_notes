#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	while(1)
	{
		//判断如果进了中断（也就是代表接收到的数据已经存放到RxData），就可以取出RxData里的数据了
		if(Serial_RxFlag == 1)
		{
			OLED_ShowHexNum(1, 8, RxData, 2);
			Serial_SendByte(RxData);
			Serial_RxFlag = 0; //标志位清零
		}
	}
}
