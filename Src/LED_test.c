/*
 * LED_test.c
 *
 *  Created on: Dec 13, 2022
 *      Author: amrith
 */

#include "stm32f407xx.h"


//output led with gpio handle for init
void led_init(GPIO_handle_t* pGPIO_handle){

	GPIO_Enable(pGPIO_handle->pGPIOx,1); // enable peripheral
	GPIO_init(pGPIO_handle); // init GPIO with pin config
}


// toggle leds onboard
void led_toggle(GPIO_Reg_Def* pGPIOx,uint8_t pin){

	GPIO_toggle_pin(pGPIOx, pin);
}


//push buutton init
void sw_init(GPIO_handle_t* pGPIO_handle){

	GPIO_Enable(pGPIO_handle->pGPIOx,1); // enable peripheral
	GPIO_init(pGPIO_handle); // init GPIO with pin config
}

// read push button
uint8_t sw_read(GPIO_Reg_Def* pGPIOx,uint8_t pin){

	return GPIO_read_pin(pGPIOx,pin);

}




void delay(){
	for(uint32_t i=0;i<300000;i++);
}

void main(){
	// PD12,13,14,15 are leds PA0 is push button

	GPIO_handle_t led ,button;
	led.pGPIOx=GPIOD;
	led.pin_config.pin_number = 12;

	led.pin_config.pin_mode = OUTPUT;
	led.pin_config.pin_output_mode = OUTPUT_PUSHPULL;
	led.pin_config.pin_input_mode = NOPULL;
	led.pin_config.pin_speed = LOW;


	button.pGPIOx=GPIOA;
	button.pin_config.pin_number=0;

	button.pin_config.pin_mode=INPUT;
	button.pin_config.pin_speed=LOW;
	button.pin_config.pin_input_mode=NOPULL;



	GPIO_Enable(GPIOD,ENABLE); //enable clock peripheral
	GPIO_Enable(GPIOA,ENABLE); //enable clock peripheral

	GPIO_init(&led);
	GPIO_init(&button);

	while(1)
	{
		if(sw_read(GPIOA,0)==1)
		{
		GPIO_toggle_pin(GPIOD, 12);
		delay();
		}
	}

}

