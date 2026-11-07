#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxPacketFlag = 0; 
uint16_t TxPacket[4];
uint16_t RxPacket[4];


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
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU; //配置为上拉输入
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//配置USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600; //波特率，即1s传9600位，那传1位的时间=1/9600 s
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; //发送模式和接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶检验
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据8位
	USART_Init(USART1,&USART_InitStructure);
	
	//使能接收标志位中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
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

//发送HEX数据包函数
void Serial_SendHexPacket(uint16_t *TxPacket)
{
	Serial_SendByte(0xFF); //发送包头
	Serial_SendArray(TxPacket,4); //发送数据
	Serial_SendByte(0xFE); //发送包尾
}

//中断函数:使用状态机接收数据包
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0; //定义静态变量RxState,表示接收的状态
	static uint8_t CoRxPacket = 0; //定义静态变量CoRxPacket,表示RxPacket数组里的元素序号
	
	//先判断是否是USART1的RXNE标志位产生的中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//状态0，判断包头
		if(RxState == 0)
		{
			if(USART_ReceiveData(USART1) == 0xFF)
			{
				RxState = 1; //置于状态1
				CoRxPacket = 0; //清零CoRxPacket,保证每次进入状态1时，将接收到的第一个数据存入到RxPacket数组的第一个数
			}
		}
		
		//状态1，将有效数据存入数组
		else if(RxState == 1)
		{
			RxPacket[CoRxPacket] = USART_ReceiveData(USART1); //将接收到的数据存入数组
			CoRxPacket ++; 
			
			//这里是固定长度的数据包，故存了4个数据之后将状态置于2
			if(CoRxPacket >=4)
			{
				RxState = 2;
			}
		}
		
		//状态2，判断包尾
		else if(RxState == 2)
		{
			if(USART_ReceiveData(USART1) == 0xFE)
			{
				RxState = 0; //状态置0，回到开始状态
				Serial_RxPacketFlag = 1; //标志位：置1表示接收到了一个数据包，供主函数使用
			}
			
			//如果不是包尾，则说明发送过来的数据包不是按照规定包头、包尾发送的
			else
			{
				RxState = 0; //状态置0，回到开始状态
				Serial_Printf("Number Packet Error!"); //给一个反馈
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志位
		                                               //这里可以不用这个语句，因为在对USART_DR的读操作可以自动将该位清零。
	}
}
