/*
 * stm32f407xx_spi.h
 *
 *  Created on: Dec 13, 2022
 *      Author: amrith
 */

#ifndef INC_STM32F407XX_SPI_H_
#define INC_STM32F407XX_SPI_H_

#include "stm32f407xx.h"
#include "stddef.h"

//steps to configure as Master
//1. set BR[]
//2. CPOL and CPHA
//3. dff 8/16bit format
//4.LSB first if not in TI SPI mode
//5.set the SSM and SSI bits in the SPI_CR1 register for Soft Slave Mgmt
//opt: frf bit set if TI SPI mode
//mstr ans spe must be set


#define SPI_SLAVE 0
#define SPI_MASTER 1

#define DUPLEX 0
#define HALF_DUPLEX 1
#define SIMPLEX_RX_ONLY 2

#define DFF_8 0
#define DFF_16 1


#define IDLE_MODE_LOW  0
#define IDLE_MODE_HIGH 1

#define LEADING_EDGE 0
#define LAGGING_EDGE 1

#define SOFTWARE_MGT 1
#define HARDWARE_MGT 0

#define BRR_2 		0
#define BRR_4 		1
#define BRR_8 		2
#define BRR_16 		3
#define BRR_32 		4
#define BRR_64 		5
#define BRR_128 	6
#define BRR_256 	7

#define SPI_SR_RXNE 0
#define SPI_SR_TX 	1
#define SPI_SR_BSY 	7

#define SPI_IDLE 	0
#define SPI_BSY_TX 	1
#define SPI_BSY_RX	2
#define SPI_ERR_OVF 3


#define SPI1_NVIC 35
#define SPI2_NVIC 36
#define SPI3_NVIC 51


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
	uint8_t *TxBuffer;
	uint8_t *RxBuffer;
	uint32_t Txlen;
	uint32_t Rxlen;
	uint8_t TxState;
	uint8_t RxState;

}SPI_handle_t;



void SPI_ErrEvent(SPI_handle_t* SPI_handle,uint8_t flaf);

void SPI_clr_ovrflag(SPI_handle_t* SPI_handle);
void SPI_close_tx(SPI_handle_t* SPI_handle);
void SPI_close_rx(SPI_handle_t* SPI_handle);

void SPI_CLK_EN(SPI_handle_t* SPI_handle,uint8_t flag);
void SPI_ssi_en(SPI_handle_t* SPI_handle, uint8_t flag);
uint8_t get_SPIstatusflag(SPI_regdef_t* pSPIx,uint8_t bit_position);
void SPI_init(SPI_handle_t* SPI_handle);
void SPI_deinit(SPI_handle_t* SPI_handle);
void SPI_txData(SPI_regdef_t* pSPIx,uint8_t *TxBuffer,uint32_t len);
void SPI_rxData(SPI_regdef_t* pSPIx,uint8_t *RxBuffer,uint32_t len);

//interrupt functions
uint8_t SPI_txData_IT(SPI_handle_t* SPI_handle,uint8_t *TxBuffer,uint32_t len);
uint8_t SPI_rxData_IT(SPI_handle_t* SPI_handle,uint8_t *RxBuffer,uint32_t len);


void SPI_IRQConfig(SPI_handle_t* SPI_handle,uint8_t flag);
void SPI_IRQPriority(SPI_handle_t* SPI_handle,uint8_t priority);
void SPI_IRQHadling(SPI_handle_t* SPI_handle);


#endif /* INC_STM32F407XX_SPI_H_ */
