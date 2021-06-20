//#include <stdio.h>
#include "GPIO.h"
#include "result.h"

void gpio_TC_001(void)
{	
	Clock_Init();
	GPIO_Init();
	
	/* LED ON = set bit 5th of PA OUT register */
	PA->OUT ^= (1 << LED_GREEN);
	
	int *Aout_Address = (int*)0x40020014;
	/* address of PA OUT = 0x4002.0014 */
	CHECK_RESULT(*Aout_Address, 0x00000020);
}
