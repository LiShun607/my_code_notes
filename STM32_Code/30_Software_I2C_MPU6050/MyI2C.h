#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void MyI2C_W_SCL(uint8_t BitVal);
void MyI2C_W_SDA(uint8_t BitVal);
uint8_t MyI2C_R_SDA(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendACK(uint8_t ACK);
uint8_t MyI2C_ReceiveACK(void);


#endif
