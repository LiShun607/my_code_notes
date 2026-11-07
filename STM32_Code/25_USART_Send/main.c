#include "stm32f10x.h"                  // Device header
#include "Serial.h"

int main(void)
{
	Serial_Init();
	
	Serial_SendByte('A');
	
	uint16_t MyArray[] = {'A','B','C','D'};
	Serial_SendArray(MyArray,4);
	
	Serial_SendString("Hello DOUDOU!");
	
	Serial_SendNum(1562,4);
	
	printf("Num=%d\r\nName:DOUDOU\r\n",666);
	
	char String[100];
	sprintf(String,"Num=%d\r\n",666);
	Serial_SendString(String);
	
	Serial_Printf("Num=%d\r\n",12345);
	
	while(1)
	{
		
	}
}
