/*****************************************************************************
* Copyright (C) 2021 by Bhargav Dharmendra Chauhan
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
*    File name   : sine.c
*    Description : Integer sine function source file
*    definitions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 04/19/2021
*
*****************************************************************************/

// including required libraries
#include "MKL25Z4.h"
#include "stdint.h"
#include "sine.h"

#define TRIG_TABLE_STEPS 		(32)
#define TRIG_TABLE_STEP_SIZE 	(HALF_PI/TRIG_TABLE_STEPS)

// sine lookup table
static const int16_t sin_lookup[TRIG_TABLE_STEPS + 1] = { 0, 100, 200, 299, 397,
		495, 591, 686, 780, 871, 960, 1047, 1132, 1214, 1292, 1368, 1440, 1509,
		1575, 1636, 1694, 1747, 1797, 1841, 1882, 1918, 1949, 1976, 1998, 2015,
		2027, 2035, 2037 };

// interpolation function to get an coordinate between two points
int32_t interpolate(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int32_t y = ((((x2 - x1) / 2) + ((x - x1) * (y2 - y1))) / (x2 - x1) + y1);
	return y;
}

// definition of the function in the header file
int32_t fp_sin(int32_t x)
{
	// sign variable
	int sign = 1;
	// other variables for calculation
	int32_t angle, lookup_index, x1, x2, y1, y2, y;

	// if x is negative update sign value
	if (x < 0) {
		x = -x;
		sign = -1;
	}
	// bring x in first quadrant by as it repeats after PI
	while (x > PI)
		x -= TWO_PI;

	// check again if it became 0 and update the sign
	if (x < 0) {
		x = -x;
		sign = sign * -1;
	}

	// wrap around Half Pi as we have lookup table till half pi
	if (x > HALF_PI) {
		x = PI - x;
	}

	// calculate lookup table index
	lookup_index = (x * TRIG_TABLE_STEPS) / HALF_PI;

	// calculate sign angle
	angle = lookup_index * TRIG_TABLE_STEP_SIZE;

	// if angle calculated is equal to x return corresponding value from lookup table
	if (angle == x)
		return sign * sin_lookup[lookup_index];

	// else calculate variables for interpolation
	x1 = angle;
	y1 = sin_lookup[lookup_index];
	x2 = (lookup_index + 1) * TRIG_TABLE_STEP_SIZE;
	y2 = sin_lookup[lookup_index + 1];

	// get the y coordinate using interpolation
	y = interpolate(x, x1, y1, x2, y2);

	// return y
	return sign * y;
}

