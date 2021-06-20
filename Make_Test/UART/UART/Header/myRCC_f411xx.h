#ifndef __myRCC_f411_H
#define __myRCC_f411_H

#include "stm32f4xx.h"

#define HSICLK 16000000
#define HSECLK 8000000

/*Define clock source of system clock*/
typedef enum
{
	RCC_HSI_SRC,
	RCC_HSE_SRC,
	RCC_PLL_SRC
}System_CLK;

typedef struct
{
	uint32_t PLLSrc;
	uint32_t PLLN;
	uint8_t PLLM;
	uint8_t PLLP;
	uint8_t PLLQ;
}PLL_Config_t;

typedef struct
{
	uint8_t ClkSrc;
	uint16_t AHBPres;
	uint8_t APB1Pres;
	uint8_t APB2Pres;
	PLL_Config_t PLLPres;
}Clk_Config_t;


void SysClk_Config(Clk_Config_t ClkConfig);
uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);
#endif


