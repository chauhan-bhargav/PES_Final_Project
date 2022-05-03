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
 *    File name   : cbfifo.h
 *    Description : Contains functions definitions of circular buffer
 *
 *    Author: Bhargav Dharmendra Chauhan
 *    Tools : MCUXpresso
 *    Date  : 04/05/2021
 *
 *****************************************************************************/

#ifndef _CBFIFO_H_
#define _CBFIFO_H_

//including required libraries
#include <stdlib.h>  // for size_t

//defining the buffer size statically
#define BUFFER_SIZE (256)

//creating a structure to store the circular buffer parameters like head, tail etc.
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int length;
}cbfifo;


/*****************************************************************************
* Circular buffer initialization function which initializes the passed cbfifo
* instance
*
* Parameters:
*   cbfifo *cbfifo      cbfifo instance
*
*****************************************************************************/
void cbfifo_init(cbfifo *cbfifo);

/*****************************************************************************
* Enqueues data onto the FIFO, up to the limit of the available FIFO
* capacity.
*
* Parameters:
*   buf      			Pointer to the data
*   nbyte    			Max number of bytes to enqueue
*	cbfifo *cbfifo		cbfifo instance
* Returns:
*   The number of bytes actually enqueued, which could be 0. In case
*   of an error, returns (size_t) -1.
*
*****************************************************************************/
size_t cbfifo_enqueue(void *buf, size_t nbyte, cbfifo *cbfifo);

/*****************************************************************************

* Attempts to remove ("dequeue") up to nbyte bytes of data from the passed
* FIFO. Removed data will be copied into the buffer pointed to by buf.
*
* Parameters:
*   buf      			Destination for the dequeued data
*   nbyte    			Bytes of data requested
*   cbfifo *cbfifo		cbfifo instance
*
* Returns:
*   The number of bytes actually copied, which will be between 0 and
*   nbyte.
*
* To further explain the behavior: If the FIFO's current length is 24
* bytes, and the caller requests 30 bytes, cbfifo_dequeue should
* return the 24 bytes it has, and the new FIFO length will be 0. If
* the FIFO is empty (current length is 0 bytes), a request to dequeue
* any number of bytes will result in a return of 0 from
* cbfifo_dequeue.
*
*****************************************************************************/
size_t cbfifo_dequeue(void *buf, size_t nbyte, cbfifo *cbfifo);

/****************************************************************************
*
* Returns the passed FIFO's capacity
*
* Parameters:
*   cbfifo *cbfifo		cbfifo instance
*
* Returns:
*   The capacity, in bytes, for the FIFO
*
****************************************************************************/
size_t cbfifo_capacity(cbfifo *cbfifo);

/*****************************************************************************
*
* Returns the number of bytes currently on the FIFO.
*
* Parameters:
*   cbfifo *cbfifo		cbfifo instance
*
* Returns:
*   Number of bytes currently available to be dequeued from the FIFO
*
*****************************************************************************/
size_t cbfifo_length(cbfifo *cbfifo);


#endif // _CBFIFO_H_
