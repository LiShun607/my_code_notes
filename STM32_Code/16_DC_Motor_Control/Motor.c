#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//初始化直流电机
//对于直流电机，除了需要配置PWM外，还需要配置两个GPIO口，让两个GPIO口电平一高一低，从而实现正转和反转
void Motor_Init(void)
{
	PWM_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP; //配置为推挽输出
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

//该函数将Speed传给PWM_SetCompare3函数，改变占空比，从而改变速度
//并且通过if判断Speed的正负，来翻转两个GPIO的电平，从而实现正反转
void Motor_Speed(int16_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		PWM_SetCompare3(Speed);
	}
	if(Speed < 0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(-Speed);
	}
}
