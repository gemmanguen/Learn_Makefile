#ifndef __myTIM_f411_H
#define __myTIM_f411_H
#include "stm32f4xx.h"

/*Disable/ enable clock for Timer		
*/
#define	TIM1_CLK_ENA()	(RCC->APB2ENR|=1<<0)
#define TIM2_CLK_ENA()	(RCC->APB1ENR|=1<<0)
#define TIM3_CLK_ENA()	(RCC->APB1ENR|=1<<1)
#define TIM4_CLK_ENA()	(RCC->APB1ENR|=1<<2)
#define TIM5_CLK_ENA()	(RCC->APB1ENR|=1<<3)
#define TIM9_CLK_ENA()	(RCC->APB2ENR|=1<<16)
#define TIM10_CLK_ENA()	(RCC->APB2ENR|=1<<17)
#define TIM11_CLK_ENA()	(RCC->APB2ENR|=1<<18)

#define TIM1_CLK_DIS()	(RCC->APB2ENR&=~(1<<0))
#define TIM2_CLK_DIS()	(RCC->APB1ENR&=~(1<<0))
#define TIM3_CLK_DIS()	(RCC->APB1ENR&=~(1<<2))
#define TIM4_CLK_DIS()	(RCC->APB1ENR&=~(1<<3))
#define TIM5_CLK_DIS()	(RCC->APB1ENR&=~(1<<4))
#define TIM9_CLK_DIS()	(RCC->APB2ENR&=~(1<<16))
#define TIM10_CLK_DIS()	(RCC->APB2ENR&=~(1<<17))
#define TIM11_CLK_DIS()	(RCC->APB2ENR&=~(1<<18))

/*Define external trigger polarity*/
#define ETP_NonInverted	0
#define ETP_Inverted		1

/*Define external trigger prescaler*/
#define ETPS_None 0x0
#define	ETPS_DIV2	0x1
#define	ETPS_DIV4 0x2
#define ETPS_DIV8 0x3


typedef enum
{
	INTERNAL_CLK,
	EXTERNAL_MODE1_ITR0,
	EXTERNAL_MODE1_ITR1,
	EXTERNAL_MODE1_ITR2,
	EXTERNAL_MODE1_ITR3,
	EXTERNAL_MODE1_TI1ED,
	EXTERNAL_MODE1_TI1,
	EXTERNAL_MODE1_TI2,			
	EXTERNAL_MODE1_ETR,
	EXTERNAL_MODE2,
}TIM_CLK_SRC;

/*Counter mode*/
#define	CounterUp 													0x00000000UL
#define	CounterDown 												1<<4UL
#define	CounterUpdown1 											1<<5UL
#define	CounterUpdown2 											2<<5UL
#define	CounterUpdown3 											3<<5UL

/*ARR preload enable/disable*/
#define APRE_ENABLE 												1<<7UL
#define	APRE_DISABLE 												0x00000000UL

/*Clock division ratio*/
#define CLK_DIV1 														0x00000000UL
#define CLK_DIV2 														1<<8UL
#define CLK_DIV4 														2<<8UL

/*Output compare mode*/
#define TIM_OCMODE_TIMING                   0x00000000U                                                                             
#define TIM_OCMODE_ACTIVE                   1<<4UL                                         
#define TIM_OCMODE_INACTIVE                 2<<4UL                                         
#define TIM_OCMODE_TOGGLE                   3<<4UL                    
#define TIM_OCMODE_PWM1                     6<<4UL                    
#define TIM_OCMODE_PWM2                     7<<4UL
#define TIM_OCMODE_FORCED_ACTIVE            5<<4UL              
#define TIM_OCMODE_FORCED_INACTIVE          4<<4UL                            

/*Output compare polarity*/
#define TIM_OCPOLARITY_HIGH 								0x00000000U
#define TIM_OCPOLARITY_LOW									1<<1UL

/*PWM Channel*/
typedef enum
{
	TIM_CHANNEL_1,
	TIM_CHANNEL_2,
	TIM_CHANNEL_3,
	TIM_CHANNEL_4
}TIM_PWM_CHANNEL;


typedef struct
{
	uint32_t ClockSource;     
  uint32_t ClockPolarity;   
  uint32_t ClockPrescaler;  
  uint32_t ClockFilter;     
}TIM_CLK_Config_t;


typedef struct
{
	uint32_t Prescaler;					//ftimer=fclk/(PSC+1)
	uint32_t CounterMode;				//select among TIM_CounterMode
	uint32_t Period;						//ARR register
	uint8_t	ClockDiv;
	uint32_t RepetitionCounter; //only for TIM1
	uint32_t ARRPreload;				/*Preload enable/disable 
															if enable: value of ARR register is loaded after an Update event
															if disable: value of ARR register is loaded immediately
															*/
}TIM_Config_t;

typedef struct
{
	uint32_t OCMode;						// Config in TIMx_CCMR1 register
	uint32_t Pulse;							//Compare value
	uint32_t OCPolarity;
	uint32_t OCFastMode;
	
}TIM_OCConfig_t;

typedef struct
{
	TIM_TypeDef* pTIMx;
	TIM_Config_t TIMConfig;
	TIM_OCConfig_t PWMConfig;
}TIM_Handle_t;



/*Peripherals clock setup*/
void TIM_RCC_Control(TIM_TypeDef* pTIMx,uint8_t status);
void TIM_ConfigClockSrc(TIM_Handle_t* TIMHandle,TIM_CLK_Config_t CLKConfig);
/*Initialize/start/stop timer */
void TIM_Base_Init(TIM_Handle_t* TIMHandle);
void TIM_Base_Start(TIM_Handle_t* TIMHandle);
void TIM_Base_Stop(TIM_Handle_t* TIMHandle);
void TIM_Base_Start_IT(TIM_Handle_t*TIMHandle);
/*Initialize PWM*/
void TIM_PWM_ChannelConfig(TIM_Handle_t*PWMHandle,uint32_t Channel);
void TIM_PWM_Start(TIM_Handle_t*PWMHandle,uint32_t Channel);

#endif



