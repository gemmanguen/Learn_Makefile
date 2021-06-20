#ifndef __myUSART_f411_H
#define __myUSART_f411_H

#include "stm32f4xx.h"



#define NULL ((void*)0)


/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_TypeDef *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *TxBuffer;
	uint8_t *RxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxState;
	uint8_t RxState;
}USART_Handle_t;

/*
 *Disable/ enable clock for USART		
 */

typedef enum
{
	USART_FLAG_PE,	//Parity error flag
	USART_FLAG_FE,
	USART_FLAG_NF,
	USART_FLAG_ORE,
	USART_FLAG_IDLE,
	USART_FLAG_RXNE,
	USART_FLAG_TC,
	USART_FLAG_TXE,
	USART_FLAG_LBD,
	USART_FLAG_CTS
}USART_FLAGS;
/*
 *USART sending/receiving state
 */
typedef enum
{
	USART_READY,
	USART_BUSY_IN_TX,
	USART_BUSY_IN_RX
}USART_STATES;
/*
 *Disable/ enable clock for USART		
 */
#define	USART1_CLK_ENA()	(RCC->APB2ENR|=1<<4)
#define USART2_CLK_ENA()	(RCC->APB1ENR|=1<<17)
#define USART6_CLK_ENA()	(RCC->APB2ENR|=1<<5)

#define USART1_CLK_DIS()	(RCC->APB2ENR&=~(1<<4))
#define USART2_CLK_DIS()	(RCC->APB1ENR&=~(1<<17))
#define USART6_CLK_DIS()	(RCC->APB2ENR&=~(1<<5))

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX  2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3



/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void USART_RCC_Control(USART_TypeDef *pUSARTx, uint8_t status);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_TypeDef *pUSARTx);


/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t status);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

/*
 * Other Peripheral Control APIs
 */
//void USART_PeripheralControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi);
uint32_t USART_GetFlagStatus(USART_TypeDef *pUSARTx , uint32_t FlagName);
void USART_ClearFlag(USART_TypeDef *pUSARTx, uint16_t StatusFlagName);

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);

#endif


