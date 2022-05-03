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
*    File name   : uart.h
*    Description : UART header file containing the function definitions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 04/05/2021
*
*
*****************************************************************************/

#ifndef UART_H_
#define UART_H_

//including required libs
#include <stdint.h>

//defining the BAUD RATE for uart communication
#define BAUD_RATE (38400)

/*****************************************************************************
* Initializes the UART0 module of KL25Z for the provided baud rate
*
* Parameters:
*   baud_rate      			Baud rate of the communication
*
*****************************************************************************/
void Init_Uart0(uint32_t baud_rate);

/*****************************************************************************
* This function is the UART0 interrupt request handler function
*	When the UART interrupt occurs, it performs checks for error, transmit
*	or receive functionalities
*
*****************************************************************************/
void UART0_IRQHandler(void);

/*****************************************************************************
* This function ties the inbuild reading functions such as getchar(),
* scanf() etc to the UART
*
*****************************************************************************/
int __sys_readc(void);

/*****************************************************************************
* This function ties the inbuild writing functions such as putchar(),
* printf() etc to the UART
*
*****************************************************************************/
int __sys_write(int handle, char *buf, int size);

/*****************************************************************************
* This function takes user input from the uart continuously and calls
* respective function according to the commands
*
*****************************************************************************/
void read_uart_input();

uint16_t get_deci_input();

#endif /* UART_H_ */
