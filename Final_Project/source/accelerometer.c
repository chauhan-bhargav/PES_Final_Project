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
 *    File name   : accelerometer.c
 *    Description : accelerometer functions
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *	  References: https://github.com/alexander-g-dean/ESF/blob/master/NXP
 *
 *****************************************************************************/

// including libraries
#include <MKL25Z4.H>
#include "accelerometer.h"
#include "i2c.h"
#include <math.h>
#include "fsl_debug_console.h"
#include <stdio.h>


// macros
#define ANGLE_RADIANS (180)
#define LEFT_SHIFT_8 (8)

// initializes mma8451 sensor
int init_mma()
{
	// set active mode, 14 bit samples and 800 Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	printf("MMA Initialized\r\n");
	return 1;
}

void i2c_test()
{
	if(i2c_read_byte(MMA_ADDR, REG_WHOAMI) == WHOAMI)
	{
		printf("I2C Testing Done\r\n");

	}
}

// reads acceleorometer values and measure the roll angle
int read_roll_angle()
{
	// initializing variables
	int i, y_value, z_value;
	int roll_angle;
	uint8_t data_arr[6];
	int16_t temp_arr[3];

	// calling i2c
	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data_arr[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data_arr[i] = i2c_repeated_read(1);

	// extracing 16 bits of data and storing in temp array
	for ( i=0 ; i<3 ; i++ ) {
		temp_arr[i] = (int16_t)((data_arr[2*i] << LEFT_SHIFT_8) | data_arr[2*i+1]);
	}

	// Align for 14 bits
	y_value = temp_arr[0]/4;
	z_value = temp_arr[2]/4;

	// roll angle measurement using inverse tan
	roll_angle = atan2(y_value, z_value) * ANGLE_RADIANS /PI;

	// return the roll angle
	return roll_angle;
}

