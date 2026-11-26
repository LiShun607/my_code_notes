#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "W25Q64.h"

uint16_t i;
uint8_t MID; //用来存放厂商ID
uint16_t DID; //用来存放设备ID
uint8_t SendByteArray[] = {0x08, 0x02, 0x03, 0x04}; //需要发送的数据
uint8_t ReceiveByteArray[4]; //用来存放接收到的数据

int main(void)
{
	//初始化各模块
	OLED_Init();
	W25Q64_Init();
	
	W25Q64_GetID(&MID,&DID); //获取ID
	
	W25Q64_SectorErase(0x000000); //页编程(即写数据)之前，必须先擦除对应扇区
	W25Q64_PageProgram(0x000000,SendByteArray,4); //向W25Q64写数据
	W25Q64_ReadData(0x000000,ReceiveByteArray,4); //从W25Q64读数据
	
	//在OLED屏幕上显示固定部分
	OLED_ShowString(1, 1, "MID:");
	OLED_ShowString(1, 9, "DID:");
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 13, DID, 4);
	
	OLED_ShowString(2, 1, "Send:");
	OLED_ShowString(3, 1, "Rec:");
	
	//在OLED屏幕上显示数据
	for(i = 0;i < 4;i ++)
	{
		OLED_ShowHexNum(2, 6+i*3, SendByteArray[i], 2);
	}
	for(i = 0;i < 4;i ++)
	{
		OLED_ShowHexNum(3, 6+i*3, ReceiveByteArray[i], 2);
	}
	
	while(1)
	{
		
		
	}
}
