#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"

uint8_t ID;
int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	ID = MPU6050_ReadReg(WHO_AM_I); //读取MPU6050的地址
	
	//显示固定部分
	OLED_ShowString(1, 1, "ID:"); 
	OLED_ShowHexNum(1, 4, ID, 2);
	
	OLED_ShowString(2, 1, "A:");
	OLED_ShowString(2, 3, "x:");
	OLED_ShowString(3, 3, "y:");
	OLED_ShowString(4, 3, "z:");
	OLED_ShowString(2, 10, "G:");
	
	while(1)
	{
		//主循环里通过调用MPU6050_GetData函数不断刷新各个数据，并显示到屏幕上
		MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
		OLED_ShowSignedNum(2, 5, AccX, 4);
		OLED_ShowSignedNum(3, 5, AccY, 4);
		OLED_ShowSignedNum(4, 5, AccZ, 4);
		OLED_ShowSignedNum(2, 12, GyroX, 4);
		OLED_ShowSignedNum(3, 12, GyroY, 4);
		OLED_ShowSignedNum(4, 12, GyroZ, 4);
	}
}
