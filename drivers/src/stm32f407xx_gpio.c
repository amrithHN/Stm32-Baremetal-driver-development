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
	return (pGPIOx->IDR | mask);

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
