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
 * <NVIC : cotex m4 PROGRAMMING MANUAL page 210,208 >
 *
 */

#define  NVIC_ISER0 ((volatile uint32_t*)0xE000E100)
#define  NVIC_ISER1 ((volatile uint32_t*)0xE000E132)
#define  NVIC_ISER2 ((volatile uint32_t*)0xE000E164)
#define  NVIC_ISER3 ((volatile uint32_t*)0xE000E196)
#define  NVIC_ISER4 ((volatile uint32_t*)0xE000E1C8)


#define  NVIC_ICER0 ((volatile uint32_t*)0XE000E180)
#define  NVIC_ICER1 ((volatile uint32_t*)0XE000E1B1)
#define  NVIC_ICER2 ((volatile uint32_t*)0XE000E1E4)
#define  NVIC_ICER3 ((volatile uint32_t*)0XE000E216)
#define  NVIC_ICER4 ((volatile uint32_t*)0XE000E248)

#define NVIC_IPR_BASE ((volatile uint32_t*)0xE000E400)
#define NVIC_IPRx(x) ((volatile uint32_t*)NVIC_IPR_BASE+x)


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
#define INTERRUPT_FT   04
#define INTERRUPT_RT   05
#define INTERRUPT_FRT   06

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

#define PORT_NUM_MAP(x) ((x == GPIOA)?0: \
						 (x == GPIOB)?1: \
						 (x == GPIOC)?2: \
						 (x == GPIOD)?3: \
						 (x == GPIOE)?4:0)


/*
 * <EXTI IRQ number refer Interrupt table for priority and number page :372 >
 *
 */

#define EXTI0 6
#define EXTI1 7
#define EXTI2 8
#define EXTI3 9
#define EXTI4 6
#define EXTI9_5 23
#define EXTI15_10 40

/*
 * <Priority for any IRQ is in the range of 0-15 as its a 4 bit value>
 *
 */

#define EXTI_PRIORITY_0 0
#define EXTI_PRIORITY_1 1
#define EXTI_PRIORITY_2 2
#define EXTI_PRIORITY_3 3
#define EXTI_PRIORITY_4 4
#define EXTI_PRIORITY_5 5
#define EXTI_PRIORITY_6 6

#define EXTI_PRIORITY_15 15

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

void GPIO_IRQConfig(uint8_t IRQnumber,uint8_t flag);
void GPIO_IRQPriority(uint8_t IRQ_number,uint8_t IRQ_priority);
void IRQ_handling(uint8_t pin);


#endif /* INC_STM32F407XX_GPIO_H_ */
