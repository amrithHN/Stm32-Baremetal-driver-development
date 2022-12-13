/*
 * LED_test.h
 *
 *  Created on: Dec 13, 2022
 *      Author: amrith
 */

#ifndef LED_TEST_H_
#define LED_TEST_H_

#include "stm32f407xx_gpio.h"

/*
 * Toggle onboard leds and using push button to turn on/off
 *
 */

//output led with gpio handle for init
void led_init(GPIO_handle_t*);


// toggle leds onboard
void led_toggle(GPIO_Reg_Def* pGPIOx,uint8_t pin);


//push buutton init
void sw_init(GPIO_handle_t*);

// read push button
uint8_t sw_read(GPIO_Reg_Def* pGPIOx,uint8_t pin);

void led_test(void);






#endif /* LED_TEST_H_ */
