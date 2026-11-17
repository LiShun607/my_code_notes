#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

// 注意：0xD0 是包含了最低位“写”标志位(0)的 8位地址。
// 如果是要读数据，需要将最低位置 1，即使用后面的 |0x01。
#define MPU6050_Address			0xD0

/*
 * 函数名：MPU6050_WriteReg
 * 功能：向 MPU6050 的指定寄存器写入一个字节的数据
 * 时序（写）：Start -> 器件地址(写) -> ACK -> 寄存器地址 -> ACK -> 写入数据 -> ACK -> Stop
 */
void MPU6050_WriteReg(uint8_t RegAddr,uint8_t SendByte)
{
	MyI2C_Start();                     // 1. 主机发出起始信号
	MyI2C_SendByte(MPU6050_Address);   // 2. 发送MPU6050地址 + 写方向(0)
	MyI2C_ReceiveACK();                //    接收从机应答（此时从机如果在线会拉低SDA）
	MyI2C_SendByte(RegAddr);           // 3. 发送要写入的寄存器地址
	MyI2C_ReceiveACK();                //    接收从机应答
	MyI2C_SendByte(SendByte);          // 4. 发送要写入寄存器的数据
	MyI2C_ReceiveACK();                //    接收从机应答
	MyI2C_Stop();                      // 5. 主机发出停止信号，释放总线
}

/*
 * 函数名：MPU6050_ReadReg
 * 功能：读取 MPU6050 指定寄存器的一个字节数据
 * 时序（读）：Start -> 器件地址(写) -> ACK -> 寄存器地址 -> ACK 
 *            -> 重复Start -> 器件地址(读) -> ACK -> 读数据 -> NACK -> Stop
 * 难点：读时序比写时序复杂，需要“写地址”来选中寄存器，再用“读地址”拿数据。
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	uint8_t ReceiveByte;
	
	MyI2C_Start();                     // 1. 起始信号
	MyI2C_SendByte(MPU6050_Address);   // 2. 发送器件地址 + 写方向(0)，为了告诉芯片我要找谁
	MyI2C_ReceiveACK();                //    接收应答
	MyI2C_SendByte(RegAddr);           // 3. 发送要读取的寄存器地址
	MyI2C_ReceiveACK();                //    接收应答（此时芯片内部已经定位到了这个寄存器）
	
	MyI2C_Start();                     // 4. 再次发出起始信号（注意：这里没有Stop，这叫“重复起始”）
	MyI2C_SendByte(MPU6050_Address|0x01); // 5. 发送器件地址 + 读方向(1)！因为最低位或了0x01，所以变成了 0xD1
	MyI2C_ReceiveACK();                //    接收应答
	ReceiveByte = MyI2C_ReceiveByte(); // 6. 主机开始接收从机发来的数据
	MyI2C_SendACK(1);                  // 7. 主机发送非应答(NACK)，告诉从机“我读完了，你可以释放SDA线了”
	MyI2C_Stop();                      // 8. 停止信号
	
	return ReceiveByte;
}

/*
 * 函数名：MPU6050_Init
 * 功能：MPU6050初始化配置
 * 逻辑：根据数据手册，按需配置几个核心寄存器，使其脱离睡眠并开始按我们要求的频率和量程工作。
 */
void MPU6050_Init(void)
{
	MyI2C_Init(); // 初始化MyI2C(底层引脚初始化)
	
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
