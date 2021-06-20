
#include "flash.h"

void Flash_Init(void)
{
	RCC->CR |= (1 << 0);
}

void Flash_Unlock(void)
{
	/* This sequence consists of 2 write cycle, 
	 * where 2 key values (KEY1 & KEY2) are written to the FLASH_KEYR address
	 */	 
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void Flash_Lock(void)
{
	/* Set bit 32-th (LOCK bit) to lock flash */
	FLASH->CR |= FLASH_CR_LOCK;
}

void Flash_Erase(uint32_t sector)
{
	/* Check that no Flash memmory operation is ongoing by checking BSY bit in FLASH_SR Register */
	while ((FLASH->SR & FLASH_SR_BSY))
	{
		/* Waiting for BSY bit */
	}
	
	/* Sector erase active by setting SER bit in FLASH_CR register 
	 * & Select Sector you wish to erase by SNB bits in FLASH_CR register
	 */
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= (sector << 3);
	
	/* Trigger erase operator by Setting the STRT bit in the FLASH_CR register */
	FLASH->CR |= FLASH_CR_STRT;
	
	/* Wait for the BSY bit to be cleared */
	while ((FLASH->SR & FLASH_SR_BSY));
}	

/**
  * @brief  Write data to Flash
  * @param  Start Address, data buffer, and data length to write
  * @retval ERROR or NO ERROR
  */
FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t* u8BufferWrite, uint32_t u32Length)
{
	uint32_t u32count = 0;
	
	/* Check inpit param */
	if ((u8BufferWrite == 0x00U) || (u32Length == 0U) || (u32Length % 4U != 0U))
	{
		return FLASH_ERRORS;
	}
	
	/*Check that no main Flash memory operation is ongoing
	 * by checking the BSY bit in the FLASH_SR register
	 */
	while ((FLASH->SR & FLASH_SR_BSY));
	
	Flash_Unlock();
	/* Active Flash Programming 
	 * by Setting the PG bit in the FLASH_CR register */
	FLASH->CR |= FLASH_CR_PG;
	
	/* Perform the data write operation(s) to the desired memory address */
	for (u32count = 0U; u32count < (u32Length/4); u32count++)
	{
		*(uint32_t*)(u32StartAddr + u32count*4U) = *(uint32_t*)(u8BufferWrite + u32count*4U);
	}
	
	/* Wait for the BSY bit to be cleared */
	while((FLASH->SR & FLASH_SR_BSY));
	FLASH->CR &= ~FLASH_CR_PG;
	Flash_Lock();
	return FLASH_NO_ERRORS;
}

/**
  * @brief  Read data from Flash
  * @param  Start Address, data buffer, and data length to read
  * @retval ERROR or NO ERROR
  */
FlashStatus Flash_Read(volatile uint32_t u32StartAddr, uint8_t* u8BufferRead, uint32_t u32Length)
{
	uint32_t u32count;
	
	/* Check inpit param */
	if ((u8BufferRead == 0x00U) || (u32Length == 0U))
	{
		return FLASH_ERRORS;
	}
	
	/* Read data buffer */
	for (u32count = 0U; u32count < u32Length; u32count++)
	{
		*(uint8_t*)(u8BufferRead + u32count) = *(uint8_t*)(u32StartAddr + u32count);
	}
	
	return FLASH_NO_ERRORS;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t u32Address)
{
  uint32_t sector = 0;
  if((u32Address < ADDR_FLASH_SECTOR_1) && (u32Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = 0; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_2) && (u32Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = 1; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_3) && (u32Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = 2; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_4) && (u32Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = 3; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_5) && (u32Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = 4; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_6) && (u32Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = 5; 
  }
  else if((u32Address < ADDR_FLASH_SECTOR_7) && (u32Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = 6; 
  }
  else if((u32Address < SECTOR_END) && (u32Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = 7; 
  }
  return sector;
}
	