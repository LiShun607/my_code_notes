#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);
	//GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while(1)
	{
		for(int i=0;i<5;i++)
		{
			GPIO_WriteBit(GPIOA,(1<<i),Bit_RESET);
			Delay_ms(100);
			GPIO_WriteBit(GPIOA,(1<<i),Bit_SET);
			Delay_ms(100);
		}
	}
}
