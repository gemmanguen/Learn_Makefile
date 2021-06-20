#include"myTIM_f411xx.h"


static void TIM_PWM_CH1_Config(TIM_Handle_t* PWMHandle);
static void TIM_PWM_CH2_Config(TIM_Handle_t* PWMHandle);
static void TIM_PWM_CH3_Config(TIM_Handle_t* PWMHandle);
static void TIM_PWM_CH4_Config(TIM_Handle_t* PWMHandle);
/*
 *Enable/ Disable RCC clock for Timers
*/
void TIM_RCC_Control(TIM_TypeDef* pTIMx,uint8_t status)
{
	if(status==ENABLE)
	{
		if(pTIMx==TIM1)
			TIM1_CLK_ENA();
		else if(pTIMx==TIM2)
			TIM2_CLK_ENA();
		else if(pTIMx==TIM3)
			TIM3_CLK_ENA();
		else if(pTIMx==TIM4)
			TIM4_CLK_ENA();
		else if(pTIMx==TIM5)
			TIM5_CLK_ENA();
		else if(pTIMx==TIM9)
			TIM9_CLK_ENA();
		else if(pTIMx==TIM10)
			TIM10_CLK_ENA();
		else if(pTIMx==TIM11)
			TIM11_CLK_ENA();
	}else
	{
		if(pTIMx==TIM1)
			TIM1_CLK_DIS();
		else if(pTIMx==TIM2)
			TIM2_CLK_DIS();
		else if(pTIMx==TIM3)
			TIM3_CLK_DIS();
		else if(pTIMx==TIM4)
			TIM4_CLK_DIS();
		else if(pTIMx==TIM5)
			TIM5_CLK_DIS();
		else if(pTIMx==TIM9)
			TIM9_CLK_DIS();
		else if(pTIMx==TIM10)
			TIM10_CLK_DIS();
		else if(pTIMx==TIM11)
			TIM11_CLK_DIS();
	}
}

/*
 *Select clock source for Timers
 *Internal/External mode1/External mode2
*/
void TIM_ConfigClockSrc(TIM_Handle_t* TIMHandle,TIM_CLK_Config_t CLKConfig)
{
	//Clear all configurable bits
	TIMHandle->pTIMx->SMCR&=~(TIM_SMCR_ETP|TIM_SMCR_ECE|TIM_SMCR_ETPS|TIM_SMCR_ETF|TIM_SMCR_SMS|TIM_SMCR_TS);
	switch(CLKConfig.ClockSource)
	{
		case INTERNAL_CLK:
		{
			TIMHandle->pTIMx->SMCR&=~((0x7)<<TIM_SMCR_SMS_Pos);
			break;
		}
		case EXTERNAL_MODE1_ETR:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x7 to select ETR as trigger source
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE2:
		{
			//Enable External mode 2
			TIMHandle->pTIMx->SMCR|=(1<<TIM_SMCR_ECE_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		} 
		case EXTERNAL_MODE1_TI1ED:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x4 to select TI1_ED as trigger source
			TIMHandle->pTIMx->SMCR|=(0x4<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_TI1:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x5 to select TI1FP1 as trigger source
			TIMHandle->pTIMx->SMCR|=(0x5<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_TI2:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x6 to select TI2FP2 as trigger source
			TIMHandle->pTIMx->SMCR|=(0x6<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_ITR0:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x0 to select ITR0 as trigger source
			TIMHandle->pTIMx->SMCR&=~(0x7<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_ITR1:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x1 to select ITR1 as trigger source
			TIMHandle->pTIMx->SMCR|=(0x6<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_ITR2:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x2 to select ITR2 as trigger source
			TIMHandle->pTIMx->SMCR|=(0x6<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
		case EXTERNAL_MODE1_ITR3:
		{
			//Disable External mode 2
			TIMHandle->pTIMx->SMCR&=~(1<<TIM_SMCR_ECE_Pos);
			//SMS[2:0]=0x7 to select External mode 1
			TIMHandle->pTIMx->SMCR|=(0x7<<TIM_SMCR_SMS_Pos);
			//TS[6:4]=0x3 to select ITR3 as trigger source
			TIMHandle->pTIMx->SMCR|=(0x6<<TIM_SMCR_TS_Pos);
			//Config Clock polarity/Prescaler/Filter
			TIMHandle->pTIMx->SMCR|=(CLKConfig.ClockPolarity<<TIM_SMCR_ETP_Pos)|(CLKConfig.ClockPrescaler<<TIM_SMCR_ETPS_Pos)|(CLKConfig.ClockFilter<<TIM_SMCR_ETF_Pos);
			break;
		}
	} 
}


void TIM_Base_Init(TIM_Handle_t* TIMHandle)
{
	uint32_t temp=0;
	//Enable clock for TIMER
	TIM_RCC_Control(TIMHandle->pTIMx,ENABLE);
	//Select the counter mode:UP/DOWN/Center-aligned
	temp &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
  temp |= TIMHandle->TIMConfig.CounterMode;
	//Select clock division
	temp &=~(TIM_CR1_CKD);
	temp |= TIMHandle->TIMConfig.ClockDiv;
	//Set/clear auto reload preload register 
	temp&=~(TIM_CR1_ARPE);
	temp|= TIMHandle->TIMConfig.ARRPreload;
	TIMHandle->pTIMx->CR1=temp;
	//Config ARR register value
	TIMHandle->pTIMx->ARR=(uint32_t)TIMHandle->TIMConfig.Period;
	//Config PSC register value
	TIMHandle->pTIMx->PSC=(uint16_t)TIMHandle->TIMConfig.Prescaler;
	//Config RCR register value
	if(TIMHandle->pTIMx==TIM1)
	{
		TIMHandle->pTIMx->RCR=TIMHandle->TIMConfig.RepetitionCounter;
	}
	//Update event
	TIMHandle->pTIMx->EGR|=TIM_EGR_UG;
	while((TIM2->SR&(1<<0))==0); //wait until update event detected
	TIM2->SR&=~TIM_SR_UIF;
}
/*
 *Enable counter to start timer
 */
void TIM_Base_Start(TIM_Handle_t* TIMHandle)
{
	TIMHandle->pTIMx->CR1|=TIM_CR1_CEN;
}

void TIM_Base_Start_IT(TIM_Handle_t*TIMHandle)
{
	TIMHandle->pTIMx->DIER|=TIM_DIER_UIE;
	TIMHandle->pTIMx->CR1|=TIM_CR1_CEN;
}

/*
 *Disable counter to stop timer
 */
void TIM_Base_Stop(TIM_Handle_t* TIMHandle)
{
	TIMHandle->pTIMx->CR1&=~TIM_CR1_CEN;
}

/*
 *Select mode and channel for PWM
 */
void TIM_PWM_ChannelConfig(TIM_Handle_t*PWMHandle,uint32_t Channel)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1:
		{
			//Config channel 1 in PWM mode
			TIM_PWM_CH1_Config(PWMHandle);
			/* Set the Preload enable bit for channel1 */
      PWMHandle->pTIMx->CCMR1 |= TIM_CCMR1_OC1PE;
      /* Configure the Output Fast mode */
      PWMHandle->pTIMx->CCMR1 &= ~TIM_CCMR1_OC1FE;
      PWMHandle->pTIMx->CCMR1 |= PWMHandle->PWMConfig.OCFastMode;
      break;
		}
		case TIM_CHANNEL_2:
		{
			//Config channel 2 in PWM mode
			TIM_PWM_CH2_Config(PWMHandle);
			/* Set the Preload enable bit for channel1 */
      PWMHandle->pTIMx->CCMR1 |= TIM_CCMR1_OC2PE;
      /* Configure the Output Fast mode */
      PWMHandle->pTIMx->CCMR1 &= ~TIM_CCMR1_OC2FE;
      PWMHandle->pTIMx->CCMR1 |= PWMHandle->PWMConfig.OCFastMode;
			break;
		}
		case TIM_CHANNEL_3:
		{
			//Config channel 3 in PWM mode
			TIM_PWM_CH3_Config(PWMHandle);
			/* Set the Preload enable bit for channel1 */
      PWMHandle->pTIMx->CCMR2 |= TIM_CCMR2_OC3PE;
      /* Configure the Output Fast mode */
      PWMHandle->pTIMx->CCMR2 &= ~TIM_CCMR2_OC3FE;
      PWMHandle->pTIMx->CCMR2 |= PWMHandle->PWMConfig.OCFastMode;
			break;
		}
		case TIM_CHANNEL_4:
		{
			//Config channel 2 in PWM mode
			TIM_PWM_CH4_Config(PWMHandle);
			/* Set the Preload enable bit for channel1 */
      PWMHandle->pTIMx->CCMR2 |= TIM_CCMR2_OC4PE;
      /* Configure the Output Fast mode */
      PWMHandle->pTIMx->CCMR2 &= ~TIM_CCMR2_OC4FE;
      PWMHandle->pTIMx->CCMR2 |= PWMHandle->PWMConfig.OCFastMode;
			break;
		}
		default:
			break;
	}
}


static void TIM_PWM_CH1_Config(TIM_Handle_t* PWMHandle)
{
	uint32_t tmpccmrx;
	uint32_t tmpccer;
	/* Disable the Channel 1 temporarily: Reset the CC1E Bit */
	PWMHandle->pTIMx->CCER &= ~TIM_CCER_CC1E;
	/* Get the TIMx CCER register value */
	tmpccer = PWMHandle->pTIMx->CCER;
	/* Get the TIMx CCMR1 register value */
	tmpccmrx = PWMHandle->pTIMx->CCMR1;
	/* Reset the Output Compare Mode Bits */
	tmpccmrx &= ~TIM_CCMR1_OC1M;
	tmpccmrx &= ~TIM_CCMR1_CC1S;
	/* Select the Output Compare Mode */
	tmpccmrx |= PWMHandle->PWMConfig.OCMode;
	/* Reset the Output Polarity level */
	tmpccer &= ~TIM_CCER_CC1P;
	/* Set the Output Compare Polarity */
	tmpccer |= PWMHandle->PWMConfig.OCPolarity;
	/* Write to TIMx CCMR1 */
	PWMHandle->pTIMx->CCMR1 = tmpccmrx;
	/* Set the Capture Compare Register value */
	PWMHandle->pTIMx->CCR1 = PWMHandle->PWMConfig.Pulse;
	/* Write to TIMx CCER */
	PWMHandle->pTIMx->CCER = tmpccer;
}

static void TIM_PWM_CH2_Config(TIM_Handle_t* PWMHandle)
{
	uint32_t tmpccmrx;
	uint32_t tmpccer;
	/* Disable the Channel 2 temporarily: Reset the CC2E Bit */
	PWMHandle->pTIMx->CCER &= ~TIM_CCER_CC2E;
	/* Get the TIMx CCER register value */
	tmpccer = PWMHandle->pTIMx->CCER;
	/* Get the TIMx CCMR1 register value */
	tmpccmrx = PWMHandle->pTIMx->CCMR1;
	/* Reset the Output Compare Mode Bits */
	tmpccmrx &= ~TIM_CCMR1_OC2M;
	tmpccmrx &= ~TIM_CCMR1_CC2S;
	/* Select the Output Compare Mode */
	tmpccmrx |= (PWMHandle->PWMConfig.OCMode<<8U);
	/* Reset the Output Polarity level */
	tmpccer &= ~TIM_CCER_CC2P;
	/* Set the Output Compare Polarity */
	tmpccer |= (PWMHandle->PWMConfig.OCPolarity<<4U);
	/* Write to TIMx CCMR1 */
	PWMHandle->pTIMx->CCMR1 = tmpccmrx;
	/* Set the Capture Compare Register value */
	PWMHandle->pTIMx->CCR2 = PWMHandle->PWMConfig.Pulse;
	/* Write to TIMx CCER */
	PWMHandle->pTIMx->CCER = tmpccer;
}


static void TIM_PWM_CH3_Config(TIM_Handle_t* PWMHandle)
{
	uint32_t tmpccmrx;
	uint32_t tmpccer;
	/* Disable the Channel 3 temporarily: Reset the CC2E Bit */
	PWMHandle->pTIMx->CCER &= ~TIM_CCER_CC3E;
	/* Get the TIMx CCER register value */
	tmpccer = PWMHandle->pTIMx->CCER;
	/* Get the TIMx CCMR1 register value */
	tmpccmrx = PWMHandle->pTIMx->CCMR2;
	/* Reset the Output Compare Mode Bits */
	tmpccmrx &= ~TIM_CCMR2_OC3M;
	tmpccmrx &= ~TIM_CCMR2_CC3S;
	/* Select the Output Compare Mode */
	tmpccmrx |= PWMHandle->PWMConfig.OCMode;
	/* Reset the Output Polarity level */
	tmpccer &= ~TIM_CCER_CC3P;
	/* Set the Output Compare Polarity */
	tmpccer |= (PWMHandle->PWMConfig.OCPolarity<<8U);
	/* Write to TIMx CCMR2 */
	PWMHandle->pTIMx->CCMR2 = tmpccmrx;
	/* Set the Capture Compare Register value */
	PWMHandle->pTIMx->CCR3 = PWMHandle->PWMConfig.Pulse;
	/* Write to TIMx CCER */
	PWMHandle->pTIMx->CCER = tmpccer;
}

static void TIM_PWM_CH4_Config(TIM_Handle_t* PWMHandle)
{
	uint32_t tmpccmrx;
	uint32_t tmpccer;
	/* Disable the Channel 3 temporarily: Reset the CC2E Bit */
	PWMHandle->pTIMx->CCER &= ~TIM_CCER_CC4E;
	/* Get the TIMx CCER register value */
	tmpccer = PWMHandle->pTIMx->CCER;
	/* Get the TIMx CCMR1 register value */
	tmpccmrx = PWMHandle->pTIMx->CCMR2;
	/* Reset the Output Compare Mode Bits */
	tmpccmrx &= ~TIM_CCMR2_OC4M;
	tmpccmrx &= ~TIM_CCMR2_CC4S;
	/* Select the Output Compare Mode */
	tmpccmrx |= (PWMHandle->PWMConfig.OCMode<<8U);
	/* Reset the Output Polarity level */
	tmpccer &= ~TIM_CCER_CC4P;
	/* Set the Output Compare Polarity */
	tmpccer |= (PWMHandle->PWMConfig.OCPolarity<<12U);
	/* Write to TIMx CCMR2 */
	PWMHandle->pTIMx->CCMR2 = tmpccmrx;
	/* Set the Capture Compare Register value */
	PWMHandle->pTIMx->CCR4 = PWMHandle->PWMConfig.Pulse;
	/* Write to TIMx CCER */
	PWMHandle->pTIMx->CCER = tmpccer;
}

void TIM_PWM_Start(TIM_Handle_t*PWMHandle,uint32_t Channel)
{
	PWMHandle->pTIMx->CCER|=(1<<(Channel*4));
	TIM_Base_Start(PWMHandle);
}


