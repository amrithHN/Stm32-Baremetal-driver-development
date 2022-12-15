/*
 * button_interrupt.h
 *
 *  Created on: Dec 15, 2022
 *      Author: amrith
 */

#ifndef BUTTON_INTERRUPT_H_
#define BUTTON_INTERRUPT_H_


#include "stm32f407xx_gpio.h"

/*
 * Toggle onboard leds and using push button to turn on/off
 *
 */

//output led with gpio handle for init
void led_init(GPIO_handle_t*);


// toggle leds onboard
void led_toggle(GPIO_Reg_Def* pGPIOx,uint8_t pin);


//push button init
void sw_init(GPIO_handle_t*);

// read push button
uint8_t sw_read(GPIO_Reg_Def* pGPIOx,uint8_t pin);

void led_test(void);








#endif /* BUTTON_INTERRUPT_H_ */
