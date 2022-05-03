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
*    File name   : cbfifo.c
*    Description : Circular buffer C file, includes functions
*                  related to creating and implementing the Queue using
*                  circular buffer.
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 04/05/2021
*
*    References:
*    https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
*
*****************************************************************************/

//including the standard functions
#include <stdint.h>
#include "MKL25Z4.h"
//including the header file
#include "cbfifo.h"

// function description given in cbfifo.h file
void cbfifo_init(cbfifo *cbfifo)
{
	cbfifo->head = 0;
	cbfifo->tail = 0;
	cbfifo->length = 0;
	for (unsigned int i=0; i < BUFFER_SIZE; i++)
	    cbfifo->buffer[i] = '0';
}

// function description given in cbfifo.h file
size_t cbfifo_enqueue(void *buf, size_t nbyte, cbfifo *cbfifo)
{
    //variable to keep track on bytes read
    int bytesWritten = 0;
    int bytes = nbyte;
    uint32_t interrupt_mask;

    // disabling the interrupt
	interrupt_mask = __get_PRIMASK();
	__disable_irq();

    //if bytes are legal
    if( bytes>=0 )
    {
        // buffer is not full then add data
        if( cbfifo->length != BUFFER_SIZE)
        {

            //making uint8 pointer so the we can
            //read and write single byte of data
            //typecasting the input data to uint8
            uint8_t *bufferRead = (uint8_t *) buf;

            //loop to read required number of bytes
            for(int i = 0; i< bytes ; i++)
            {
                //add data to head
                cbfifo->buffer[cbfifo->head] = *bufferRead;
                //move head ahead
                //below statement is performing circular increment
                cbfifo->head = (cbfifo->head + 1) % BUFFER_SIZE;
                //increase different counters
                bytesWritten++;
                bufferRead++;
                cbfifo->length++;
                // if we reach tail then the buffer is full
                if(cbfifo->head == cbfifo->tail)
                {
                    break;
                }
            }
        }
        // buffer is full
        else
        {
            return bytesWritten;
        }

    }
    //illegal input data
    else
    {
        return (size_t)-1;
    }
    // enabling the interrupt
    __set_PRIMASK(interrupt_mask);
    return bytesWritten;
}

// function description given in cbfifo.h file
size_t cbfifo_dequeue(void *buf, size_t nbyte, cbfifo *cbfifo)
{
    //checking illegal nbyte
    if((int)nbyte < 0)
        return -1;

    //creating a uint8 pointer to point to the input data
    uint8_t *bufferWrite = (uint8_t *) buf;
    int bytesWritten = 0;
    uint32_t interrupt_mask;

    //buffer empty condition
    if(cbfifo->length == 0 )
    {
        return 0;
    }
    //buffer is not empty
    else
    {
    	// disabling the interrupt
    	interrupt_mask = __get_PRIMASK();
    	__disable_irq();
        //write the required amount of bytes
        for(int i = 0; i< nbyte ; i++)
        {
            //pointer to the tail data
            *bufferWrite = cbfifo->buffer[cbfifo->tail];
            //increment the counters
            bufferWrite++;
            bytesWritten++;
            cbfifo->length--;
            //circular increment the tail
            cbfifo->tail = (cbfifo->tail + 1) % BUFFER_SIZE;
            //if tail reaches the head then the buffer is empty
            //no more data to dequeue
            if(cbfifo->head == cbfifo->tail)
            {
                break;
            }
        }
        // enabling the interrupt
        __set_PRIMASK(interrupt_mask);
    }

    return bytesWritten;
}

// function description given in cbfifo.h file
size_t cbfifo_capacity(cbfifo *cbfifo)
{
    //returns the buffer capacity
    return BUFFER_SIZE;
}

// function description given in cbfifo.h file
size_t cbfifo_length(cbfifo *cbfifo)
{
    //returns the current length of circular buffer
    return (cbfifo->length);

}



