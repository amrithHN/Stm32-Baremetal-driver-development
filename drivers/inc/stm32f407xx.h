/*
 * stm3f407xx.h
 *
 *  Created on: Dec 01, 2022
 *      Author: Amrith
 */

#ifndef INC_STM3F407XX_H_
#define INC_STM3F407XX_H_

#include <stdint.h>

#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE

// memory map page 64 of docs

/*
 * <Memory base address>
 *
 */
#define FLASH_BASE_ADDR 0x08000000UL
#define SRAM1_BASE_ADDR 0x20000000UL //page 71
#define SRAM2_BASE_ADDR 0x2001C000UL //page 71
#define ROM 0x1FFF0000UL
#define SRAM SRAM1_BASE_ADDR

/*
 * <APB peripheral base addresses>
 *
 */
#define PERIPH_BASE_ADDR 0x40000000 		/* < all peripherals start from this address page: 65> */
#define APB1_BASE_ADDR PERIPH_BASE_ADDR 	/* <same as base> */
#define APB2_BASE_ADDR 0x40010000UL
#define AHB1_BASE_ADDR 0x40020000UL
#define AHB2_BASE_ADDR 0x50000000


/*
 * <GPIO register address A,B,C,D,E (REMAING NOT POPULATED ON DISC1)>
 *
 */
#define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0000)
#define GPIOB_BASE_ADDR (AHB1_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR (AHB1_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR (AHB1_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR (AHB1_BASE_ADDR + 0x1000)


/*
 * <I2C register address>
 *
 */
#define I2C1_BASE_ADDR (APB1_BASE_ADDR + 0x5400)
#define I2C2_BASE_ADDR (APB1_BASE_ADDR + 0x5800)
#define I2C3_BASE_ADDR (APB1_BASE_ADDR + 0x5C00)


/*
 * <SPI register address>
 *
 */
#define SPI1_BASE_ADDR (APB2_BASE_ADDR + 0x3000)
#define SPI2_BASE_ADDR (APB1_BASE_ADDR + 0x3800)
#define SPI3_BASE_ADDR (APB1_BASE_ADDR + 0x3C00)

/*
 * <USART register address>
 *
 */
#define USART1_BASE_ADDR (APB2_BASE_ADDR + 0x3000)
#define USART2_BASE_ADDR (APB1_BASE_ADDR + 0x4400)
#define USART3_BASE_ADDR (APB1_BASE_ADDR + 0x4800)
#define UART4_BASE_ADDR  (APB1_BASE_ADDR + 0x4C00)
#define UART5_BASE_ADDR  (APB1_BASE_ADDR + 0x5000)
#define UART6_BASE_ADDR (APB2_BASE_ADDR + 0x1400)

/*
 * <RCC base address>
 *
 */
#define RCC_BASE_ADDR (AHB1_BASE_ADDR + 0x3800)


/*
 * <syscfg BASE ADDRESS>
 *
 */
#define SYSCFG_BASE_ADDR (APB2_BASE_ADDR + 0x3800)

/*
 * <EXTI BASE ADDRESS>
 *
 */
#define EXTI_BASE_ADDR (APB2_BASE_ADDR + 0x3C00)


/*
 * <REGISTER DEFINITION FOR EACH PERIPHERAL >
 * - peripherals have a set of registers for its configuration etc
 */

/*
 * <GPIO REG definition page 287>
 *
 */

#define __vo32_t volatile uint32_t
#define __vo8_t volatile uint8_t

typedef struct GPIO{
	__vo32_t MODER;
	__vo32_t OTYPER;
	__vo32_t OSPEEDR;
	__vo32_t PUPDR;
	__vo32_t IDR;
	__vo32_t ODR;
	__vo32_t BSRR;
	__vo32_t LCKR;
	__vo32_t AFR[2]; //AFRL & AFRH together in one array

}GPIO_Reg_Def;

/*
 * <creating pointers to each GPIOs with reg def>
 *
 */

#define GPIOA (GPIO_Reg_Def*) GPIOA_BASE_ADDR
#define GPIOB (GPIO_Reg_Def*) GPIOB_BASE_ADDR
#define GPIOC (GPIO_Reg_Def*) GPIOC_BASE_ADDR
#define GPIOD (GPIO_Reg_Def*) GPIOD_BASE_ADDR
#define GPIOE (GPIO_Reg_Def*) GPIOE_BASE_ADDR



/*
 * <RCC Register defition page:256 >
 *
 */

typedef struct {
	__vo32_t CR;
	__vo32_t PLL_CFGR;
	__vo32_t CFGR;
	__vo32_t CIR;
	__vo32_t AHB1_RSTR;
	__vo32_t AHB2_RSTR;
	__vo32_t AHB3_RSTR;
	__vo32_t RESERVED0;
	__vo32_t APB1_RSTR;
	__vo32_t APB2_RSTR;
	__vo32_t RESERVED1[2];
	__vo32_t AHB1_ENR;
	__vo32_t AHB2_ENR;
	__vo32_t AHB3_ENR;
	__vo32_t RESERVED2;
	__vo32_t APB1_ENR;
	__vo32_t APB2_ENR;
	__vo32_t RESERVED3[2];
	__vo32_t AHB1LPENR;
	__vo32_t AHB2LPENR;
	__vo32_t AHB3LPENR;
	__vo32_t RESERVED4;
	__vo32_t APB1LPENR;
	__vo32_t APB2LPENR;
	__vo32_t RESERVED5[2];
	__vo32_t BDCR;
	__vo32_t CSR;
	__vo32_t RESERVED6[2];
	__vo32_t SSCGR;
	__vo32_t PLLI2SCFGR;

}RCC_RegDef;


#define RCC ((RCC_RegDef*)RCC_BASE_ADDR)

/*
 * <Enable GPIO using macros>
 *  - GPIOS are connected to AHB1 and using AHB1ENR it can be enabled or disabled
 */


#define GPIOA_EN() (RCC->AHB1_ENR |=(1<<0))
#define GPIOB_EN() (RCC->AHB1_ENR |=(1<<1))
#define GPIOC_EN() (RCC->AHB1_ENR |=(1<<2))
#define GPIOD_EN() (RCC->AHB1_ENR |=(1<<3))
#define GPIOE_EN() (RCC->AHB1_ENR |=(1<<4))



#define GPIOA_DI() (RCC->AHB1_ENR &= ~(1<<0))
#define GPIOB_DI() (RCC->AHB1_ENR &=~(1<<1))
#define GPIOC_DI() (RCC->AHB1_ENR &=~(1<<2))
#define GPIOD_DI() (RCC->AHB1_ENR &=~(1<<3))
#define GPIOE_DI() (RCC->AHB1_ENR &=~(1<<4))

/*
 * <Enable I2C using macros>
 *  - I2C1,2,3 is present in apb1 bus and enabled using RCC_APB1ENR
 */

#define I2C1_EN() (RCC->APB1_ENR |=(1<<21))
#define I2C2_EN() (RCC->APB1_ENR |=(1<<22))
#define I2C3_EN() RCC->APB1_ENR |=(1<<23)

#define I2C1_DI() (RCC->APB1_ENR &=~(1<<21))
#define I2C2_DI() (RCC->APB1_ENR &=~(1<<22))
#define I2C3_DI() RCC->APB1_ENR &=~(1<<23)

/*
 * <Enable SPI using macros>
 *  - SPI 1,2,3 is present in apb1 bus and enabled using RCC_APB1ENR
 */

//SPI1 APB2
#define SPI1_EN() (RCC->APB2_ENR |=(1<<12))
//SPI2 APB1
#define SPI2_EN() (RCC->APB1_ENR |=(1<<14))
//SPI3 APB1
#define SPI3_EN() (RCC->APB1_ENR |=(1<<15))


//SPI1 APB2
#define SPI1_DI() (RCC->APB2_ENR &=~(1<<12))
//SPI2 APB1
#define SPI2_DI() (RCC->APB1_ENR &=~(1<<14))
//SPI3 APB1
#define SPI3_DI() (RCC->APB1_ENR &=~(1<<15))


/*
 * <Enable UART using macros>
 *  - UART 1,2,3,4,5,6 present on 2,3,4,5 -> apb1 and 1,6 apb2
 */

#define USART2_EN() (RCC->APB1_ENR |=(1<<17))
#define USART3_EN() (RCC->APB1_ENR |=(1<<18))
#define USART4_EN() (RCC->APB1_ENR |=(1<<19))
#define USART5_EN() (RCC->APB1ENR |=(1<<20))


#define USART1_EN() (RCC->APB2_ENR |=(1<<4))
#define USART6_EN() (RCC->APB2_ENR |=(1<<5))



#define USART2_DI() (RCC->APB1_ENR &=~(1<<17))
#define USART3_DI() (RCC->APB1_ENR &=~(1<<18))
#define USART4_DI() (RCC->APB1_ENR &=~(1<<19))
#define USART5_DI() (RCC->APB1_ENR &=~(1<<20))


#define USART1_DI() (RCC->APB2_ENR &=~(1<<4))
#define USART6_DI() (RCC->APB2_ENR &=~(1<<5))



/*
 * <Enable SYSCFG using macros>
 *  -  apb2
 */
#define SYSCFG_EN() (RCC->APB2_ENR |=(1<<14))

#define SYSCFG_DI() (RCC->APB2_ENR &=~(1<<14))


/*
 * EXTI Handle structure
 * --mapping page:382
 *
 */

typedef struct {

	__vo32_t EXTI_IMR;
	__vo32_t EXTI_EMR;
	__vo32_t EXTI_RTSR;
	__vo32_t EXTI_FTSR;
	__vo32_t EXTI_SWIER;
	__vo32_t EXTI_PR;

}EXTI_handle_t;

#define EXTI ((EXTI_handle_t*)EXTI_BASE_ADDR)

/*
 * SYSCFG Handle structure
 * --mapping page:294
 *
 */

typedef struct {

	__vo32_t SYSCFG_MEMRMP; //00
	__vo32_t SYSCFG_PMC; //04
	__vo32_t SYSCFG_EXTICR[4];//08 0c 10 14
	uint16_t RESERVED;// 18 -> 20
	__vo32_t SYSCFG_CMPCR; //20-> 24

}SYSCFG_handle_t;

#define SYSCFG ((SYSCFG_handle_t*)SYSCFG_BASE_ADDR)



/*
 * <SPI reg definition and macros>
 *
 */


typedef struct SPI{
	__vo32_t CR1;
	__vo32_t CR2;
	__vo32_t SR;
	__vo32_t DR;
	__vo32_t CRCPR;
	__vo32_t RXCRCR;
	__vo32_t TXCRCR;
	__vo32_t I2SCFGR;
	__vo32_t I2SPR;

}SPI_regdef_t;



#endif /* INC_STM3F407XX_H_ */
