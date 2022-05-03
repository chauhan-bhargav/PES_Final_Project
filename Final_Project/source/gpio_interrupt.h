/*****************************************************************************
 * Copyright (C) 2022 by Bhargav Dharmendra Chauhan
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Bhargav Dharmendra Chauhan and the University of Colorado
 * are not liable for any misuse of this material.
 *
 *****************************************************************************/
/*****************************************************************************
 *
 *    File name   : gpio_interrupt.h
 *    Description : gpio interrupt function definition
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *
 *****************************************************************************/

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

// declaring global extern variables
extern int reference_angle, reference_angle_flag;

//function definitions

/*****************************************************************************
 * PortA IRQ handler to handle external interrupts on port A gpio pin
 *
 *****************************************************************************/
void PORTA_IRQHandler();

/*****************************************************************************
 * Initializes the port A GPIO interrupt
 *
 *****************************************************************************/
void init_gpio_interrupt();

#endif /* GPIO_INTERRUPT_H_ */
