#include "myGPIO_f411xx.h"


/*
*Enable/ Disable clock for GPIO
*/
void GPIO_RCC_Control(GPIO_TypeDef *pGPIOx, uint8_t status){
	if(status==ENABLE){
		if(pGPIOx==GPIOA)
			GPIOA_CLK_ENA();
		else if(pGPIOx==GPIOB)
			GPIOB_CLK_ENA();
		else if(pGPIOx==GPIOC)
			GPIOC_CLK_ENA();
		else if(pGPIOx==GPIOD)
			GPIOD_CLK_ENA();
		else if(pGPIOx==GPIOE)
			GPIOE_CLK_ENA();
		else if(pGPIOx==GPIOH)
			GPIOH_CLK_ENA();
	}
	else{
		if(pGPIOx==GPIOA)
			GPIOA_CLK_DIS();
		else if(pGPIOx==GPIOB)
			GPIOB_CLK_DIS();
		else if(pGPIOx==GPIOC)
			GPIOC_CLK_DIS();
		else if(pGPIOx==GPIOD)
			GPIOD_CLK_DIS();
		else if(pGPIOx==GPIOE)
			GPIOE_CLK_DIS();
		else if(pGPIOx==GPIOH)
			GPIOH_CLK_DIS();
	}
}

void GPIO_Init(GPIO_Handle_t *GPIOHandle){
	uint32_t tmp=0;
	//ENABLE Clock for GPIO
	GPIO_RCC_Control(GPIOHandle->pGPIOx, ENABLE);
	//Config pin mode
	if(GPIOHandle->GPIO_PinConfig.GPIO_PinMode<=GPIO_Mode_Analog){
		tmp=(GPIOHandle->GPIO_PinConfig.GPIO_PinMode<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
		GPIOHandle->pGPIOx->MODER&=~(0x3<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum)); //Clearing bits at correspoding Pin position
		GPIOHandle->pGPIOx->MODER|=tmp; //Setting bit
	}
	else{
		//Config interrupt mode: Rising edge/ Falling edge/ Both
		if(GPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_Mode_IT_FT){
			EXTI->FTSR|=(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->RTSR&=~(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
		}else if(GPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_Mode_IT_RT){
			EXTI->RTSR|=(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->FTSR&=~(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
		}else if(GPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_Mode_IT_FRT){
			EXTI->FTSR|=(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->RTSR|=(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
		}
		//Clock enable for SYSCFG
		RCC->APB2ENR |= 1<<14;	
		
		//Enable maskable exti interrupt line [Pin0-Pin15] 
		EXTI->IMR|=(1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
		
		//Select source that trigger an interrupt[PortA-PortH]
		if(GPIOHandle->pGPIOx==GPIOA){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]&=~(0xF<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}else if(GPIOHandle->pGPIOx==GPIOB){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]|=(0x1<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}else if(GPIOHandle->pGPIOx==GPIOC){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]|=(0x2<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}else if(GPIOHandle->pGPIOx==GPIOD){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]|=(0x3<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}else if(GPIOHandle->pGPIOx==GPIOE){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]|=(0x4<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}else if(GPIOHandle->pGPIOx==GPIOB){
			SYSCFG->EXTICR[GPIOHandle->GPIO_PinConfig.GPIO_PinNum/4]|=(0x7<<(4*(GPIOHandle->GPIO_PinConfig.GPIO_PinNum%4)));
		}
		
		//
	}
	tmp=0;
	
	//Config speed
	tmp=(GPIOHandle->GPIO_PinConfig.GPIO_Speed<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	GPIOHandle->pGPIOx->OSPEEDR&=~(0x3<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	GPIOHandle->pGPIOx->OSPEEDR|=tmp;
	tmp=0;
	
	//Config Pullup/Pulldown pin
	tmp=(GPIOHandle->GPIO_PinConfig.GPIO_Pupd<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	GPIOHandle->pGPIOx->PUPDR&=~(0x3<<(2*GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	GPIOHandle->pGPIOx->PUPDR|=tmp;
	tmp=0;
	
	//Config Output type
	tmp=(GPIOHandle->GPIO_PinConfig.GPIO_PinOPType<<(GPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	GPIOHandle->pGPIOx->OTYPER&=~(0x1<<GPIOHandle->GPIO_PinConfig.GPIO_PinNum);
	GPIOHandle->pGPIOx->OTYPER|=tmp;
	tmp=0;
	
	//Config Alternate function
	if(GPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_Mode_ALT){
		uint8_t tmp1, tmp2;
		tmp1 =GPIOHandle->GPIO_PinConfig.GPIO_PinNum/8;
		tmp2 =GPIOHandle->GPIO_PinConfig.GPIO_PinNum %8;
		GPIOHandle->pGPIOx->AFR[tmp1]&=~(0xF<<(4*tmp2));
		GPIOHandle->pGPIOx->AFR[tmp1]|=(GPIOHandle->GPIO_PinConfig.GPIO_PinAltMode<<(4*tmp2));
	}
}

void GPIO_DeInit(GPIO_TypeDef *pGPIOx){
	if(pGPIOx==GPIOA){
		RCC->AHB1RSTR|=1<<0;
		RCC->AHB1RSTR&=1<<0;
	}
	else if(pGPIOx==GPIOB){
		RCC->AHB1RSTR|=1<<1;
		RCC->AHB1RSTR&=1<<1;
	}
	else if(pGPIOx==GPIOC){
		RCC->AHB1RSTR|=1<<2;
		RCC->AHB1RSTR&=1<<2;
	}
	else if(pGPIOx==GPIOD){
		RCC->AHB1RSTR|=1<<3;
		RCC->AHB1RSTR&=1<<3;
	}
	else if(pGPIOx==GPIOE){
		RCC->AHB1RSTR|=1<<4;
		RCC->AHB1RSTR&=1<<4;
	}
	else if(pGPIOx==GPIOH){
		RCC->AHB1RSTR|=1<<7;
		RCC->AHB1RSTR&=1<<7;
	}
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t PinNum){
	uint8_t value;
	value =(uint8_t)(((pGPIOx->IDR)>>PinNum)& 1UL);
	return value;
}

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t PinNum,uint8_t value){
	if(value==SET){
		pGPIOx->ODR|=(1<<PinNum);
	}
	else{
		pGPIOx->ODR&=~(1<<PinNum);
	}
}

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNum){
	pGPIOx->ODR^=(1<<PinNum);
}

	
void GPIO_IRQStatusConfig(uint8_t IRQ_Number, uint8_t status ){
	if(status==ENABLE){
		if(IRQ_Number<=31){
			NVIC->ISER[0]|=1<<IRQ_Number;
		}else if(IRQ_Number>31&&IRQ_Number<=63){
			NVIC->ISER[1]|=1<<(IRQ_Number%32);
		}else if(IRQ_Number>63&&IRQ_Number<=95){
			NVIC->ISER[2]|=1<<(IRQ_Number%64);
		}
	}else{
		if(IRQ_Number<=31){
			NVIC->ICER[0]|=1<<IRQ_Number;
		}else if(IRQ_Number>31&&IRQ_Number<=63){
			NVIC->ICER[1]|=1<<(IRQ_Number%32);
		}else if(IRQ_Number>63&&IRQ_Number<=95){
			NVIC->ICER[2]|=1<<(IRQ_Number%64);
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQ_Number,uint8_t priority){
	//uint8_t ip_section=IRQ_Number%4;
	// Number of implemented priority bits in IP Register of Cortex M4 is 4
		NVIC->IP[IRQ_Number]|=(priority<<(8-4));
}

void GPIO_IRQHandling(uint8_t PinNum)
{
	if(EXTI->PR&(1<<PinNum))
	{
		//Clear pending bit in PR register
		EXTI->PR|=(1<<PinNum);
	}
}








