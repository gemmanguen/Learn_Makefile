
#include "myUSART_f411xx.h"
/**********************************************************************/
 void USART_RCC_Control(USART_TypeDef *pUSARTx, uint8_t status){
	 if(status==ENABLE){
			if(pUSARTx==USART1){
				USART1_CLK_ENA();
			}else if(pUSARTx==USART2){
				USART2_CLK_ENA();
			}else if(pUSARTx==USART6){
				USART6_CLK_ENA();
			}		
	 }
	 else{
		 if(pUSARTx==USART1){
				USART1_CLK_DIS();
			}else if(pUSARTx==USART2){
				USART2_CLK_DIS();
			}else if(pUSARTx==USART6){
				USART6_CLK_DIS();
			}		
	 }
 }
 
 
 
 void USART_Init(USART_Handle_t *pUSARTHandle)
{

	//Temporary variable
	uint32_t temp=0;
	//enable the Clock for given USART peripheral
	USART_RCC_Control(pUSARTHandle->pUSARTx,ENABLE);
	
	/***************************/
	/*Configuration of CR1*/
	if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		//Enable the Receiver 
		temp|= USART_CR1_RE;
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field 
		temp|=USART_CR1_TE;
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields 
		temp|=USART_CR1_RE |USART_CR1_TE;
	}
  //Config the Word length 
	if(pUSARTHandle->USART_Config.USART_WordLength==USART_WORDLEN_9BITS)
	{
		//9 bits data
		temp|= USART_CR1_M ;
	}else
	{
		//8 bits data
		temp&=~USART_CR1_M;
	}
  //Config parity control bit fields
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//Enale the parity control 
		temp = USART_CR1_PCE;
		//Enable EVEN parity 
		temp &= ~USART_CR1_PS;

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//Enable the parity control 
	  temp |= USART_CR1_PCE;
	  //Enable ODD parity 
	  temp |= USART_CR1_PS;
	}
  //Program the CR1 register 
	pUSARTHandle->pUSARTx->CR1 = temp;

	/***************************/
	/* Configuration of CR2*/
	temp=0;
	//Implement the code to configure the number of stop bits inserted during USART frame transmission 
	temp |= pUSARTHandle->USART_Config.USART_NoOfStopBits << 12;
	//Program the CR2 register 
	pUSARTHandle->pUSARTx->CR2 = temp;

	/***************************/
	/* Configuration of CR3*/
	temp=0;
	//Configuration of USART hardware flow control 
	if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Enable CTS flow control 
		temp |= USART_CR3_CTSE;
	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		// Enable RTS flow control 
		temp |= USART_CR3_RTSE;

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Enable both CTS and RTS Flow control 
		temp|=USART_CR3_CTSE|USART_CR3_RTSE;;
	}
	//Program the CR3 register
	pUSARTHandle->pUSARTx->CR3 = temp;

	/***************************/
	/*Configuration of BRR(Baudrate register)*/
	//Baud rate=9600 with PCLK=16Mhz
	USART1->BRR=(0x68UL<<4)|0x3UL;
	
	
	/*Turn on UE bit in CR1 register in order to enable USART*/
	pUSARTHandle->pUSARTx->CR1|=(1<<13);
}

/*
 * USART_SendData
 */
 
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;
	uint32_t i;
 
	for( i = 0 ; i < Len; i++)
	{
		//Wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE));

    //WordLength=8 or 9 bits
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//if 9BIT, load the DR with 2bytes
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);
			
			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used, 9bits of user data will be sent		
				//Increase pTxBuffer twice 
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used, 8bits of user data will be sent
				//Increase pTxBuffer
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer 
			pUSARTHandle->pUSARTx->DR =(uint32_t) (*pTxBuffer  & (uint8_t)0xFF);
			
			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}


/*
 *  USART_ReceiveData
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint32_t i;
	for(i = 0 ; i < Len; i++){
		//Wait until RXNE flag is set in the SR
		while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));
		//Word length=8 or 9
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//check if using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used,read first 9 bits
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);
				//Increase the pRxBuffer twice
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used,read 8bits 
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
				 
				 //Increment the pRxBuffer
				pRxBuffer++;
			}
		}
		else
		{
			//check if using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , read 8 bits from DR
				 *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & 0xFF);
			}
			else
			{
				//Parity is used, so ,read only 7 bits 
				 *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & 0x7F);
			}
			//Increse the pRxBuffer
			pRxBuffer++;
		}
	}
}


/*
 *  USART_SendDataWithIT 
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TxState;

	if(txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->TxBuffer = pTxBuffer;
		pUSARTHandle->TxState = USART_BUSY_IN_TX;

		//Enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1|=USART_CR1_TXEIE;
		//Enable interrupt for TC 
		//pUSARTHandle->pUSARTx->CR1|=USART_CR1_TCIE;		
	}
	return txstate;
}


/*
 * USART_ReceiveDataIT
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->RxState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->RxBuffer = pRxBuffer;
		pUSARTHandle->RxState = USART_BUSY_IN_RX;
		//enable interrupt for RXNE
		pUSARTHandle->pUSARTx->CR1|=USART_CR1_RXNEIE;
	}
	return rxstate;

}

/*
 *Enable/Disable NVIC interrupt
 */
void USART_IRQInterruptConfig(uint8_t IRQ_Number, uint8_t status)
{
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

/*
 *Setting NVIC Interrupt priority
 */
void USART_IRQPriorityConfig(uint8_t IRQ_Number, uint32_t priority)
{
	NVIC->IP[IRQ_Number]|=(priority<<(8-4));
}

/*
 *Return flag status in SR register
 */
uint32_t USART_GetFlagStatus(USART_TypeDef *pUSARTx , uint32_t FlagName)
{
	return (uint32_t)((pUSARTx->SR)&(1<<FlagName));
}

/*
 * Handling when an interrupt occur
 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
	uint32_t temp1 , temp2;

/************************************************/
//*Check for interrupt from sending data complete*/

	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_FLAG_TC);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TCIE_Pos);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC
		if ( pUSARTHandle->TxState == USART_BUSY_IN_TX)
		{
			//Check the TxLen 
			if(! pUSARTHandle->TxLen )
			{
				//Disable TCIE
				pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_TC_Pos);		
				pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_TCIE_Pos);
				//Reset the application state
				pUSARTHandle->TxState = USART_READY;
				//Reset Buffer address to NULL
				pUSARTHandle->TxBuffer= NULL;				
				//Reset the length to zero
				pUSARTHandle->TxLen=0;

			}
		}
	}

/**************************************************/
	
/*Check for interrupt from sending data*/
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TXE_Pos);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE_Pos);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE
		if(pUSARTHandle->TxState == USART_BUSY_IN_TX)
		{
			if(pUSARTHandle->TxLen > 0)
			{
				//Word length = 8 or 9 bits
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//if 9BIT , load the DR with 2bytes 
					pUSARTHandle->pUSARTx->DR = (*(uint16_t*)(pUSARTHandle->TxBuffer) & (uint16_t)0x01FF);
					//check for USART_ParityControl
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used, 9bits of user data will be sent
						//Increase the Txbuffer twice
						pUSARTHandle->TxBuffer++;
						pUSARTHandle->TxBuffer++;
						pUSARTHandle->TxLen--;
					}
					else
					{
						//Parity bit is used , 8bits of user data will be sent
						//Increase the Txbuffer
						pUSARTHandle->TxBuffer++;
						pUSARTHandle->TxLen--;
					}
				}
				else
				{
					//8bit data transfer
					pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->TxBuffer  & (uint8_t)0xFF);
					//Increase the Txbuffer 
					pUSARTHandle->TxBuffer++;
					pUSARTHandle->TxLen--;
				}
			}
			if (!pUSARTHandle->TxLen )
			{
				//disable TXE
				pUSARTHandle->pUSARTx->CR1&= ~(1<<USART_CR1_TXEIE_Pos);
				pUSARTHandle->TxState = USART_READY;
				//Reset Buffer address to NULL
				pUSARTHandle->TxBuffer= NULL;
				//Reset the length to zero
				pUSARTHandle->TxLen=0;
			}
		}
	}
	
/******************************************/
	
/*Check for interrupt from receiving data*/
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RXNE_Pos);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE_Pos);

	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		if(pUSARTHandle->RxState == USART_BUSY_IN_RX)
		{
			if(pUSARTHandle->RxLen > 0)
			{
				//Word length = 8 or 9 bits
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//check if using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used, read first 9 bits from DR
						*((uint16_t*)pUSARTHandle->RxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);
						//Increase pRxBuffer two times
						pUSARTHandle->RxBuffer++;
						pUSARTHandle->RxBuffer++;
						pUSARTHandle->RxLen--;
					}
					else
					{
						//Parity is used, read 8 bits from DR
						 *(pUSARTHandle->RxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
						 
						 //Increase the pRxBuffer
						 pUSARTHandle->RxBuffer++;
						 pUSARTHandle->RxLen--;
					}
				}
				else
				{
					//check if using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used,read 8 bits from DR
						 *(pUSARTHandle->RxBuffer) = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
					}
					else
					{
						//Parity is used,read only 7 bits
						 *(pUSARTHandle->RxBuffer) = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);
					}
					//Increase the pRxBuffer
					pUSARTHandle->RxBuffer++;	
					pUSARTHandle->RxLen--;
				}							
			}
			if(! pUSARTHandle->RxLen)
			{
				//disable the RXNE
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE_Pos );
				pUSARTHandle->RxState = USART_READY;
			}
		}
	}
}


