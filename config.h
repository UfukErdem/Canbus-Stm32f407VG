#include "stm32f4xx.h"  

#ifndef config_h
#define config_h

/* Function Prototype */
void config_GPIOD(void);
void config_CANBUS(void);
void Can_SendBytes(uint16_t CanBus_Id, uint8_t Byte_0,uint8_t Byte_1,uint8_t Byte_2,uint8_t Byte_3,uint8_t Byte_4,uint8_t Byte_5,uint8_t Byte_6,uint8_t Byte_7);

#endif
