#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);
	//GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	uint16_t led_pins[]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4};
	
	for(int i=0;i<5;i++)//初始化所有灯为高电平（熄灭）
	{
		GPIO_WriteBit(GPIOA,led_pins[i],Bit_SET);
	}
	
	int Last=4;//记录上一次亮的灯的位置，注意这里的位置一定不能和下面第一个亮的灯的位置相同
	
	while(1)
	{
		for(int i=0;i<5;i++)
		{	
			GPIO_WriteBit(GPIOA,led_pins[i],Bit_RESET);
			GPIO_WriteBit(GPIOA,led_pins[Last],Bit_SET);
			Last=i;
			Delay_ms(80);
		}
		Last=0;
		for(int i=4;i>-1;i--)
		{	
			GPIO_WriteBit(GPIOA,led_pins[i],Bit_RESET);
			GPIO_WriteBit(GPIOA,led_pins[Last],Bit_SET);
			Last=i;
			Delay_ms(80);
		}
	}
}
	