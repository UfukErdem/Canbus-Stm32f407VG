#include <config.h>
#include "stm32f4xx.h"  


void config_GPIOD(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void config_CANBUS(void)
{
	 // Enable GPIO and CAN1 clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	 
	 // We configured to GPIO for Can Rx pin and Tx pin	 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	 
	 // GPIO and CAN pins bridge each other. 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1); 
	 
	 // We created structure for configure to Can and Can Filter Register
	CAN_InitTypeDef CAN_InitStructure;	
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
 	 // Reset CAN1 clock
	CAN_DeInit(CAN1);
	
	 // configuration for Can1
	/* Can Line use APB1 clock line so we have 42Mhz oscillation clock
    *	Bit rate = Sysclock / (Prescaler * (1 + Tseg1 + Tseg2))
    *	42 / ( 2 * ( 1 + 12 + 8) ) = 1 Mb
    */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = 2; 
    
	CAN_StructInit(&CAN_InitStructure);
    CAN_Init(CAN1, &CAN_InitStructure);

	// configuration for Can1 mask and filter
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    /* The filter and mask are configured to receive messages with ID between 0x00 and 0x003. */
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x000 << 5; 
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
 	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x1FC << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

	// We enabled FIFO flag(We can say Rx interrupt) for interrupt.
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
 	
 	// We configured NVIC register for using canbus Rx0 interrupt.
  	NVIC_InitTypeDef  NVIC_InitStructure;
   	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   	NVIC_Init(&NVIC_InitStructure);
}

void Can_SendBytes(uint16_t CanBus_Id, uint8_t Byte_0,uint8_t Byte_1,uint8_t Byte_2,uint8_t Byte_3,uint8_t Byte_4,uint8_t Byte_5,uint8_t Byte_6,uint8_t Byte_7)
{
	/* This function use for transmitting message*/
	
	/* We gave a Message ID to packet*/
	TxMessage.StdId = CanBus_Id;
	/* Set as data frame */
	TxMessage.RTR 	= CAN_RTR_Data;
	/* Set as Standart Frame (CAN2.0A)*/
	TxMessage.IDE 	= CAN_Id_Standard;
	/* The number of bytes to be sent has been determined */
	TxMessage.DLC 	= 8;
	/* Data to be sent has been determined*/
	TxMessage.Data[0] = Byte_0;    
	TxMessage.Data[1] = Byte_1;    
	TxMessage.Data[2] = Byte_2;    
	TxMessage.Data[3] = Byte_3;    
	TxMessage.Data[4] = Byte_4;    
	TxMessage.Data[5] = Byte_5;    
	TxMessage.Data[6] = Byte_6;    
	TxMessage.Data[7] = Byte_7;    
	
	CAN_Transmit(CAN1,&TxMessage);
}

