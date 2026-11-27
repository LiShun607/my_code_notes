#include "stm32f10x.h"                  // Device header


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
 *函数名：MySPI_Init
 *功能：初始化硬件SPI
 *SPI模式：模式0，SLK默认为低电平，第一个边沿移入数据，第二个边沿移出数据
 */
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); //开启SPI1的时钟
	
	//------------- 配置GPIO ----------------
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP; //输出脚配置为复用推挽输出
	                               //硬件SPI规定：对于主机来说，输出脚须配置为复用推挽输出，输入脚须配置为浮空输入或上拉输入
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP; //这里仍然用软件来模拟SS，故配置为推挽输出
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU; //输入脚配置为上拉输入
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	//------------- 配置SPI ----------------
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //配置SCK的预分频系数
	                    //SCK频率=72MHz/该预分频系数（注意：SPI1在APB2总线上，故为72MHz;若用SPI2(挂载在APB1总线上)，则为36MHz）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //配置CPHA，1Edge代表SCK第一个边沿采样（模式0）
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //配置CPOL,Low代表SCK默认为低电平（模式0）
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC检验，这里给个默认值就行
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据帧
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //配置高位先行还是低位先行，MSB代表高位先行
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //配置SPI为主机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //SS用软件模拟
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
	
	MySPI_W_SS(1); //默认SS为高电平
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
 *时序：手册中硬件SPI非连续传输时序图
 */
uint8_t MySPI_SwapByte(uint8_t SendByte)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET); //等待TXE标志位
	                                                            //当TXE为1时，代表TXDR(发送数据寄存器)空，就可以发数据给TXDR
	SPI_I2S_SendData(SPI1,SendByte); //发数据给TXDR
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET); //等待RXNE标志位
	                                                               //当RXNE为1时，代表RXDR(接收数据寄存器)非空，即接收到了数据，就可以读取RXDR
	return SPI_I2S_ReceiveData(SPI1); //返回RXDR中接收到的数据
}
