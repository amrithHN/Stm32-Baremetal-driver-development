#include "stm32f407xx_gpio.h"


/*********************************************************************
 * @fn      		  - GPIO_Enable
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE flag
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */

void GPIO_Enable(GPIO_Reg_Def* pGPIOx,uint8_t flag){

			if(pGPIOx==GPIOA){

				flag?GPIOA_EN(): GPIOA_DI();
			}
			else if(pGPIOx==GPIOB)
			{
				flag?GPIOB_EN(): GPIOB_DI();
			}
			else if(pGPIOx==GPIOC)
			{
				flag?GPIOC_EN(): GPIOC_DI();
			}
			else if(pGPIOx==GPIOD)
			{
				flag?GPIOD_EN(): GPIOD_DI();
			}
			else if(pGPIOx==GPIOE)
			{
				flag?GPIOE_EN(): GPIOE_DI();
			}

}

/*********************************************************************
 * @fn      		  - GPIO_init
 *
 * @brief             -
 *
 * @param[in]         - initilises GPIO port and pin with specified mode and configuration
 * @param[in]         - base address of GPIO_handle
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_init(GPIO_handle_t* pGPIO_handle){


	if(pGPIO_handle->pin_config.pin_mode<=3)
	{
	//MODER
		uint32_t temp = pGPIO_handle->pin_config.pin_mode<<(2*pGPIO_handle->pin_config.pin_number);
		pGPIO_handle->pGPIOx->MODER|=temp;
	//SPEED
		temp = pGPIO_handle->pin_config.pin_speed<<(2*pGPIO_handle->pin_config.pin_number);
		pGPIO_handle->pGPIOx->OSPEEDR|=temp;

	//OUPUT type
		if(pGPIO_handle->pin_config.pin_mode==OUTPUT)
		{
			temp = pGPIO_handle->pin_config.pin_output_mode<<(pGPIO_handle->pin_config.pin_number);
			pGPIO_handle->pGPIOx->OTYPER|=temp;
		}

	//INPUT pull type
		if(pGPIO_handle->pin_config.pin_mode==INPUT)
		{
				temp = pGPIO_handle->pin_config.pin_input_mode<<(2*pGPIO_handle->pin_config.pin_number);
				pGPIO_handle->pGPIOx->PUPDR|=temp;
		}

	//Alternate function
		if(pGPIO_handle->pin_config.pin_alternate_function == ALT_FUNC)
		{

				//less -- 0..7
				uint32_t temp1 = pGPIO_handle->pin_config.pin_number/8;
				uint32_t temp2 = pGPIO_handle->pin_config.pin_number%8;
				pGPIO_handle->pGPIOx->AFR[temp1] = pGPIO_handle->pin_config.pin_alternate_function<<(4*temp2);



		}
	}
	else{	//Interrupt mode configuration

		if(pGPIO_handle->pin_config.pin_mode == INTERRUPT_FT)
		{
			//falling mode interrupt trigger , for each pin number 0-16 there is a exti line
			EXTI->EXTI_FTSR |= (1<<pGPIO_handle->pin_config.pin_number); // enable Falling edge
			EXTI->EXTI_RTSR &= ~(1<<pGPIO_handle->pin_config.pin_number); // disable rising edge
		}
		else if(pGPIO_handle->pin_config.pin_mode == INTERRUPT_RT)
		{
			//rising edge interrupt trigger
			EXTI->EXTI_RTSR |= (1<<pGPIO_handle->pin_config.pin_number); // enable rising edge
			EXTI->EXTI_FTSR &= ~(1<<pGPIO_handle->pin_config.pin_number); // disable Falling edge

		}
		else if (pGPIO_handle->pin_config.pin_mode == INTERRUPT_FRT)
		{
			//rising and falling edge interrupt trigger
			EXTI->EXTI_RTSR |= (1<<pGPIO_handle->pin_config.pin_number); // enable rising edge
			EXTI->EXTI_FTSR |= (1<<pGPIO_handle->pin_config.pin_number); // enable Falling edge

		}

		//There are 4 EXTICR register , 4 bits represent one port , EXTICR1 - 0-3 pin (any port) EXTICR2 - 4-7 , EXTICR3 8-11
		//EXTICR3 12-15

		uint8_t register_bank = pGPIO_handle->pin_config.pin_number/4 + 1 ;
		uint8_t section_select = pGPIO_handle->pin_config.pin_number%4;
		uint8_t temp2 =PORT_NUM_MAP(pGPIO_handle->pGPIOx);
		SYSCFG_EN();
		SYSCFG->SYSCFG_EXTICR[register_bank] =  temp2 <<(4*section_select);


		//config IMR
		EXTI->EXTI_IMR |= (1<<pGPIO_handle->pin_config.pin_number); //not mask the interrupt


	}

}


/*********************************************************************
 * @fn      		  - GPIO_RESET
 *
 * @brief             -
 *
 * @param[in]         - RESET GPIO port
 * @param[in]         - base address of GPIO
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */

void GPIO_RESET(GPIO_Reg_Def* pGPIOx){

	if(pGPIOx==GPIOA){

		GPIOA_RESET();
	}
	else if(pGPIOx==GPIOB)
	{
		GPIOB_RESET();
	}
	else if(pGPIOx==GPIOC)
	{
		GPIOC_RESET();
	}
	else if(pGPIOx==GPIOD)
	{
		GPIOD_RESET();
	}
	else if(pGPIOx==GPIOE)
	{
		GPIOE_RESET();
	}

}

uint8_t GPIO_read_pin(GPIO_Reg_Def* pGPIOx,uint8_t pin){

	uint32_t mask = 1<<pin;
	return (pGPIOx->IDR & mask);

}

uint16_t GPIO_read_port(GPIO_Reg_Def* pGPIOx){

	uint16_t value = (uint16_t)pGPIOx->IDR;
	return (value);

}


void GPIO_write_pin(GPIO_Reg_Def* pGPIOx,uint8_t pin,uint8_t value){

	if(value)
		pGPIOx->ODR |= (1<<pin);
	else
		pGPIOx->ODR &= ~(1<<pin);
}



void GPIO_write_port(GPIO_Reg_Def* pGPIOx,uint16_t value){
	pGPIOx->ODR = value;
}


void GPIO_toggle_pin(GPIO_Reg_Def* pGPIOx,uint8_t pin){


		pGPIOx->ODR ^= (1<<pin);


}



void GPIO_IRQConfig(uint8_t IRQnumber,uint8_t flag){

	if(flag){

		//enable interrupts processor side using ISER0-4
		if(IRQnumber<32)
		{
			//0-31 interupts in this ISER0 as given in programming manual 1:enable 0:No effect
			*NVIC_ISER0 |= (1<<IRQnumber);
		}
		else if(IRQnumber>=32 && IRQnumber<=63)
		{
			*NVIC_ISER1 |=(1<<IRQnumber%32);
		}
		else if(IRQnumber>=64 && IRQnumber<=95)
		{
			*NVIC_ISER2 |=(1<<IRQnumber%64);
		}
		else if(IRQnumber>=96 && IRQnumber<=127)
		{
			*NVIC_ISER3 |=(1<<IRQnumber%96);
		}
	}
	else
	{
		//disable interrupts processor side using ICER0-4
		if(IRQnumber<32)
		{
			//0-31 interupts in this ICER0 as given in programming manual 1:disable 0:No effect
			*NVIC_ICER0 |= (1<<IRQnumber);
		}
		else if(IRQnumber>=32 && IRQnumber<=63)
		{
			*NVIC_ICER1 |=(1<<IRQnumber%32);
		}
		else if(IRQnumber>=64 && IRQnumber<=95)
		{
			*NVIC_ICER2 |=(1<<IRQnumber%64);
		}
		else if(IRQnumber>=96 && IRQnumber<=127)
		{
			*NVIC_ICER3 |=(1<<IRQnumber%96);
		}
	}


}

void GPIO_IRQPriority(uint8_t IRQ_number,uint8_t IRQ_priority){

	uint8_t temp1 = IRQ_number/4;
	uint8_t temp2 = IRQ_number%4;
//only upper nibble is used lower nibble is discarded so shift the value
	uint8_t shift_temp = (8*temp2 + 4) ; // so each shift is 12 times discarding upper nibble of priority and using lower nibble to upper nibble of IPRx register
	*(NVIC_IPRx(temp1)) |= IRQ_priority<<shift_temp;

}

void IRQ_handling(uint8_t pin){

	if(EXTI->EXTI_PR)
	{
		EXTI->EXTI_PR |=(1<<pin);
	}
}
