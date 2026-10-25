#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"


int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:000%");
	
	PWM_SetPrescaler1(720 - 1); //改变PSC
	PWM_SetCompare1(99); //改变CCR
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq1(), 5); //在屏幕上不断刷新显示Freq
		OLED_ShowNum(2, 6, IC_GetCompare2(), 3); //在屏幕上不断刷新显示Duty
		OLED_ShowNum(3, 1, TIM_GetCapture1(TIM3), 3); //在屏幕上不断刷新显示捕获1的值
		OLED_ShowNum(4, 1, TIM_GetCapture2(TIM3), 3); //在屏幕上不断刷新显示捕获2的值
	}
}
