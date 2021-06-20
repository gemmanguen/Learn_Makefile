#include "myRCC_f411xx.h"


uint16_t AHBPrescaler[]={2,4,8,16,64,128,256,512};
uint8_t APB1Prescaler[]={2,4,8,16};
uint8_t APB2Prescaler[]={2,4,8,16};

/*
 * Config and select clock source for AHB, APB1, APB2 bus
 */
void SysClk_Config(Clk_Config_t ClkConfig)
{
	//Select HSE clk source
	if(ClkConfig.ClkSrc==RCC_HSE_SRC)
	{	
		RCC->CR|=(1<<RCC_CR_HSEON_Pos);
		while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos)));
		RCC->CFGR|=RCC_CFGR_SW_HSE;
		while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSE);
		
	}//Select PLL clock source
	else if(ClkConfig.ClkSrc==RCC_PLL_SRC)
	{
		if(ClkConfig.PLLPres.PLLSrc==RCC_HSE_SRC)
		{
			RCC->CR|=(1<<RCC_CR_HSEON_Pos);
			while(!(RCC->CR & (1<<RCC_CR_HSERDY_Pos)));
			RCC->CR|=RCC_CR_PLLON;
			while(!(RCC->CR & (1<<RCC_CR_PLLRDY_Pos)));
			RCC->CFGR|=RCC_CFGR_SW_PLL;
			while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
		}else if(ClkConfig.PLLPres.PLLSrc==RCC_HSI_SRC)
		{
			RCC->CR|=(1<<RCC_CR_HSION_Pos);
			while(!(RCC->CR & (1<<RCC_CR_HSIRDY_Pos)));
			RCC->CR|=RCC_CR_PLLON;
			while(!(RCC->CR & (1<<RCC_CR_PLLRDY_Pos)));
			RCC->CFGR|=RCC_CFGR_SW_PLL;
			while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
		}
		/*Config PLLM,PLLN,PLLP,PLLQ*/

			
	}
	else if(ClkConfig.ClkSrc==RCC_HSI_SRC)
	{
		RCC->CR|=(1<<RCC_CR_HSION_Pos);
		while(!(RCC->CR & (1<<RCC_CR_HSIRDY_Pos)));
		RCC->CFGR|=RCC_CFGR_SW_HSI;
		while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSI);
	}
	//Power enable clock and voltage regulator
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;
	PWR->CR|=PWR_CR_VOS;
	//Flash prefetch and latency related setting
	FLASH->ACR|=FLASH_ACR_ICEN|FLASH_ACR_DCEN|FLASH_ACR_PRFTEN|FLASH_ACR_LATENCY_5WS;
	//Prescaler
	switch(ClkConfig.AHBPres)
	{
		case 2:
			RCC->CFGR|=0x8<<4;
			break;
		case 4:
			RCC->CFGR|=0x9<<4;
			break;
		case 8:
			RCC->CFGR|=0xA<<4;
			break;
		case 16:
			RCC->CFGR|=0xB<<4;
			break;
		case 64:
			RCC->CFGR|=0xC<<4;
			break;
		case 128:
			RCC->CFGR|=0xD<<4;
			break;
		case 256:
			RCC->CFGR|=0xE<<4;
			break;
		case 512:
			RCC->CFGR|=0xF<<4;
			break;
		default:
			break;
	}
	
	switch(ClkConfig.APB1Pres)
	{
		case 2:
			RCC->CFGR|=0x4<<10;
			break;
		case 4:
			RCC->CFGR|=0x5<<10;
			break;
		case 8:
			RCC->CFGR|=0x6<<10;
			break;
		case 16:
			RCC->CFGR|=0x7<<10;
			break;
	}
	switch(ClkConfig.APB2Pres)
	{
		case 2:
			RCC->CFGR|=0x4<<13;
			break;
		case 4:
			RCC->CFGR|=0x5<<13;
			break;
		case 8:
			RCC->CFGR|=0x6<<13;
			break;
		case 16:
			RCC->CFGR|=0x7<<13;
			break;
	}
}
	
/*
 *Calculate the output clk from PLL if PLL is selected as a clk source
 *Input:		none
 *Return:		clk out of PLL
 */
static uint32_t RCC_GetPLLOutClk()
{
	uint32_t PLLoutput,PLLinput,PLLN,temp;
	uint8_t PLLM,PLLP;
	if((RCC->PLLCFGR&(1<<RCC_PLLCFGR_PLLSRC_Pos))==0)
	{
		PLLinput=HSICLK;
	}else if((RCC->PLLCFGR&(1<<RCC_PLLCFGR_PLLSRC_Pos)))
	{
		PLLinput=HSECLK;
	}
	PLLM= (RCC->PLLCFGR)&0x3F;
	PLLN= (RCC->PLLCFGR>>9)&0x1FF;
	
	temp=(RCC->PLLCFGR>>16)&0x3;
	if(temp==0)
	{
		PLLP=2;
	}else if(temp==1)
	{
		PLLP=4;
	}else if(temp==2)
	{
		PLLP=8;
	}else if(temp==3)
	{
		PLLP=16;
	}
	PLLoutput=(PLLinput*PLLN/PLLM)/PLLP;
	return PLLoutput;
}

/*
 *Calculate clk on APB1 bus
 *Input:		none
 *Return:		clk on APB1 bus
 */
uint32_t RCC_GetPCLK1Value()
{
	uint32_t pclk1,SysClk;
	uint8_t clksrc,temp,apbp;
	uint16_t ahbp;
	clksrc=(RCC->CFGR>>2)&0x3;
	
	if(clksrc==RCC_HSI_SRC)
	{
		SysClk=HSICLK;
	}else if(clksrc==RCC_HSE_SRC)
	{
		SysClk=HSECLK;
	}else if(clksrc==RCC_PLL_SRC)
	{
		SysClk=RCC_GetPLLOutClk();
	}
	//Calculate AHB prescaler
	temp=(RCC->CFGR>>4)&0xF;
	if(temp<8)
	{
		ahbp=1;
	}else
	{
		ahbp=AHBPrescaler[temp-8];
	}
	//Calculate APB1 prescaler
	temp=(RCC->CFGR>>10)&0x7;
	if(temp<4)
	{
		apbp=1;
	}else
	{
		apbp=APB1Prescaler[temp-4];
	}
	pclk1=(SysClk/ahbp)/apbp;
	return pclk1;
}

/*
 *Calculate clk on APB2 bus
 *Input:		none
 *Return:		clk on APB2 bus
 */
uint32_t RCC_GetPCLK2Value()
{
	uint32_t pclk2,SysClk;
	uint8_t clksrc,temp,apbp;
	uint16_t ahbp;
	clksrc=(RCC->CFGR>>2)&0x3;
	
	if(clksrc==RCC_HSI_SRC)
	{
		SysClk=HSICLK;
	}else if(clksrc==RCC_HSE_SRC)
	{
		SysClk=HSECLK;
	}else if(clksrc==RCC_PLL_SRC)
	{
		SysClk=RCC_GetPLLOutClk();
	}
	//Calculate AHB prescaler
	temp=(RCC->CFGR>>4)&0xF;
	if(temp<8)
	{
		ahbp=1;
	}else
	{
		ahbp=AHBPrescaler[temp-8];
	}
	//Calculate APB2 prescaler
	temp=(RCC->CFGR>>13)&0x7;
	if(temp<4)
	{
		apbp=1;
	}else
	{
		apbp=APB2Prescaler[temp-4];
	}
	pclk2=(SysClk/ahbp)/apbp;
	return pclk2;
}



