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
*    File name   : uart.c
*    Description : UART source file containing the function implementations
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 05/02/2021
*
*    References:
*    https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo/src
*    Prof. Howdy lecture slides and videos
*
*****************************************************************************/
//including the required libraries
#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "cbfifo.h"

//defining the macros for different constant data
#define UART_OVERSAMPLE_RATE (16)	// defining the oversample rate
#define SYS_CLOCK (24e6)			// defining system clock rate(24Mhz)
#define PARITY (0)					// defining parity as 0 -> NONE
#define DATA_MODE (0)				// 0 -> 8 bits mode, 1 -> 9 bits mode
#define STOP_BITS (1)				// Number of stop bits, 0 -> 1 stop bit, 1 -> 2 stop bits
#define CARRIAGE_RETURN (13)		// Carriage return decimal value according to ascii table
#define BACKSPACE (8)				// Backspace key decimal value according to ascii table
#define SPACE (32)					// Space key decimal value according to ascii table
#define TAB (9)						// Tab key decimal value according to ascii table
#define MAX_DIGIT ('9')				// Max digit represented in char
#define MIN_DIGIT ('0')				// Min digit represented in char
#define DECIMAL_CONVERSION (10)		// multiplication factor to convert into decimal

//creating an instance of transmit and receive buffer
cbfifo receive_cbfifo, transmit_cbfifo;

//function definition in uart.h file
void Init_Uart0(uint32_t baud_rate) {

	//defining a variable for baud rate calculation
	uint16_t sbr;

	// Enable clock gating for UARTO and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before initialization
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set UART clock to 24 MHz clock by selecting the FLL clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UARTO Rx and Ix
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); //RX
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); //TX

	// Set baud rate and oversampling ratio
	sbr = (uint16_t) ((SYS_CLOCK) / (baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active  edge and IIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(
			0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_MODE) | UART0_C1_PE(PARITY);

	// Don't invert transmit data, do enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1)
	| UART0_C3_FEIE(1) | UART0_C3_PEIE(1);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(
			1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	//initializing the cbfifo for receive and transmit
	cbfifo_init(&receive_cbfifo);
	cbfifo_init(&transmit_cbfifo);

	// Enable interrupts
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	// Enable UART transmitter and receiver
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

//function definition in uart.h file
void UART0_IRQHandler(void) {
	//define a variable to store transmit and receive character
	char character;

	//check if any of the error flags are set
	if (UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK |
	UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		//if any of the flag is set reset it by writing 1
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
		UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		//read the data register to reset the RDRF flag
		character = UART0->D;
	}
	// check if character is received and caused interrupt
	// this is done by checking thr receive data register flag
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		//store the recieved character to the variable
		character = UART0->D;

		//check if the receive fifo is full
		if (!(cbfifo_length(&receive_cbfifo) == cbfifo_capacity(&receive_cbfifo))) {
			//if not full enqueue the received character to the cbfifo
			cbfifo_enqueue(&character, 1, &receive_cbfifo);
		}
		// else queue is full and the received character is discarded
		else {

		}
	}

	// enable the transmit interrupt and check if transmit buffer is empty
	// isr is triggered for transmission for this condition
	if ((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK)) {
		// if the above condition is true, then character is ready to transmit
		// check if the cbfifo is not empty
		// then dequeue the character from the cbfifo and put into the data register
		if (cbfifo_dequeue(&character, 1, &transmit_cbfifo) == 1) {
			UART0->D = character;
		}
		// else disable the transmit interrupt
		else {
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

//function definition in uart.h file
int __sys_write(int handle, char *buf, int size)
{
	// wait while the transmit buffer is full
	while (cbfifo_length(&transmit_cbfifo) == cbfifo_capacity(&transmit_cbfifo))
		;
	// if enqueued the requested bytes set the transmit interrupt and send data
	if (cbfifo_enqueue(buf, size, &transmit_cbfifo) == size)
	{
		if (!(UART0->C2 & UART0_C2_TIE_MASK))
		{
			UART0->C2 |= UART0_C2_TIE(1);
		}
	}
	// error condition data unable to write
	else
	{
		return -1;
	}
	return 0;

}

//function definition in uart.h file
int __sys_readc(void)
{
	// declare a variable to store the received character
	uint8_t character;
	// while the receive fifo is empty wait
	while (cbfifo_length(&receive_cbfifo) == 0)
		;
	//if 1 character dequeued then return the character
	if (cbfifo_dequeue(&character, 1, &receive_cbfifo) == 1)
	{
		return character;
	}
	else
	{
		return -1;
	}
}

// function definition in header file
uint16_t get_deci_input()
{

    //initialize the variables for digit store
    uint8_t digit = 0;
    uint8_t digit_store[10];
    int counter = 0;
    uint16_t number = 0;

    //while digit is not 13 (carriage return)
    while(digit != CARRIAGE_RETURN)
    {
        //store the character
        digit=getchar();
        //check if it is digit and store the digit into the array in decimal form
        if((digit >= MIN_DIGIT) && (digit <= MAX_DIGIT) && counter >= 0)
        {
            putchar(digit);
            digit_store[counter] = digit - MIN_DIGIT;
            counter++;
        }
        //check if user entered backspave
        else if(digit == BACKSPACE && counter > 0) //check for backspace
        {
            putchar(BACKSPACE);     //print backspace
            putchar(SPACE);         //print space
            putchar(BACKSPACE);     //print backspace
            counter--;
        }
    }
    printf("\n\r");
    //calculated the number from the array
    for(int buffer_number=0; buffer_number < counter; buffer_number++)
    {
        number*= DECIMAL_CONVERSION;
        number+= digit_store[buffer_number];
    }
    //return the number
    return number;
}

