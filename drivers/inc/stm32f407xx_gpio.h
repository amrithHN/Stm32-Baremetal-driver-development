/*
 * stm32f4xx_gpio.h
 *
 *  Created on: Dec 12, 2022
 *      Author: amrith
 */

#ifndef INC_STM32F407XX_GPIO_H_
#define INC_STM32F407XX_GPIO_H_

#include "stm32f407xx.h"


/*
 * <Each GPIO needs port and pin configured to work , enable it before use in API>
 *
 */



typedef struct{
	uint8_t pin_number;
	uint8_t pin_mode;
	uint8_t pin_input_mode;
	uint8_t pin_output_mode;
	uint8_t pin_speed;
	uint8_t pin_alternate_function;
}GPIO_PIN_config;

typedef struct{
	GPIO_Reg_Def* pGPIOx;
	GPIO_PIN_config pin_config;
}GPIO_handle_t;


/*
 * <Each GPIO pin has these functions/registers to control>
 *-- each pin reaquires , portname , pinnumber , type (input /output/AF /Analog ) , pullup or pulldown (for input) , output mode (opendrain or pushpull), speed , AFmode
 */

#define INPUT    00
#define OUTPUT   01
#define ALT_FUNC 02
#define ANALOG   03

#define NOPULL   00
#define PULLUP   01
#define PULLDOWN 02

#define LOW 	00
#define MEDIUM 	01
#define HIGH 	02
#define VHIGH 	03

#define OUTPUT_PUSHPULL 	00
#define OUTPUT_OPENDRAIN 	01

#define GPIOA_RESET() if(1){ RCC->AHB1_RSTR |= 1<<0; RCC->AHB1_RSTR &= ~(1<<0);  }
#define GPIOB_RESET() if(1){ RCC->AHB1_RSTR |= 1<<1; RCC->AHB1_RSTR &= ~(1<<1);  }
#define GPIOC_RESET() if(1){ RCC->AHB1_RSTR |= 1<<2; RCC->AHB1_RSTR &= ~(1<<2);  }
#define GPIOD_RESET() if(1){ RCC->AHB1_RSTR |= 1<<3; RCC->AHB1_RSTR &= ~(1<<3);  }
#define GPIOE_RESET() if(1){ RCC->AHB1_RSTR |= 1<<4; RCC->AHB1_RSTR &= ~(1<<4);  }


/*
 * <GPIO APIs to control them>
 *
 */


/*
 *  GPIO_init(): Initialisation of GPIO pin based on the config handle
 */
void GPIO_init(GPIO_handle_t*);

/*
 *  GPIO_RESET(): RESET GPIO port
 */
void GPIO_RESET(GPIO_Reg_Def*);

/*
 *  GPIO_Enable(): enables or disables the GPIO using the bus clock
 */
void GPIO_Enable(GPIO_Reg_Def*,uint8_t flag);

/*
 *  GPIO_read_pin(): reads the pin when configured as input
 */
uint8_t GPIO_read_pin(GPIO_Reg_Def*,uint8_t pin);

/*
 *  GPIO_read_port(): reads the port(16 bit return value) when configured as input
 */
uint16_t GPIO_read_port(GPIO_Reg_Def*);

/*
 *  GPIO_write_pin(): write to pin when configured in output mode
 */
void GPIO_write_pin(GPIO_Reg_Def*,uint8_t pin,uint8_t value);

/*
 *  GPIO_write_port(): write to port (16 bit value) when configured in output mode
 */
void GPIO_write_port(GPIO_Reg_Def*,uint16_t value);

/*
 *  GPIO_toggle_pin(): toggles output state
 */
void GPIO_toggle_pin(GPIO_Reg_Def*,uint8_t pin);



/*
 *  IRQ stuff
 */


#endif /* INC_STM32F407XX_GPIO_H_ */
