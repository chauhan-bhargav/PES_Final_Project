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
*    File name   : sine.h
*    Description : sine function header file
*    definitions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 04/19/2021
*
*****************************************************************************/

#ifndef SINE_H_
#define SINE_H_

// defining macros
#define TRIG_SCALE_FACTOR 		(2037)
#define HALF_PI            		(3200)
#define PI              		(6399)
#define TWO_PI          		(12799)

/*****************************************************************************
* Performs interpolation and generate new data point based on the range
* of a discrete set of known data points
*
* Parameters:
*   x,x1,x2,y1,y2		known data points on a coordinate system
*   y					output value calculated using interpolation
*
*
*****************************************************************************/
int32_t interpolate(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/*****************************************************************************
* Performs the sine trigonometry using only integers used to drive the DAC
* output
*
* Parameters:
*   x			value whose sine is to be calculated
*
*****************************************************************************/
int32_t fp_sin(int32_t x);

#endif /* SINE_H_ */
