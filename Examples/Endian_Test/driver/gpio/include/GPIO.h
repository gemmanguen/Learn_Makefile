#ifndef	_GPIO_H
#define	_GPIO_H

typedef unsigned int 	uint32_t;
#define LED_GREEN	5

/*!< Peripheral memory map */
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000UL)

/*!< AHB1 peripherals */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)

/* GPIO Offset config */
typedef struct
{
  volatile uint32_t MODE;    /*!< GPIO port mode register,               Address offset: 0x00      */
  volatile uint32_t TYPE;   /*!< GPIO port output type register,        Address offset: 0x04      */
  volatile uint32_t SPEED;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  volatile uint32_t PU;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  volatile uint32_t IN;      /*!< GPIO port input data register,         Address offset: 0x10      */
  volatile uint32_t OUT;      /*!< GPIO port output data register,        Address offset: 0x14      */
  volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
} GPIO_OFF;

#define PA               ((GPIO_OFF *) GPIOA_BASE)
#define PC               ((GPIO_OFF *) GPIOC_BASE)

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

#define RCC                ((RCC_OFF *) RCC_BASE)

/* function led prototype */
void GPIO_Init(void);
void myDelay(uint32_t);
void Clock_Init(void);
int Check_Endian(void);

#endif	/* !GPIO.H */

