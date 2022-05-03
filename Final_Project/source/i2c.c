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
 *    File name   : i2c.c
 *    Description : i2c functions implementation
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *    References: https://github.com/alexander-g-dean/ESF/blob/master/NXP
 *
 *****************************************************************************/

// including require libraries
#include "MKL25Z4.h"
#include "i2c.h"

// declaring global variables
int lock_detect = 0;
int lock_i2c = 0;

// function definition in header file
void i2c_init(void)
{
	// enable clock gating for i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	// set I2C pins
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	// baud set to 100k
	// baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64

 	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

// function definition in header file
void i2c_busy(void){


	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;

	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK;  	//set MASTER mode
	I2C0->C1 |= I2C_C1_TX_MASK; 	// Set transmit (TX) mode
	I2C0->D = 0xFF;

	// wait for interrupt
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	}
	// clear interrupt bit
	I2C0->S |= I2C_S_IICIF_MASK;


	// Clear arbitration error flag
	I2C0->S |= I2C_S_ARBL_MASK;


	// Send start,  Set transmit (TX) mode, START signal generated
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	// Wait until start is send
	I2C0->C1 |= I2C_C1_IICEN_MASK;


	// send stop, set slave mode
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	// set receiver
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;

	// wait
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	// Clear arbitration error & interrupt flag
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	lock_i2c=1;
}

// function definition in header file
void i2c_wait(void) {

	lock_detect = 0;

	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}

	if (lock_detect >= 200)
		i2c_busy();

	I2C0->S |= I2C_S_IICIF_MASK;
}

// function definition in header file
void i2c_start()
{
	// set to transmit mode
	I2C_TRAN;
	// send start signal
	I2C_M_START;
}

// function definition in header file
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	//send dev address
	I2C0->D = dev;
	//wait for completion
	I2C_WAIT

	//send read address
	I2C0->D =  address;
	//wait for completion
	I2C_WAIT

	//repeated start
	I2C_M_RSTART;
	//send dev address (read)
	I2C0->D = (dev|0x1);
	//wait for completion
	I2C_WAIT
	//set to receive mode
	I2C_REC;

}

// function definition in header file
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	// initialize variable
	uint8_t data;
	lock_detect = 0;

	// set NACK after read
	if(isLastRead)
	{
		NACK;
	}
	// ACK after read
	else
	{
		ACK;
	}
	// dummy read
	data = I2C0->D;
	// wait for completion
	I2C_WAIT;

	// send stop
	if(isLastRead)	{
		I2C_M_STOP;
	}
	// read data
	data = I2C0->D;

	// return data
	return  data;
}


// function definition in header file
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	//set to transmit mode
	I2C_TRAN;
	//send start
	I2C_M_START;
	//send dev address
	I2C0->D = dev;
	//wait for completion
	I2C_WAIT
	//send read address
	I2C0->D =  address;
	// wait for completion
	I2C_WAIT
	//repeated start
	I2C_M_RSTART;
	//send dev address (read)
	I2C0->D = (dev|0x1);
	//wait for completion
	I2C_WAIT

	//set to receive mode
	I2C_REC;
	//set NACK after read
	NACK;

	//dummy read
	data = I2C0->D;
	//wait for completion
	I2C_WAIT

	//send stop
	I2C_M_STOP;
	//read data
	data = I2C0->D;
	//return data
	return data;
}

// function definition in header file
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	//set to transmit mode
	I2C_TRAN;
	//send start
	I2C_M_START;
	//send dev address
	I2C0->D = dev;
	//wait for ack
	I2C_WAIT

	//send write address
	I2C0->D =  address;
	//wait for completion
	I2C_WAIT

	//send data
	I2C0->D = data;
	//wait for completion
	I2C_WAIT
	// send stop
	I2C_M_STOP;

}
