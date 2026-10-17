#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

void CountSensor_Init(void);
uint16_t CountSensor_GetCount(void);

extern uint8_t CountSensor_Flag;
extern uint16_t CountSensor_Count;

#endif
