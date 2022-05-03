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
 *    File name   : main.c
 *    Description : main file
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *
 *
 *****************************************************************************/

// including libraries
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <stdio.h>

#include "i2c.h"
#include "accelerometer.h"
#include "gpio_interrupt.h"
#include "sysclock.h"
#include "uart.h"
#include "led.h"
#include "audio_out.h"

// macros definition
#define NORMAL_DELAY_CONVERSION_VALUE (6000)
#define BUFFER_SIZE (1024)
#define TONE1 (200)
#define TONE2 (400)
#define TONE3 (600)
#define TONE4 (1000)
#define DELAY_100MS (100)
#define DELAY_30MS (30)
#define MAX_ANGLE_RANGE (180)


/*****************************************************************************
 * Introduces a delay for the input time by running loop for required
 * amount of iterations.
 *
 * Parameters:
 *   delay_time_ms      Delay time required in milli-seconds
 *
 *****************************************************************************/
// this function is not required to be in separate source file
// as it is a small function and only required in the main source file
void delay(int delay_time_ms) {
	int iterations = 0;
	//conversion of delay in millisecs to number of iterations
	iterations = NORMAL_DELAY_CONVERSION_VALUE * delay_time_ms;
	//do nothing till delay(iteration 0)
	while (iterations--) {
	}
}

/*****************************************************************************
 * Function which performs entire project functionality in defined flow
 *
 *****************************************************************************/
void tilt_measurement() {

	// local variables
	uint16_t dac_buffer[BUFFER_SIZE];
	// frequency array: add new frequencies here
	int frequency[] = { TONE1, TONE2, TONE3, TONE4 };
	// computing the size of the above array
	uint8_t number_of_frequencies = sizeof(frequency) / sizeof(frequency[0]);
	uint32_t samples = 0, i = 0;
	uint16_t target_angle = 0;
	uint8_t angle_flag = 0;
	int max_angle = 0;
	int roll_angle;


	// print uart commands
	printf("Set the reference angle:\n\r\t");
	printf("By adjusting the axis and pressing tactile switch\n\n\r");
	while (!reference_angle_flag)
		;
	printf("Reference angle is %d degree on roll_angle axis\n\r",
			reference_angle);
	// reduce the range
	max_angle = MAX_ANGLE_RANGE - reference_angle;

	// take input angle until valid input
	do {
		printf("\n\r");
		printf("Enter target angle using UART, Range: 0 to %d degree: ",
				max_angle);

		// store in variable
		target_angle = get_deci_input();

		// compare the input angle with correct range
		if (target_angle >= 0 && target_angle <= max_angle) {
			angle_flag = 0;

		} else {
			printf("\n\r\t***INVALID ANGLE***\n\r");
			angle_flag = 1;
		}
	} while (angle_flag);

	// print the target angle
	printf("Target Angle selected: %d\n\r", target_angle);

	// infinite loop to measure the angle continuously
	while (1) {

		// call roll measurement function
		roll_angle = read_roll_angle();

		printf("Roll angle from reference is %d degree\n\r",
				roll_angle - reference_angle);

		// if target angle reached
		if (target_angle == (roll_angle - reference_angle)) {

			// green light
			control_RGB_led(0, 1, 0);

			// play tone on buzzer
			while (i < number_of_frequencies) {
				// generate the samples of provided frequency and store into the buffer
				samples = tone_to_samples(frequency[i], dac_buffer, BUFFER_SIZE);
				// generate the dma buffer for DMA transfer
				generate_dma_buffer(dac_buffer, samples);
				// start the dma transfer
				start_DMA0_transfer();

				delay(DELAY_30MS);
				// increment i or wrap around the loop
				i++;

			}

		} else {
			// stop the buzzer
			TPM0->SC &= ~TPM_SC_CMOD_MASK;
			// red light
			control_RGB_led(1, 0, 0);
		}
		delay(DELAY_100MS);

	}

}

// main function
int main(void) {

	// initializing board hardware and debig console
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL

	BOARD_InitDebugConsole();
#endif


	// initialize elements
	sysclock_init();
	Init_Uart0(BAUD_RATE);

	// printing welcome message
	printf("***** Angle Detection using Accelerometer *****\n\n\r");

	// initialize and test elements
	init_gpio_interrupt();
	led_init();
	init_DAC0();
	init_TPM0();
	init_DMA0();
	i2c_init();
	i2c_test();

	// checking if mma initialized properly
	if (!init_mma()) {
		PRINTF("Accelerometer NOT Initialized\n\r");
		while (1)
			;
	}
	printf("Accelerometer Initialized\n\r");

	// measure the tilt
	tilt_measurement();

	return 0;
}

