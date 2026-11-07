#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>

int main(void)
{
	//初始化
	OLED_Init();
	Serial_Init();
	LED_Init();
	
	//显示固定语句
	OLED_ShowString(1, 1, "TxPacket:");
	OLED_ShowString(3, 1, "RxPacket:");
	
	while(1)
	{
		//如果接收到了数据包，就判断文本是否是规定的开关灯文本
		if(Serial_RxPacketFlag == 1)
		{
			OLED_ShowString(4, 1, "                "); //清空第四行
			OLED_ShowString(4, 1, RxTextPacket); //显示接收到的文本数据包
			
			//如果是LED_ON,就开灯
			if(strcmp("LED_ON",RxTextPacket) == RESET)
			{
				LED1_ON();
				OLED_ShowString(2, 1, "LED_ON_OK"); //显示反馈语句
				Serial_SendString("LED_ON_OK\r\n"); //发送一个反馈到串口
			}
			
			//如果是LED_OFF,就关灯
			else if(strcmp("LED_OFF",RxTextPacket) == RESET)
			{
				LED1_OFF();
				OLED_ShowString(2, 1, "LED_OFF_OK"); //显示反馈语句
				Serial_SendString("LED_OFF_OK\r\n"); //发送一个反馈到串口
			}
			
			//如果接收到的不是规定语句，仍然给一个反馈语句
			else
			{
				OLED_ShowString(2, 1, "ERROR COMMEND!");
				Serial_SendString("ERROR COMMEND!\r\n");
			}
			Serial_RxPacketFlag = 0; //将该标志位清零
		}
	}
}
