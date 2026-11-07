#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern char RxTextPacket[100];
extern uint8_t Serial_RxPacketFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint16_t *Array,uint8_t Length);
void Serial_SendString(char *String);
void Serial_SendNum(uint32_t Num,uint16_t Length);
void Serial_Printf(char *format, ...);

#endif
