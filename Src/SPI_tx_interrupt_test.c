/*
 * SPI_tx_test.c
 *
 *  Created on: Dec 16, 2022
 *      Author: amrith
 */


#include <stdint.h>
#include "stm32f407xx.h"
#include<string.h>

//stm32f407
//SPI2 MOSI PB15
//SPI2 MISO PB14
//SPI2 SCk PB13
//spi2 nss PB12
//AF mode 5

void Pin_init(){
	GPIO_handle_t Spi2_handle;

	Spi2_handle.pGPIOx = GPIOB;
	Spi2_handle.pin_config.pin_alternate_function = 5;
	Spi2_handle.pin_config.pin_speed = HIGH;
	Spi2_handle.pin_config.pin_mode = ALT_FUNC;
	Spi2_handle.pin_config.pin_input_mode = NOPULL;

	//SPI2 MOSI PB15
	Spi2_handle.pin_config.pin_number = 15;
	GPIO_init(&Spi2_handle);

	//SPI2 MISO PB14
	Spi2_handle.pin_config.pin_number = 13;
	GPIO_init(&Spi2_handle);

	//SPI2 SCk PB13
	Spi2_handle.pin_config.pin_number = 12;
	GPIO_init(&Spi2_handle);

	//spi2 nss PB12
	Spi2_handle.pin_config.pin_number = 14;
	GPIO_init(&Spi2_handle);



}
SPI_handle_t pSPIx ;

void spi_conf(){



	pSPIx.pSPIx = SPI2;
	pSPIx.SPI_config.Mode = SPI_MASTER;
	pSPIx.SPI_config.Bus_config = DUPLEX;
	pSPIx.SPI_config.speed = BRR_2;
	pSPIx.SPI_config.SSM = SOFTWARE_MGT;
	pSPIx.SPI_config.DFF = DFF_8;
	pSPIx.SPI_config.CPOL = IDLE_MODE_HIGH;
	pSPIx.SPI_config.CPHA = LEADING_EDGE;


	SPI_init(&pSPIx);
	char str[]="Hello World";
	pSPIx.TxState = SPI_IDLE;
	pSPIx.RxState = SPI_IDLE;
	SPI_IRQConfig(&pSPIx, ENABLE);


	SPI_txData_IT(&pSPIx,(uint8_t *)str,strlen(str));

}

int main(){



	Pin_init();
	spi_conf();






	while(1)
	{
		//loop here

	}
}


void SPI2_IRQHandler(void){

	SPI_IRQHadling(&pSPIx);
}
