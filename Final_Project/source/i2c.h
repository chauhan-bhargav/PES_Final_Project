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
 *    File name   : i2c.h
 *    Description : i2c functions definition
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 05/01/2021
 *    References: https://github.com/alexander-g-dean/ESF/blob/master/NXP
 *
 *****************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

// i2c macros definition for sending communiation signals such as start, stop ack

#define I2C_M_START 		I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  		I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 		I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    	while(I2C0->S & 0x01)
#define TRANS_COMP			while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        	I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           		I2C0->C1 &= ~I2C_C1_TXAK_MASK

/*****************************************************************************
 * Iinitializes the I2C communication via KL25z
 *
 *****************************************************************************/
void i2c_init(void);

/*****************************************************************************
 * Sends I2C start signal and starts i2c communication
 *
 *****************************************************************************/
void i2c_start(void);

/*****************************************************************************
 * I2c read setup for reading the data
 * Parameters:
 *   dev      	device address
 *   address    read address
 *****************************************************************************/
void i2c_read_setup(uint8_t dev, uint8_t address);

/*****************************************************************************
 * Reads repeatedly from the slave device
 * Parameters:
 *   isLastRead      	flag variable for last read
 *****************************************************************************/
uint8_t i2c_repeated_read(uint8_t isLastRead);

/*****************************************************************************
 * Reads a byte from the slave device via i2c
 * Parameters:
 *   dev      	device address
 *   address    read address
 *****************************************************************************/
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/*****************************************************************************
 * Writes a byte to the slave device via i2c
 * Parameters:
 *   dev      	device address
 *   address    read address
 *   data		data to write
 *****************************************************************************/
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
