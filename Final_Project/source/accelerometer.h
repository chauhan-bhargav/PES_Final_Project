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
 *    File name   : accelerometer.h
 *    Description : accelerometer functions and macros definition
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *
 *	  References: https://github.com/alexander-g-dean/ESF/blob/master/NXP
 *
 *****************************************************************************/

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


#ifndef MMA8451_H
#define MMA8451_H
#include <stdint.h>


// defining macros required for MMA initialization
#define MMA_ADDR 	(0x3A)		// mma sensor address
#define REG_XHI 	(0x01)		// xhi register address
#define REG_CTRL1  	(0x2A)		// CTRL1 register address
#define REG_WHOAMI 	(0x0D)		// who am i register for testing
#define WHOAMI 		(0x1A)

#define PI (3.14159265)		// pi value

// function declarations

/*****************************************************************************
 * Initializes the MMA sensor of KL25Z board
 *
 *****************************************************************************/
int init_mma(void);

/*****************************************************************************
 * Reads and returns the roll angle using trigonometry calculations
 *
 *****************************************************************************/
int read_roll_angle(void);

/*****************************************************************************
 * Tests I2C communication to MMA sensor
 *
 *****************************************************************************/
void i2c_test();

#endif

#endif /* ACCELEROMETER_H_ */
