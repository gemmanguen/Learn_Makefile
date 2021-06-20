#ifndef __myGPIO_f411_H
#define __myGPIO_f411_H

#include "stm32f4xx.h"




/*Pin number of GPIO port*/
typedef enum{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15,
}GPIO_PIN;

/*---------------------------------*/
/*Modes of GPIO pins*/
typedef enum {
	GPIO_Mode_In,
	GPIO_Mode_Out,
	GPIO_Mode_ALT,
	GPIO_Mode_Analog,
	GPIO_Mode_IT_FT,	//External interrupt with falling edge
	GPIO_Mode_IT_RT,	//External interrupt with rising edge
	GPIO_Mode_IT_FRT	//External interrupt with rising and falling edge
}GPIO_MODE;


/*---------------------------------*/
/*GPIO output types*/
typedef enum{
	GPIO_OP_PP, // Push-pull output
	GPIO_OP_OD	// Open drain ouput
}GPIO_OPType;


/*---------------------------------*/
/*GPIO Speed*/
typedef enum{
	GPIO_SPD_Low,
	GPIO_SPD_Med,
	GPIO_SPD_Fast,
	GPIO_SPD_High
}GPIO_SPD;


/*---------------------------------*/
/*GPIO Pullup/Pulldown*/
typedef enum{
	GPIO_NONE_PUPD,
	GPIO_PU,
	GPIO_PD
}GPIO_PUPD;

/*---------------------------------*/
/*Clock enable GPIO peripherals */

#define GPIOA_CLK_ENA()		(RCC->AHB1ENR |= (1<<0))
#define GPIOB_CLK_ENA()		(RCC->AHB1ENR |= (1<<1))
#define GPIOC_CLK_ENA()		(RCC->AHB1ENR |= (1<<2))
#define GPIOD_CLK_ENA()		(RCC->AHB1ENR |= (1<<3))
#define GPIOE_CLK_ENA()		(RCC->AHB1ENR |= (1<<4))
#define GPIOH_CLK_ENA()		(RCC->AHB1ENR |= (1<<7))
/*---------------------------------*/

/*Clock disable GPIO peripherals */

#define GPIOA_CLK_DIS()		(RCC->AHB1ENR &=~(1<<0))
#define GPIOB_CLK_DIS()		(RCC->AHB1ENR &=~(1<<1))
#define GPIOC_CLK_DIS()		(RCC->AHB1ENR &=~(1<<2))
#define GPIOD_CLK_DIS()		(RCC->AHB1ENR &=~(1<<3))
#define GPIOE_CLK_DIS()		(RCC->AHB1ENR &=~(1<<4))
#define GPIOH_CLK_DIS()		(RCC->AHB1ENR &=~(1<<7))

/*************************************************************************/
/*Pins config struct */
typedef struct
{
	uint8_t	GPIO_PinNum;
	uint8_t	GPIO_PinMode;
	uint8_t GPIO_Speed;
	uint8_t	GPIO_Pupd;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltMode;
}GPIO_PinConfig_t;

/*Handle struct for GPIO pins*/
typedef struct
{
	GPIO_TypeDef *pGPIOx;						//This holds the address of GPIO Port
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;


/**************************************************************************/
/*API functions*/

//Init configurable items for GPIOx
void GPIO_Init(GPIO_Handle_t *GPIOHandle);

//Reset all setting of GPIOx to default
void GPIO_DeInit(GPIO_TypeDef *pGPIOx);

//Enable/Disable clock for GPIOx
void GPIO_RCC_Control(GPIO_TypeDef *pGPIOx, uint8_t status);

//Update status for a specific output pin 0/1
void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t PinNum,uint8_t value);

//Return status at a specifi input pin 0/1
uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t PinNum);

//Toggle status of a specific output pin 0<->1
void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNum);

//Config external interrupt if necessary
void GPIO_IRQStatusConfig(uint8_t IRQ_Number,uint8_t status );
void GPIO_IRQPriorityConfig(uint8_t IRQ_Number,uint8_t priority);
void GPIO_IRQHandling(uint8_t PinNum);

/*************************************************************************/
#endif





