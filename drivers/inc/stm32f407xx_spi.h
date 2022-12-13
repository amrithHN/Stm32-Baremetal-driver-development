/*
 * stm32f407xx_spi.h
 *
 *  Created on: Dec 13, 2022
 *      Author: amrith
 */

#ifndef INC_STM32F407XX_SPI_H_
#define INC_STM32F407XX_SPI_H_

#include "stm32f407xx.h"

typedef struct SPI{
	__vo32_t SPI_CR1;
	__vo32_t SPI_CR2;
	__vo32_t SPI_SR;
	__vo32_t SPI_DR;
	__vo32_t SPI_CRCPR;
	__vo32_t SPI_RXCRCR;
	__vo32_t SPI_TXCRCR;
	__vo32_t SPI_I2SCFGR;
	__vo32_t SPI_I2SPR;


}SPI_config_t;








#endif /* INC_STM32F407XX_SPI_H_ */
