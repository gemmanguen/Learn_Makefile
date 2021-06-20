//#include <stdio.h>
#include "GPIO.h"

void Clock_Init(void)
{
//	*((volatile uint32_t*)RCC_BASE) |= (1<<0);		//HSION
	RCC->CR |= (1 << 0);
}

void myDelay(uint32_t time)
{
	uint32_t i;
	while(time--)
	{
		for(i = 0; i < 1000; i++);
	}
}

void GPIO_Init(void)
{
	/* PC13 as INPUT pullup mode */
//	*((volatile uint32_t*)(RCC_BASE + 0x30)) |= (1 << 2);
	RCC->AHB1 |= (1 << 2);
	PC->PU |= (1 << 26);
		
	/* PA5 as OUTPUT Push-Pull mode */
	RCC->AHB1 |= (1 << 0);
	PA->MODE |= (1 << 10); 
	PA->TYPE &= ~(1 << LED_GREEN);
	PA->SPEED |= (1 << 10);
} 

/* This function is use to check endianness of my machine and print it
     No Parameter
     Return: 1 = Little endian
				 0 = Big Endian
 */
int Check_Endian(void)
{
    unsigned int i = 0x00000001;
    char* c = (char*)&i;
    if (*c)
	{
		//printf("Little endian\n");
		PA->OUT &= ~(1 << LED_GREEN);	
		return 1;
	}
    else
	{
		//printf("Big endian\n");
		PA->OUT |= (1 << LED_GREEN);	
		return 0;
	}
}

