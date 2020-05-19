#include "stm32f4xx.h"
#include <config.h>

CanTxMsg TxMessage;
CanRxMsg RxMessage;

char canbus_Received_Data[8]; 
uint16_t canbus_Received_ID;
uint32_t i;

void CAN1_RX0_IRQHandler(void)
{
		 
    CAN_Receive( CAN1, 0, &RxMessage);
  
	canbus_Received_Data[0] = RxMessage.Data[0];
	canbus_Received_Data[1] = RxMessage.Data[1];
	canbus_Received_Data[2] = RxMessage.Data[2];
	canbus_Received_Data[3] = RxMessage.Data[3];
	canbus_Received_Data[4] = RxMessage.Data[4];
	canbus_Received_Data[5] = RxMessage.Data[5];
	canbus_Received_Data[6] = RxMessage.Data[6];
	canbus_Received_Data[7] = RxMessage.Data[7]; 
	canbus_Received_ID = RxMessage.StdId;
	 
}

int main(void)
{
	config_GPIOD();
	config_CANBUS();
	
	
	while(1);
	/*
	You can communicate between two stm32f407Vg microprocessors using the CanSendBytes function. You can follow the data exchange from DEBUG.	
	*/
}





