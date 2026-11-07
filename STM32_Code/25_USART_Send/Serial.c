#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void)
{
	//开启USART1时钟和GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO,USART1的Tx、Rx分别对应PA9、PA10
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP; //配置为复用推挽输出
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600; //波特率，即1s传9600位，那传1位的时间=1/9600 s
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx; //发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶检验
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据8位
	USART_Init(USART1,&USART_InitStructure);
	
	//使能USART
	USART_Cmd(USART1,ENABLE);
}

//该函数可以发送一字节
void Serial_SendByte(uint8_t Byte)
{
	//USART1向串口发送字节函数
	USART_SendData(USART1,Byte);
	
	//等待发送数据寄存器置空
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

//该函数可以发送一数组
void Serial_SendArray(uint16_t *Array,uint8_t Length)
{
	uint8_t i;
	
	//不断循环调用SendByte函数，把数组里的每一个字节都发送出去
	for(i = 0;i < Length;i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

//该函数可以发送一字符串
void Serial_SendString(char *String)
{
	uint8_t i;
	for(i = 0;String[i]!='\0';i ++)
	{
		Serial_SendByte(String[i]);
	}
}

//该函数用来计算X的Y次方
uint32_t Serial_Pow(uint16_t X,uint16_t Y)
{
	uint16_t result = 1;
	while(Y --)
	{
		result *=X;
	}
	return result;
}

//该函数可以将数字以字符的形式发送出去，例如：输入12345，串口助手就会显示12345
void Serial_SendNum(uint32_t Num,uint16_t Length)
{
	uint8_t i;
	for(i = 0;i < Length;i ++)
	{
		Serial_SendByte(Num/Serial_Pow(10,Length - i - 1)%10 + 0x30);
	}
}

//以下两个函数是printf的重定向
//重写fputc函数
int fputc(int ch, FILE *f)
{
	//把原本要送去屏幕的字符，塞进串口发送函数里，直接通向串口
	Serial_SendByte(ch);
	return ch;
}

//封装一个串口打印函数
void Serial_Printf(char *format, ...)
{
	char String[100]; //内部定义一个足够大的缓冲区，防止内存溢出
	va_list arg; //定义一个参数列表，用来接收“...”里的变量
	va_start(arg,format); //开始读取可变参数
	vsprintf(String,format,arg); //核心：用vsprintf把格式化好的数据存入String
	va_end(arg); //结束读取
	
	Serial_SendString(String); //把拼好的字符串直接通过串口发出去
}
