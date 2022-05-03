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
*    File name   : led.h
*    Description : Contains definitions of led functions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 05/02/2021
*
*****************************************************************************/

#ifndef LED_H_
#define LED_H_

/*****************************************************************************
* Initializes the registers, gpio, I/O and clock for the required led pin
*
*****************************************************************************/
void led_init();

/*****************************************************************************
* This function turns the corresponding LEDs ON and OFF as per the input
*
* Input Parameters:
*   red_on 			if 1, switches RED LED ON, else OFF
*   green_on 		if 1, switches GREEN LED ON, else OFF
*   blue_on 		if 1, switches BLUE LED ON, else OFF
*
*****************************************************************************/
void control_RGB_led(unsigned int red_on, unsigned int green_on, unsigned int blue_on);

#endif /* LED_H_ */
