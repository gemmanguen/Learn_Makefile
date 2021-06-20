#ifndef _FLASH_H
#define _FLASH_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/*!< Peripheral memory map */
#define FLASH_BASE			0x08000000UL /*!< FLASH(up to 1 MB) base address in the alias region */
#define FLASH_END			0x0807FFFFUL /*!< FLASH end address */
#define PERIPH_BASE			0x40000000UL /*!< Peripheral base address in the alias region */
#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE		(PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE		(PERIPH_BASE + 0x10000000UL)

#define FLASH_R_BASE		(AHB1PERIPH_BASE + 0x3C00UL)
#define RCC_BASE			(AHB1PERIPH_BASE + 0x3800UL)
#define RCC					((RCC_OFF *) RCC_BASE)

#define FLASH				((FLASH_TypeDef *) FLASH_R_BASE)

/* CLock Register Offset */
typedef struct
{
  volatile uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  uint32_t RV1[11];
  volatile uint32_t AHB1;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  volatile uint32_t AHB2;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  uint32_t RV2[2];
  volatile uint32_t APB1;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  volatile uint32_t APB2;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
} RCC_OFF;

typedef enum {
	FLASH_ERRORS = 0U,	/* There is an error */
	FLASH_NO_ERRORS		/* There is no error */ 
} FlashStatus;

/* FLASH_Keys */
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
	
/* FLASH Registers */
typedef struct
{
  volatile uint32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
  volatile uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
  volatile uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
  volatile uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
  volatile uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
} FLASH_TypeDef;

/*****************  Bits definition for FLASH_CR register  ****************/           
#define FLASH_CR_LOCK					(0x1UL << 31U)		/*!< 0x80000000 */
#define FLASH_CR_PG						(0x1UL << 0U)		/*!< 0x00000001 */                                   
#define FLASH_CR_SER					(0x1UL << 1U)		/*!< 0x00000002 */
#define FLASH_CR_STRT					(0x1UL << 16U)		/*!< 0x00010000 */  
#define FLASH_CR_SNB					(0x1FUL << 3U)		/*!< 0x000000F8 */                 
#define FLASH_CR_SNB_0					(0x01UL << 3U)		/*!< 0x00000008 */
#define SNB_Bits						(3U)
/*****************  Bits definition for FLASH_SR register  ****************/
#define FLASH_SR_BSY					(0x1UL << 16U)		/*!< 0x00010000 */

/* Base address of the Flash sectors */

#define ADDR_FLASH_SECTOR_0		((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1		((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2		((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3		((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4		((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5		((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6		((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7		((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define SECTOR_END				((uint32_t)0x0807FFFF)
/*----------------------- Function Prototypes ---------------------*/
void Flash_Init(void);
void Flash_Unlock (void);
void Flash_Lock(void);
void Flash_Erase(uint32_t sector);
uint32_t GetSector(uint32_t Address);
FlashStatus Flash_Write(volatile uint32_t u32StartAddr, uint8_t* u8BufferWrite, uint32_t u32Length);
FlashStatus Flash_Read(volatile uint32_t u32StartAddr, uint8_t* u8BufferRead, uint32_t u32Length);

#endif