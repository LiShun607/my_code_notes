#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

/*
 *函数名：W25Q64_Init
 *功能：初始化底层SPI，来使用软件SPI时序
 */
void W25Q64_Init(void)
{
	MySPI_Init();
}

/*
 *函数名：W25Q64_WriteEnable
 *功能：向W25Q64发送写使能指令
 */

void W25Q64_WriteEnable(void)
{
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_Write_Enable); //发送'写使能'指令（注意：这里我们的目的是发送指令给从机，故从机发过来的数据无需考虑）
	MySPI_Stop(); //结束时序
}

/*
 *函数名：W25Q64_WaitBusy
 *功能：等待W25Q64忙状态，该函数执行完成后则代表W25Q64Q处于不忙状态
 *      该函数还包含超时退出机制
 */
void W25Q64_WaitBusy(void)
{
	uint32_t Time = 10000; 
	
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_Read_Status_Register_1); //发送'读取W25Q64状态1'的指令
	while((MySPI_SwapByte(W25Q64_Dummy) & 0x01) == 0x01) //W25Q64状态寄存器1的最后一位代表Busy位，Busy为1则为忙，为0则为不忙
		                                                 //在while语句中，Busy为1，则进入while循环，为0则不进入while循环
	{
		Time --; //每进入一次while循环，Time -- ，相当于计时
		if(Time == 0)
		{
			break; //超时之后退出循环
		}
	}
	MySPI_Stop(); //停止时序
}

/*
 *函数名：W25Q64_GetID
 *功能：获取W25Q64的厂商ID和设备ID
 *参数：使用指针传递数据，参数为用来存放两个ID的地址指针
 */
void W25Q64_GetID(uint8_t *MID,uint16_t *DID)
{
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_JEDEC_ID); //向从机发送'读取ID'的指令
	*MID = MySPI_SwapByte(W25Q64_Dummy); //查W25Q64手册：发送该指令之后，第一个字节为厂商ID，将厂商ID存放在MID地址上
	                                     //（注意：这里我们的目的是获取从机发送的数据，故主机发给从机的数据不用考虑，就发送W25Q64_Dummy这个无意义字节）
	*DID = MySPI_SwapByte(W25Q64_Dummy); //查W25Q64手册：第二个字节为设备ID的高八位
	*DID = *DID << 8; //左移8位，将高8位移到DID的高8位
	*DID |= MySPI_SwapByte(W25Q64_Dummy); //查W25Q64手册：第三个字节为设备ID的低8位。通过'|'操作，将低8位放到DID的低8位
	MySPI_Stop(); //停止时序
}

/*
 *函数名：W25Q64_PageProgram
 *功能：页编程W25Q64，即向W25Q64指定位置写数据
 *参数：1.指定的首地址（32位地址）
 *      2.需要发送的字节数组的首地址
 *      3.需要发送的字节数目
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t *ByteArray, uint32_t Count)
{
	uint32_t i;
	
	W25Q64_WriteEnable(); //W25Q64页编程之前必须写使能
	
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_Page_Program); //发送'页编程'的指令
	MySPI_SwapByte(Address >> 16); //查W25Q64手册：发送该指令之后，主机发送的第一个字节为指定地址的高8位
	MySPI_SwapByte(Address >> 8);  //查W25Q64手册：发送该指令之后，主机发送的第二个字节为指定地址的中8位
	MySPI_SwapByte(Address);       //查W25Q64手册：发送该指令之后，主机发送的第三个字节为指定地址的低8位
	
	//查W25Q64手册：当指定地址发送完成之后，后面主机发送的数据即为有效数据
	//循环Count次，依次发送Count个字节
	for(i = 0;i < Count; i ++)
	{
		MySPI_SwapByte(ByteArray[i]); //发送一个字节给从机
	}
	MySPI_Stop(); //停止时序
	
	W25Q64_WaitBusy(); //事后等待Busy,这样只要W25Q64_PageProgram执行完成之后，W25Q64就处于不忙状态
}

/*
 *函数名：W25Q64_SectorErase
 *功能：擦除指定扇区（W25Q64规定，向指定地址写数据之前，必须先擦除对应扇区）
 *参数：指定的扇区首地址（32位地址）
 */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); //W25Q64擦除之前必须写使能
	
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_Sector_Erase); //发送'扇区擦除'指令
	MySPI_SwapByte(Address >> 16); //查W25Q64手册：发送该指令之后，主机发送的第一个字节为扇区地址的高8位
	MySPI_SwapByte(Address >> 8);  //查W25Q64手册：发送该指令之后，主机发送的第二个字节为扇区地址的中8位
	MySPI_SwapByte(Address);       //查W25Q64手册：发送该指令之后，主机发送的第三个字节为扇区地址的低8位
	MySPI_Stop(); //停止时序
	
	W25Q64_WaitBusy(); //事后等待Busy
}

/*
 *函数名：W25Q64_ReadData
 *功能：获取W25Q64中指定地址的数据
 *参数：1.指定地址的首地址（32位地址）
 *      2.存放获得的数据的字节数组首地址（用指针来传递数据）
        3.要读取的字节个数
 */
void W25Q64_ReadData(uint32_t Address, uint8_t *ByteArray, uint32_t Count)
{
	uint32_t i;
	
	MySPI_Start(); //开始时序
	MySPI_SwapByte(W25Q64_Read_Data); //发送'读取数据'的指令
	MySPI_SwapByte(Address >> 16); //查W25Q64手册：发送该指令之后，主机发送的第一个字节为指定地址的高8位
	MySPI_SwapByte(Address >> 8);  //查W25Q64手册：发送该指令之后，主机发送的第二个字节为指定地址的中8位
	MySPI_SwapByte(Address);       //查W25Q64手册：发送该指令之后，主机发送的第三个字节为指定地址的低8位
	
	//查W25Q64手册：当指定地址发送完成之后，后面的数据则为W25Q64发送过来的有效数据
	//循环Count次，依次接收Count个字节
	for(i = 0;i < Count; i ++)
	{
		ByteArray[i] = MySPI_SwapByte(W25Q64_Dummy); //将数据存放在数组中
	}
	MySPI_Stop(); //停止时序
}
