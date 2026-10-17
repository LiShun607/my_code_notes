#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int i = -1; //初始化i，使开始的第一步是0号引脚接的灯亮
int8_t Direction = 1; //灯的运行方向，1 代表正序，-1 代表逆序
int Last = 4; //记录上一次亮灯的引脚

void LED_Init(void)//初始化LED灯引脚，可修改GPIOx和GPIO_Pin_x
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
}

//调用一次进行一步的流水灯函数
void LED_Step(void)
{
	uint16_t Led_Pins[]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4};
	if(Direction == 1)
	{
		i++;
		//i加到超出引脚范围，翻转灯的运行方向
		if(i>=5)
		{
			Direction = -1;
			Last = 0;
		}
	}
	if(Direction == -1)
	{
		i--;
		//i减到超出引脚范围，翻转灯的运行方向
		if(i<0)
		{
			Direction = 1;
			Last = 4;
			i ++; //此目的是为了让i不超出引脚范围
		}
	}
	GPIO_ResetBits(GPIOA,Led_Pins[i]);
	GPIO_SetBits(GPIOA,Led_Pins[Last]);
	Last = i;
}

//置高电平，熄灭灯
void Led_Close(void)
{
	for(int i=0;i<5;i++)
	{
		uint16_t Led_Pins[]={GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4};
		GPIO_SetBits(GPIOA,Led_Pins[i]);
	}
}
