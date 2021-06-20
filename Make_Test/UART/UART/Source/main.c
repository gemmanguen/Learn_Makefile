#include"stm32f4xx.h"
#include"myUSART_f411xx.h"
#include"myGPIO_f411xx.h"

#include"myRCC_f411xx.h"
#include"myTIM_f411xx.h"
void UART1_Init(void);
void UART1_send(uint8_t data);

USART_Handle_t myusart;
unsigned char rxbuffer[3]="HO";

Clk_Config_t myclk;
void SystemClk_Init()
{
	myclk.ClkSrc=RCC_HSE_SRC;
	myclk.AHBPres=1; 
	myclk.APB1Pres=1;
	myclk.APB2Pres=1;
	
	SysClk_Config(myclk);
}

void USART1_Init()
{
	myusart.pUSARTx=USART1;
	//myusart.USART_Config.USART_Baud=USART_STD_BAUD_9600;
	myusart.USART_Config.USART_HWFlowControl=USART_HW_FLOW_CTRL_NONE;
	myusart.USART_Config.USART_Mode=USART_MODE_TXRX;
	myusart.USART_Config.USART_NoOfStopBits=USART_STOPBITS_1;
	myusart.USART_Config.USART_ParityControl=USART_PARITY_DISABLE;
	myusart.USART_Config.USART_WordLength=USART_WORDLEN_8BITS;
	
	USART_Init(&myusart);
}

void GPIO_USART1_Init()
{
	GPIO_Handle_t mygpio;
	mygpio.pGPIOx=GPIOA;
	mygpio.GPIO_PinConfig.GPIO_PinMode=GPIO_Mode_ALT;
	mygpio.GPIO_PinConfig.GPIO_PinAltMode=7;
	mygpio.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_PP;
	mygpio.GPIO_PinConfig.GPIO_Pupd=GPIO_NONE_PUPD;
	mygpio.GPIO_PinConfig.GPIO_Speed=GPIO_SPD_Fast; 
	
	mygpio.GPIO_PinConfig.GPIO_PinNum=9;
	GPIO_Init(&mygpio);
	
	mygpio.GPIO_PinConfig.GPIO_PinNum=10;
	GPIO_Init(&mygpio);
}

void GPIOC_Init()
{
	GPIO_Handle_t mygpio;
	mygpio.pGPIOx=GPIOC;
	mygpio.GPIO_PinConfig.GPIO_PinMode=GPIO_Mode_In;
	mygpio.GPIO_PinConfig.GPIO_PinAltMode=7;
	mygpio.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_PP;
	mygpio.GPIO_PinConfig.GPIO_Pupd=GPIO_NONE_PUPD;
	mygpio.GPIO_PinConfig.GPIO_Speed=GPIO_SPD_Fast; 
	mygpio.GPIO_PinConfig.GPIO_PinNum=13;
	GPIO_Init(&mygpio);
}
void GPIOA_Init()
{
	GPIO_Handle_t mygpio;
	mygpio.pGPIOx=GPIOA;
	mygpio.GPIO_PinConfig.GPIO_PinMode=GPIO_Mode_ALT;
	mygpio.GPIO_PinConfig.GPIO_PinAltMode=1;
	mygpio.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_PP;
	mygpio.GPIO_PinConfig.GPIO_Pupd=GPIO_NONE_PUPD;
	mygpio.GPIO_PinConfig.GPIO_Speed=GPIO_SPD_Low; 
	mygpio.GPIO_PinConfig.GPIO_PinNum=0;
	GPIO_Init(&mygpio);
}
TIM_Handle_t mytim;
void TIM2_Init(void)
{
	TIM_CLK_Config_t tCLK;
	mytim.pTIMx=TIM2;
	mytim.TIMConfig.ARRPreload=APRE_DISABLE;
	mytim.TIMConfig.ClockDiv=CLK_DIV1;
	mytim.TIMConfig.CounterMode=CounterUp;
	mytim.TIMConfig.Period=9999;
	mytim.TIMConfig.Prescaler=7;
	TIM_Base_Init(&mytim);
	
	tCLK.ClockSource=INTERNAL_CLK;
	TIM_ConfigClockSrc(&mytim,tCLK);
	
	mytim.PWMConfig.OCMode=TIM_OCMODE_PWM1;
	mytim.PWMConfig.OCPolarity=TIM_OCPOLARITY_HIGH;
	mytim.PWMConfig.Pulse=4999;
	
	TIM_PWM_ChannelConfig(&mytim,TIM_CHANNEL_1);
}

void delay_us(uint32_t us)
{
	TIM2->CNT=0;
	while(TIM2->CNT < us);
}

void delay_ms(uint32_t ms)
{
	uint16_t j;
	for(j=0;j<ms;j++)
	{
		delay_us(1000);
	}
}

int i=0;
uint32_t value;
int main(){
	SystemClk_Init();
//	USART1_Init();
//	GPIO_USART1_Init();
	TIM2_Init();
	TIM_PWM_Start(&mytim,TIM_CHANNEL_1);
	GPIOC_Init();
	GPIOA_Init();
	
	value=RCC_GetPCLK1Value();
	
	//NVIC_EnableIRQ(TIM2_IRQn);
	while(1)
	{	
		if(GPIO_ReadPin(GPIOC,13)==RESET)
		{
			for(i=0;i<100000;i++){}
			while(USART_ReceiveDataIT(&myusart,rxbuffer,2)!= USART_READY);
			USART_SendData(&myusart,(uint8_t*)"Hello",5);
		}
		//GPIO_TogglePin(GPIOA,5);
		//delay_ms(1000);
	}
}

void USART1_IRQHandler()
{
	USART_IRQHandling(&myusart);
}

void TIM2_IRQHandler()
{
	GPIO_TogglePin(GPIOA,5);
	TIM2->SR&=~TIM_SR_UIF;
}


