#include "Stm32f407xx_spi.h"
#include "stm32f407xx_gpio.h"

static void SPI_tx_handle(SPI_handle_t* SPI_handle); //handle tx interrupt
static void SPI_rx_handle(SPI_handle_t* SPI_handle); //handle rx interrupt
static void SPI_err_handle(SPI_handle_t* SPI_handle); //handle errors during SPI interrupt

void SPI_CLK_EN(SPI_handle_t* SPI_handle,uint8_t flag){

	if(flag)
	{
			if(SPI_handle->pSPIx == SPI1)
			{
				RCC->APB2_ENR |= 1<<12;

			}
			if(SPI_handle->pSPIx == SPI2)
			{
				RCC->APB1_ENR |= 1<<14;
			}
			if(SPI_handle->pSPIx == SPI3)
			{
				RCC->APB1_ENR |= 1<<15;
			}
	}
	else
	{
		if(SPI_handle->pSPIx == SPI1)
		{
			RCC->APB2_ENR &= ~(1<<12);

		}
		if(SPI_handle->pSPIx == SPI2)
		{
			RCC->APB1_ENR &= ~(1<<14);
		}

		if(SPI_handle->pSPIx == SPI3)
		{
			RCC->APB1_ENR &= ~(1<<15);
		}
	}
}


void SPI_ssi_en(SPI_handle_t* SPI_handle, uint8_t flag)
{
	if(flag)
	{

		SPI_handle->pSPIx->CR1 |= (1<<8); //set ssi
	}
	else{
		SPI_handle->pSPIx->CR1 &= ~(1<<8);
	}
}

void SPI_init(SPI_handle_t* SPI_handle){



 //master 0 slave 1 page 917
	uint32_t temp=0;

	SPI_CLK_EN(SPI_handle, ENABLE);

	temp |=(SPI_handle->SPI_config.Mode<<2); //master or slave config

	if(SPI_handle->SPI_config.Bus_config == DUPLEX)
	{
	 temp &= ~(1<<10); //full duplex write 0 to 10th bit
	 temp &= ~(1<<15);

	}
	else if(SPI_handle->SPI_config.Bus_config == HALF_DUPLEX)
	{
		 temp |= (0<<15); // 1 line BIDI mode
	}
	else if(SPI_handle->SPI_config.Bus_config == SIMPLEX_RX_ONLY){
		//simple tx mode is basically full duplex with rx disconnected!

		temp |= (1<<10); //RX only mode
	}


	//DFF
	temp |=(SPI_handle->SPI_config.DFF<<11); // set if 16 bit

	//CPOL
	temp |= SPI_handle->SPI_config.CPOL<<1;

	//CPHA
	temp |= SPI_handle->SPI_config.CPHA<<0;

	//SPEED
	temp |= SPI_handle->SPI_config.speed<<3;

	//SSM
	temp |= SPI_handle->SPI_config.SSM<<9;


	SPI_handle->pSPIx->CR1 = temp; // set CR1 with the init values
	SPI_ssi_en(SPI_handle,ENABLE);
	SPI_handle->pSPIx->CR1 |= (1<<6); //set spe to enable SPI




}



void SPI_deinit(SPI_handle_t* SPI_handle){

	// disable peripheral
	SPI_handle->pSPIx->CR1 &= ~(1<<6); //spe bit reset to disable


}


uint8_t get_SPIstatusflag(SPI_regdef_t* pSPIx,uint8_t bit_position){

	return (pSPIx->SR & (1<<bit_position));

}


void SPI_txData(SPI_regdef_t* pSPIx,uint8_t *TxBuffer,uint32_t len){

	while(len){

		while(!get_SPIstatusflag(pSPIx,SPI_SR_TX));

		if( (pSPIx->CR1 & (1<<11)) == 0){
			//8 bit DFF
			pSPIx->DR =*TxBuffer;
			len--;
			TxBuffer++;
		}
		else{
			//16 bit DFF

			pSPIx->DR = *((uint16_t*)TxBuffer);
			len-=2;
			TxBuffer+=2; //send 2 bytes so uint8_t should be incremented by 2 , if typecasted to uint16_t then only one increment
		}

	}
}


void SPI_rxData(SPI_regdef_t* pSPIx,uint8_t *RxBuffer,uint32_t len){


	while(len){

			while(!get_SPIstatusflag(pSPIx,SPI_SR_RXNE));

			if( (pSPIx->CR1 & (1<<11)) == 0){
				//8 bit DFF
				*RxBuffer = pSPIx->DR;
				len--;
				RxBuffer++;
			}
			else{
				//16 bit DFF

				*((uint16_t*)RxBuffer) = pSPIx->DR;
				len-=2;
				RxBuffer+=2; //send 2 bytes so uint8_t should be incremented by 2 , if typecasted to uint16_t then only one increment
			}

		}
}




//interrupt functions
//page 898
uint8_t SPI_txData_IT(SPI_handle_t* SPI_handle,uint8_t *TxBuffer,uint32_t len){

	uint8_t state = SPI_handle->TxState;

	if(SPI_handle->RxState==SPI_IDLE)
	{
		SPI_handle->TxBuffer=TxBuffer; //save in handle structure
		SPI_handle->Txlen = len; //save length
		SPI_handle->TxState = SPI_BSY_TX; //mark it as busy

		//enable TXIE
		SPI_handle->pSPIx->CR2 |= (1<<7); // 7 is TXIE bit
		state = SPI_handle->TxState;
	}

	return state;

}

uint8_t SPI_rxData_IT(SPI_handle_t* SPI_handle,uint8_t *RxBuffer,uint32_t len){


	uint8_t state = SPI_handle->RxState;

	if(SPI_handle->TxState==SPI_IDLE)
	{
		SPI_handle->TxBuffer = RxBuffer; //save in handle structure
		SPI_handle->Rxlen = len; //save length
		SPI_handle->RxState = SPI_BSY_TX; //mark it as busy

		//enable TXIE
		SPI_handle->pSPIx->CR2 |=(1<<6); // 6 is RXINE bit
		state = SPI_handle->RxState;
	}

	return state;

}

void SPI_IRQHadling(SPI_handle_t* SPI_handle){
	uint8_t temp1=0,temp2=0;

	temp1 = SPI_handle->pSPIx->CR2 & (1<<7); //TXIE flag check 1 if not masked(ENable)
	temp2 = SPI_handle->pSPIx->SR & (1<<1); //TXE bit check 1 empty , 0 not emty

	if(temp1 && temp2 ){
		//if TXIE enabled/unmaked and TXE empty then
		//call Tx interrupt handle function

		SPI_tx_handle(SPI_handle);

	}

	temp1 = SPI_handle->pSPIx->CR2 & (1<<6); //RXNEIE flag check 1 if not masked(ENable)
	temp2 = SPI_handle->pSPIx->SR & (1<<0); //RXNE bit check 0 empty , 1 not emty

	if(temp1 && temp2 ){
		//if RXNEIE enabled/unmasked and RXNE not empty(data arrived!)
		//call Rx interrupt handle function

		SPI_rx_handle(SPI_handle);

	}

	temp1 = SPI_handle->pSPIx->CR2 & (1<<5); //EERIE flag check
	temp2 = SPI_handle->pSPIx->SR & (1<<6); //OVF flag 1 - overrun error else 0

	if(temp1 && temp2 ){
		//if RXNEIE enabled/unmasked and RXNE not empty(data arrived!)
		//call Rx interrupt handle function

		SPI_err_handle(SPI_handle);

	}

}



void SPI_IRQConfig(SPI_handle_t* SPI_handle,uint8_t flag){

	uint8_t temp=0;

	if(SPI_handle->pSPIx == SPI1)
		temp = SPI1_NVIC;
	else if(SPI_handle->pSPIx == SPI2)
		temp = SPI2_NVIC;
	else if(SPI_handle->pSPIx == SPI3)
		temp = SPI3_NVIC;
	else
		return;

	GPIO_IRQConfig(temp,flag);

}


void SPI_IRQPriority(SPI_handle_t* SPI_handle,uint8_t priority){

	uint8_t temp=0;

	if(SPI_handle->pSPIx == SPI1)
		temp = SPI1_NVIC;
	else if(SPI_handle->pSPIx == SPI2)
		temp = SPI2_NVIC;
	else if(SPI_handle->pSPIx == SPI3)
		temp = SPI3_NVIC;
	else
		return;

	GPIO_IRQPriority(temp,priority);
}


static void SPI_tx_handle(SPI_handle_t* SPI_handle){ //handle tx interrupt

	while(SPI_handle->Txlen)
	{
	if( (SPI_handle->pSPIx->CR1 & (1<<11)) == 0){
				//8 bit DFF
				SPI_handle->pSPIx->DR =*SPI_handle->TxBuffer;
				SPI_handle->Txlen--;
				SPI_handle->TxBuffer++;
			}
			else{
				//16 bit DFF

				SPI_handle->pSPIx->DR = *((uint16_t*)SPI_handle->TxBuffer);
				SPI_handle->Txlen-=2;
				SPI_handle->TxBuffer+=2; //send 2 bytes so uint8_t should be incremented by 2 , if typecasted to uint16_t then only one increment
			}
	}

	if(!SPI_handle->Txlen)
	{
		//reset TXIE state and TXbuffer and TX length

		SPI_close_tx(SPI_handle);
	}

}

static void SPI_rx_handle(SPI_handle_t* SPI_handle){ //handle rx interrupt


			if( (SPI_handle->pSPIx->CR1 & (1<<11)) == 0){
				//8 bit DFF
				*SPI_handle->RxBuffer = SPI_handle->pSPIx->DR;
				SPI_handle->Rxlen++;

			}
			else{
				//16 bit DFF

				*((uint16_t*)SPI_handle->TxBuffer) = SPI_handle->pSPIx->DR ;
				SPI_handle->Rxlen+=2;

			}

			if(!SPI_handle->Rxlen)
				{
					//reset TXIE state and RXbuffer and RX length
				SPI_close_rx(SPI_handle);
				}


}
static void SPI_err_handle(SPI_handle_t* SPI_handle){ //handle errors during SPI interrupt
	// only overrun error is applicable
	//clear ovf flag

	if(SPI_handle->TxState != SPI_BSY_TX )
	{
		//clear ovf flag page 914
		SPI_clr_ovrflag(SPI_handle);

	}
	//if SPI TX busy handle OVF flag by notifying
	SPI_ErrEvent(SPI_handle,SPI_ERR_OVF);


}

__attribute__((weak)) void SPI_ErrEvent(SPI_handle_t* SPI_handle,uint8_t flag){


}


void SPI_clr_ovrflag(SPI_handle_t* SPI_handle){

	uint32_t temp1 = SPI_handle->pSPIx->DR; //read DR
	temp1 = SPI_handle->pSPIx->SR; //read SR
	(void) temp1;
}

void SPI_close_tx(SPI_handle_t* SPI_handle){

	SPI_handle->pSPIx->CR2 = ~(1<<7); //reset TXIE to mask
	SPI_handle->TxBuffer=NULL;
	SPI_handle->Txlen = 0;
	SPI_handle->TxState = SPI_IDLE;
}

void SPI_close_rx(SPI_handle_t* SPI_handle){

	//reset TXIE state and RXbuffer and RX length
	SPI_handle->pSPIx->CR2 = ~(1<<6); //reset RXIE to mask
	SPI_handle->RxBuffer=NULL;
	SPI_handle->Rxlen = 0;
	SPI_handle->RxState = SPI_IDLE;

}

