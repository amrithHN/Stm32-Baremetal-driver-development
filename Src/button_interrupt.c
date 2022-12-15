/*
 * Button_interrupt.c
 *
 *  Created on: Dec 15, 2022
 *      Author: amrith
 */

#include "button_interrupt.h"
#include <string.h>

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
	for(uint32_t i=0;i<20000;i++);
}

uint8_t test=0;

void led_test(){
	// PD12,13,14,15 are leds PA0 is push button

	GPIO_handle_t led ,button;
	memset(&led,0,sizeof(led)); //clear stuct values
	memset(&button,0,sizeof(button)); //clear struct values


	//led config
	led.pGPIOx=GPIOD;
	led.pin_config.pin_number = 12;

	led.pin_config.pin_mode = OUTPUT;
	led.pin_config.pin_output_mode = OUTPUT_PUSHPULL;
	led.pin_config.pin_input_mode = NOPULL;
	led.pin_config.pin_speed = LOW;


	//push button config
	button.pGPIOx=GPIOA;
	button.pin_config.pin_number=0;
	button.pin_config.pin_mode=INTERRUPT_FT;
	button.pin_config.pin_speed=LOW;
	button.pin_config.pin_input_mode=NOPULL;


	GPIO_IRQConfig(EXTI0, ENABLE);
	GPIO_IRQPriority(EXTI0,EXTI_PRIORITY_6 ); //highest priority


	GPIO_Enable(GPIOD,ENABLE); //enable clock peripheral
	GPIO_Enable(GPIOA,ENABLE); //enable clock peripheral

	GPIO_init(&led);
	GPIO_init(&button);

	while(1)
	{
		if(test)
		{
		GPIO_toggle_pin(GPIOD, 12);
		}
	}

}

//override ISR function
void EXTI0_IRQHandler(){

	delay();
	IRQ_handling(0);
	test = test?0:1;
}
