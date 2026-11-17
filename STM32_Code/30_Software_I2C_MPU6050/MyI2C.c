#include "stm32f10x.h"   // Device header
#include "Delay.h"

//初始化I2C,因为该项目用的是软件来模拟I2C，即需要通过软件置SCL和SDA高低电平，故初始化两个普通的GPIO口
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出：
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//上电后默认为高电平（即释放状态）
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,(BitAction)1);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)1);
}

//写SCL电平函数，1代表释放，0代表低电平
void MyI2C_W_SCL(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,(BitAction)BitVal); //写SCL电平
	Delay_us(10); //延迟10us,防止速度过快MPU6050来不及识别
}

//写SDA电平函数，1代表释放，0代表低电平
void MyI2C_W_SDA(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)BitVal); //写SDA电平
	Delay_us(10); //延迟10us,防止速度过快MPU6050来不及识别
}

//读SDA电平函数
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2); //读取输入到SDA的电平
	return BitValue;
}

/*
  I2C协议六要素：开始时序、结束时序、发送一个字节、接收一个字节、发送应答、接收应答
*/
//开始时序
void MyI2C_Start(void)
{
	MyI2C_W_SCL(1); //开始时SCL和SDA都置高电平（注意：SCL要先于SDA变换，防止误判为结束标志）
	MyI2C_W_SDA(1);
	MyI2C_W_SDA(0); //SCL高电平期间，SDA出现下降沿，则为开始标志
	MyI2C_W_SCL(0); //将SCL置低电平
}

//结束时序
void MyI2C_Stop(void)
{
	MyI2C_W_SCL(0); //结束时SCL和SDA都置低电平（注意：SCL要先于SDA变换，防止误判为开始标志）
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1); //将SCL置高电平
	MyI2C_W_SDA(1); //SCL高电平期间,SDA出现上升沿，则为结束标志
}

//发送一个字节
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	//for循环里循环8次，将需要发送的字节的8位从高到低依次发送出去
	for(i = 0;i < 8;i ++)
	{
		MyI2C_W_SCL(0); //SCL置低电平
		MyI2C_W_SDA((Byte << i)& 0x80); //SCL低电平期间将发送的一位放在SDA上
		MyI2C_W_SCL(1); //SCL置高电平，SCL高电平期间，从机将会读取SDA的电平（即读取发送的那一位）
	}
	MyI2C_W_SCL(0); //SCL置低电平
	MyI2C_W_SDA(0); //SDA置低电平
	                //发送字节后都置低电平，方便各板块的衔接
}

//接收一个字节
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Byte = 0x00, i; //Byte用来存储接收到的字节
	
	MyI2C_W_SDA(1); //主机先释放SDA的控制权，让从机来控制
	
	//for循环里循环8次，将从机发送的字节按高位到地位一位一位依次接收，并存储到Byte里
	for(i = 0;i < 8;i ++)
	{
		MyI2C_W_SCL(0); //SCL置低电平
		                //SCL低电平期间，从机会将发送的一位放在SDA上
		MyI2C_W_SCL(1); //SCL置高电平
		if(MyI2C_R_SDA()) {Byte |= (0x80 >> i);} //SCL高电平期间，主机将会读取SDA的电平（即接收从机发送的那一位）
                                                 /*
		                                            通过按位或'|'和(0x80 >> i)共同作用来实现接收对应的位
		                                            当接收到的SDA电平为0时，不进入if语句，对应的位不会改变，即为初始的0
		                                            当接收到的SDA电平为1时，进入if语句，通过|操作，对应的位就会变为1
		                                         */
	}
	MyI2C_W_SCL(0); //SCL置低电平
	MyI2C_W_SDA(0); //SDA置低电平
	                //接收字节后都置低电平，方便各板块的衔接
	return Byte;
}

//发送应答
void MyI2C_SendACK(uint8_t ACK)
{
	MyI2C_W_SDA((BitAction)ACK); //衔接时，SCL为低电平，SCL低电平期间将需要发送的应答放在SDA上
	MyI2C_W_SCL(1); //SCL置高电平，SCL高电平期间，从机将会读取SDA的电平（即读取主机发送的应答）
	
	MyI2C_W_SCL(0); //SCL置低电平
	MyI2C_W_SDA(0); //SDA置低电平
	                //发送应答后都置低电平，方便各板块的衔接
}

//接收应答
uint8_t MyI2C_ReceiveACK(void)
{
	uint8_t ACK; //ACK用来存储主机接收到的应答
	
	MyI2C_W_SDA(1); //主机释放SDA的控制权，此时，因为衔接，SCL为低电平
	                //SCL低电平期间：如果从机接收到了主机发送的字节，就会将SDA由高电平拉至低电平，表示通信没问题
	MyI2C_W_SCL(1); //SCL置高电平
	ACK = MyI2C_R_SDA(); //SCL高电平期间，读取SDA的电平，如果为 1，则说明从机未接收到数据；如果为 0，则说明从机接收到了数据
	
	MyI2C_W_SCL(0); //SCL置低电平
	MyI2C_W_SDA(0); //SDA置低电平
	                //接收应答后都置低电平，方便各板块的衔接
	return ACK;
}
