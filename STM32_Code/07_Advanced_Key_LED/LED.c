#include "stm32f10x.h"                  // Device header
#include "Delay.h"

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

//流水灯
void LED_Flow(void)
{
	uint16_t Led_Pins[]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4};
	int Last = 4;
	for(int i=0;i<5;i++)
	{
		GPIO_ResetBits(GPIOA,Led_Pins[i]);
		GPIO_SetBits(GPIOA,Led_Pins[Last]);
		Last=i;
		Delay_ms(100);
	}
	Last=0;
	for(int i=4;i>-1;i--)
	{
		GPIO_ResetBits(GPIOA,Led_Pins[i]);
		GPIO_SetBits(GPIOA,Led_Pins[Last]);
		Last=i;
		Delay_ms(100);
	} 
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
