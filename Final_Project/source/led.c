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
*    File name   : led.c
*    Description : Contains implementation of led functions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 05/02/2021
*
*****************************************************************************/

// including required libraries
#include <MKL25Z4.h>
#include <stdint.h>
#include <stdio.h>

//defining macros
#define MASK(x) (1UL << (x))

//macros for red led
#define RED_LED_PIN 18
#define RED_GPIO_PORT GPIOB
#define RED_SCGC5_MASK SIM_SCGC5_PORTB_MASK
#define RED_LED_PCR PORTB->PCR[RED_LED_PIN]

//macros for green led
#define GREEN_LED_PIN 19
#define GREEN_GPIO_PORT GPIOB
#define GREEN_SCGC5_MASK SIM_SCGC5_PORTB_MASK
#define GREEN_LED_PCR PORTB->PCR[GREEN_LED_PIN]

//macros for blue led
#define BLUE_LED_PIN 1
#define BLUE_GPIO_PORT GPIOD
#define BLUE_SCGC5_MASK SIM_SCGC5_PORTD_MASK
#define BLUE_LED_PCR PORTD->PCR[BLUE_LED_PIN]


void led_init()
{
	//Set the clock gating register for red led pin
	SIM->SCGC5 |= RED_SCGC5_MASK;
	//control multiplexing via port control register for red led pin
	RED_LED_PCR &= ~PORT_PCR_MUX_MASK;
	RED_LED_PCR |= PORT_PCR_MUX(1);
	//set the gpio pin as output
	RED_GPIO_PORT->PDDR |= MASK(RED_LED_PIN);
	//make the red led pin output low
	RED_GPIO_PORT->PSOR |= MASK(RED_LED_PIN);

	//Set the clock gating register for green led pin
	SIM->SCGC5 |= GREEN_SCGC5_MASK;
	//control multiplexing via port control register for blue led pin
	GREEN_LED_PCR &= ~PORT_PCR_MUX_MASK;
	GREEN_LED_PCR |= PORT_PCR_MUX(1);
	//set the gpio pin as output
	GREEN_GPIO_PORT->PDDR |= MASK(GREEN_LED_PIN);
	//make the green led pin output low
	GREEN_GPIO_PORT->PSOR |= MASK(GREEN_LED_PIN);

	//Set the clock gating register for blue led pin
	SIM->SCGC5 |= BLUE_SCGC5_MASK;
	//control multiplexing via port control register for blue led pin
	BLUE_LED_PCR &= ~PORT_PCR_MUX_MASK;
	BLUE_LED_PCR |= PORT_PCR_MUX(1);
	//set the gpio pin as output
	BLUE_GPIO_PORT->PDDR |= MASK(BLUE_LED_PIN);
	//make the blue led pin output low
	BLUE_GPIO_PORT->PSOR |= MASK(BLUE_LED_PIN);

	printf("LEDs Initialized Successfully\n\r");
}


void control_RGB_led(unsigned int red_on, unsigned int green_on, unsigned int blue_on)
{
	//condition to check for red LED
	if (red_on)
	{
		//setting the port clear output register to ON the led
		RED_GPIO_PORT->PCOR |= MASK(RED_LED_PIN);
	}
	else
	{
		//setting the port set output register to OFF the led
		RED_GPIO_PORT->PSOR |= MASK(RED_LED_PIN);
	}

	//condition to check for green LED
	if (green_on)
	{
		//setting the port clear output register to ON the led
		GREEN_GPIO_PORT->PCOR |= MASK(GREEN_LED_PIN);
	}
	else
	{
		//setting the port set output register to OFF the led
		GREEN_GPIO_PORT->PSOR |= MASK(GREEN_LED_PIN);
	}

	//condition to check for blue LED
	if (blue_on)
	{
		//setting the port clear output register to ON the led
		BLUE_GPIO_PORT->PCOR |= MASK(BLUE_LED_PIN);
	}
	else
	{
		//setting the port set output register to OFF the led
		BLUE_GPIO_PORT->PSOR |= MASK(BLUE_LED_PIN);
	}
}


