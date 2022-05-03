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
 *    File name   : gpio_interrupt.c
 *    Description : gpio interrupt function declaration
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *
 *****************************************************************************/


// including required libraries
#include <MKL25Z4.h>
#include <stdint.h>
#include "fsl_debug_console.h"
#include <stdio.h>

#include "accelerometer.h"
#include "gpio_interrupt.h"

// defining macros
#define DEBOUNCE_TIME (10000)
#define RISING_EDGE_INTERRUPT (9)

// initalizing global variables
int reference_angle = 0, reference_angle_flag = 0;

// function declaration in header file
void init_gpio_interrupt()
{
	// enable clock gating to port A
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// initialize the pin control register for tactile switch gpio
	PORTA->PCR[13] = PORT_PCR_MUX(1)						|		// setting type as gpio
					PORT_PCR_IRQC(RISING_EDGE_INTERRUPT)	;		// interrupt on rising edge

	// Clear current interrupt and enable interrupt
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);

	// print status
	printf("GPIO Interrupt Enabled\n\r");
}

// function declaration in header file
void PORTA_IRQHandler()
{
	// clear the interrupt status flag and disable interrupt
	PORTA->PCR[13] |= PORT_PCR_ISF(1);
	NVIC_DisableIRQ(PORTA_IRQn);

	// read the start angle from user angle
	reference_angle = read_roll_angle();

	// set reference angle flag
	reference_angle_flag = 1;

	// software de-bouncing for input switch
	for(int i = 0; i < DEBOUNCE_TIME; i++)
	{
		__asm volatile("NOP");
	}

}
