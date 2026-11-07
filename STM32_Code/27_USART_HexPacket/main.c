#include "stm32f10x.h"                  // Device header
#include "KEY.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum; //用来存放键码

int main(void)
{
	//初始化！注意：一定不要忘记初始化！！！
	OLED_Init();
	KEY_Init();
	Serial_Init();
	
	//给TxPacket一组原始数据
	TxPacket[0] = 0x00;
	TxPacket[1] = 0x01;
	TxPacket[2] = 0x02;
	TxPacket[3] = 0x03;
	
	//显示固定字符串部分
	OLED_ShowString(1, 1, "TxPacket:");
	OLED_ShowString(3, 1, "RxPacket:");
	
	while(1)
	{
		KeyNum = KEY_GetNum();
		
		//如果按下了按键，就将TxPacket原始数据加1，显示到屏幕上并且发送到串口
		if(KeyNum == 1)
		{
			for(int i = 0;i < 4;i ++)
			{
				TxPacket[i] ++;
			}
			OLED_ShowHexNum(2, 1, TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, TxPacket[2], 2);
			OLED_ShowHexNum(2, 10,TxPacket[3], 2);
			Serial_SendHexPacket(TxPacket);
		}
		
		//如果接收到了数据包，就显示到屏幕上
		if(Serial_RxPacketFlag == 1)
		{
			OLED_ShowHexNum(4, 1, RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, RxPacket[3], 2);
			
			Serial_RxPacketFlag = 0; //将该标志位清零
		}
	}
}
