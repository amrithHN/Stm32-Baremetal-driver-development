/*
 * stm32f407xx_spi.h
 *
 *  Created on: Dec 13, 2022
 *      Author: amrith
 */

#ifndef INC_STM32F407XX_SPI_H_
#define INC_STM32F407XX_SPI_H_

#include "stm32f407xx.h"

//steps to configure as Master
//1. set BR[]
//2. CPOL and CPHA
//3. dff 8/16bit format
//4.LSB first if not in TI SPI mode
//5.set the SSM and SSI bits in the SPI_CR1 register for Soft Slave Mgmt
//opt: frf bit set if TI SPI mode
//mstr ans spe must be set


typedef struct {

	__vo8_t Mode;
	__vo8_t Bus_config;
	__vo8_t DFF;
	__vo8_t CPHA;
	__vo8_t CPOL;
	__vo8_t SSM;
	__vo8_t speed;

}SPI_config_t;

typedef struct{
	SPI_regdef_t *pSPIx;
	SPI_config_t SPI_config;
}SPI_handle_t;


void SPI_init(SPI_handle_t* SPI_handle);
void SPI_deinit();
void SPI_tx();
void SPI_rx();


#endif /* INC_STM32F407XX_SPI_H_ */
