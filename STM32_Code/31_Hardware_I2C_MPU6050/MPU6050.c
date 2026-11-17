#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

// 注意：0xD0 是包含了最低位“写”标志位(0)的 8位地址。
#define MPU6050_Address			0xD0

/*
 * 函数名：MPU6050_WaitEvent
 * 功能：等待I2C时序里各事件产生，并且包含超时跳出功能，防止程序卡死
 * 参数：I2C1、I2C2;
         I2C时序里的事件名
 */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t WaitTime = 0; //用来计次，当计次达到10000次，就当作超时
	
	while(I2C_CheckEvent(I2Cx,I2C_EVENT)!= SUCCESS) //如果没有产生对应事件，就进while语句，产生了就跳过while语句
	{
		WaitTime ++; //计次加一
		if(WaitTime == 10000) 
		{
			break; //如果达到了10000次，就直接跳出while循环
		}
	}
}

/*
 * 函数名：MPU6050_WriteReg
 * 功能：向 MPU6050 的指定寄存器写入一个字节的数据
 * 时序（写）：Start -> 等待事件EV5 -> 器件地址(写) -> ACK(硬件自动接收) -> 等待事件EV6 -> 寄存器地址 -> ACK(硬件自动接收) ->
 *             等待事件EV8 -> 写入数据 -> ACK(硬件自动接收) -> 等待事件EV8_2 -> Stop(手册规定的主机发送时序)
 */
void MPU6050_WriteReg(uint8_t RegAddr,uint8_t SendByte)
{
	I2C_GenerateSTART(I2C2,ENABLE); //产生起始条件
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT); //等待EV5产生
	
	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Transmitter); //发送7位地址，以及配置读写位(写)
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); //等待EV6产生
	
	I2C_SendData(I2C2,RegAddr); //发送从机的寄存器地址（注意：MPU6050规定地址后的一个字节数据为主机需要寻找的寄存器地址））
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING); //等待EV8事件产生
	
	I2C_SendData(I2C2,SendByte); //发送一个字节数据
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED); //等待EV8_2产生
	
	I2C_GenerateSTOP(I2C2,ENABLE); //产生停止条件
}

/*
 * 函数名：MPU6050_ReadReg
 * 功能：读取 MPU6050 指定寄存器的一个字节数据
 * 时序（读）：Start -> 等待事件EV5 -> 器件地址(写) -> ACK(硬件自动接收) -> 等待事件EV6 -> 寄存器地址 -> 
 *             ACK(硬件自动接收) -> 等待事件EV8 -> 重复Start -> 等待事件EV5 -> 器件地址(读) -> ACK(硬件自动接收) -> 
 *             等待事件EV6 -> 提前失能ACK -> 提前产生停止条件 -> 等待事件EV7 -> 读数据 -> NACK(硬件自动发送) -> 
 *             使能ACK(变为默认状态)
 * 难点：1.读时序比写时序复杂，需要“写地址”来选中寄存器，再用“读地址”拿数据。
 *       2.手册规定，当读取的为最后一个字节时，要在读取最后这个字节之前先把ACK失能(也就是主机接收到最后一个字节后自动发
 *         送的是非应答)、先产生停止条件(最后一个字节接收完成之后自动产生停止条件，不会截断时序)
 * 注意：最后不要忘记把ACK使能变为默认状态
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	uint8_t ReceiveByte; //用来存放接收的一个字节
	
	I2C_GenerateSTART(I2C2,ENABLE); //产生起始条件
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT); //等待EV5产生
	
	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Transmitter); //发送7位地址，以及配置读写位(写)
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);  //等待EV6产生
	
	I2C_SendData(I2C2,RegAddr); //发送从机的寄存器地址（注意：MPU6050规定地址后的一个字节数据为主机需要寻找的寄存器地址））
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING); //等待EV8事件产生
	
	I2C_GenerateSTART(I2C2,ENABLE); //重复起始条件
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT); //等待EV5产生
	
	I2C_Send7bitAddress(I2C2,MPU6050_Address,I2C_Direction_Receiver); //发送7位地址，以及配置读写位(读)
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);  //等待EV6产生

	I2C_AcknowledgeConfig(I2C2,DISABLE); //接收的为最后一个字节，提前失能ACK
	I2C_GenerateSTOP(I2C2,ENABLE); //提前产生停止条件
	
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED); //等待EV7产生
	ReceiveByte = I2C_ReceiveData(I2C2); //接收数据
	
	I2C_AcknowledgeConfig(I2C2,ENABLE); //使能ACK，变为默认状态
	
	return ReceiveByte;
}

/*
 * 函数名：MPU6050_Init
 * 功能：MPU6050初始化配置
 * 逻辑：先初始化硬件I2C，再根据MPU6050数据手册，按需配置几个核心寄存器，使其脱离睡眠并开始按我们要求的频率和量程工作。
 */
void MPU6050_Init(void)
{
	// --- 初始化硬件I2C ---
	//开启I2C2和GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_OD; //复用开漏输出模式
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	
	//配置I2C2
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; //使能ACK，默认给应答（从机模式下）
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7位地址（从机模式下）
	I2C_InitStructure.I2C_ClockSpeed = 50000; //时钟速度（标准速度(高达100 kHz)、快速(高达400 kHz)）
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //占空比：低电平时间/高电平时间
	                                                   //（只在快速模式下有效，标准速度下占空比始终为1）
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; //I2C模式
	I2C_InitStructure.I2C_OwnAddress1 = 0X00; //作为从机时的自身地址（从机模式下）
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE); //使能I2C2
	
	// --- 初始化MPU6050 ---
	// 1. 电源管理1：解除睡眠，选择X轴陀螺仪PLL作为时钟源(0x01)
	MPU6050_WriteReg(PWR_MGMT_1, 0x01); 
	// 2. 电源管理2：让所有轴(加速度、陀螺仪)都不进入待机状态(0x00)
	MPU6050_WriteReg(PWR_MGMT_2, 0x00);
	// 3. 采样率分频：配置为 100Hz (1kHz / (1+9) = 100Hz)，即每秒输出100次数据
	MPU6050_WriteReg(SMPLRT_DIV, 0x09);
	// 4. 配置寄存器：设置数字低通滤波器(DLPF)，配置为6档，让数据更平滑
	MPU6050_WriteReg(CONFIG, 0x06);
	// 5. 陀螺仪配置：满量程选择 ±2000°/s (0x18)
	MPU6050_WriteReg(GYRO_CONFIG, 0x18);
	// 6. 加速度计配置：满量程选择 ±16g (0x18)
	MPU6050_WriteReg(ACCEL_CONFIG, 0x18);
}

/*
 * 函数名：MPU6050_GetData
 * 功能：读取六轴原始数据（加速度XYZ、陀螺仪XYZ）
 * 参数：使用指针传参，将读到的六个轴的数据回传给主函数
 * 注意：MPU6050输出的数据是16位的（分高8位和低8位），需要拼接。
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint16_t DataH, DataL; // 用来暂存高位和低位
	
	// --- 读取加速度计数据 ---
	DataH = MPU6050_ReadReg(ACCEL_XOUT_H); // 读 X 轴高 8 位
	DataL = MPU6050_ReadReg(ACCEL_XOUT_L); // 读 X 轴低 8 位
	*AccX = (DataH << 8) | DataL;          // 将高位左移8位，和低位拼成一个16位完整数据
	
	DataH = MPU6050_ReadReg(ACCEL_YOUT_H); // 读 Y 轴高 8 位
	DataL = MPU6050_ReadReg(ACCEL_YOUT_L); // 读 Y 轴低 8 位
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(ACCEL_ZOUT_H); // 读 Z 轴高 8 位
	DataL = MPU6050_ReadReg(ACCEL_ZOUT_L); // 读 Z 轴低 8 位
	*AccZ = (DataH << 8) | DataL;
	
	// --- 读取陀螺仪数据 ---
	DataH = MPU6050_ReadReg(GYRO_XOUT_H);  // 读 X 轴高 8 位
	DataL = MPU6050_ReadReg(GYRO_XOUT_L);  // 读 X 轴低 8 位
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(GYRO_YOUT_H);  // 读 Y 轴高 8 位
	DataL = MPU6050_ReadReg(GYRO_YOUT_L);  // 读 Y 轴低 8 位
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(GYRO_ZOUT_H);  // 读 Z 轴高 8 位
	DataL = MPU6050_ReadReg(GYRO_ZOUT_L);  // 读 Z 轴低 8 位
	*GyroZ = (DataH << 8) | DataL;
}
