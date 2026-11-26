#include "stm32f10x.h"                  // Device header


/*
 *函数名：MySPI_W_SLK
 *功能：改变SLK的电平
 *参数：1或0，1代表高电平，0代表低电平
 */
void MySPI_W_SLK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

/*
 *函数名：MySPI_W_SS
 *功能：改变SS的电平
 *参数：1或0，1代表高电平，0代表低电平
 */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

/*
 *函数名：MySPI_W_MOSI
 *功能：改变MOSI的电平
 *参数：1或0，1代表高电平，0代表低电平
 */
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

/*
 *函数名：MySPI_R_MISO
 *功能：读取MISO的电平
 *返回值：1或0，1代表高电平，0代表低电平
 */
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

/*
 *函数名：MySPI_Init
 *功能：初始化软件模拟SPI所用的GPIO口
 *SPI模式：模式0，SLK默认为低电平，第一个边沿移入数据，第二个边沿移出数据
 */
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出脚配置为推挽输出
	                                                 //SPI规定：对于主机来说，输出脚须配置为推挽输出，输入脚须配置为浮空输入或上拉输入
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU; //输入脚配置为上拉输入
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	MySPI_W_SS(1); //SS默认为高电平，不选择任何从机
	MySPI_W_SLK(0); //SPI模式0：SLK默认为低电平
}

/*
 *函数名：MySPI_Start
 *功能：将SS电平由高置低，产生起始条件
 */
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

/*
 *函数名：MySPI_Stop
 *功能：将SS电平由低置高，产生停止条件
 */
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/*
 *函数名：MySPI_SwapByte
 *功能：主机与从机交换一个字节，从而实现主从机之间发送接收字节
 *参数：主机需要发送的字节
 *返回值：从机发送过来的字节
 */
uint8_t MySPI_SwapByte(uint8_t SendByte)
{
	uint8_t i;
	
	for(i = 0;i < 8;i ++) //一个字节8位，循环8次
	{
		MySPI_W_MOSI(SendByte & 0x80); //把发送的字节的最高位放到MOSI上
		                               //从机同时也会将数据最高位放到MISO上
		SendByte <<= 1; //字节右移一位
		MySPI_W_SLK(1); //SLK电平由低置高，产生第一个边沿
		if(MySPI_R_MISO() == 1){SendByte |= 0x01;} //第一个边沿移入数据
		MySPI_W_SLK(0); //SLK电平由高置低，产生第二个边沿，第二个边沿移出数据
	}
	return SendByte;
}
