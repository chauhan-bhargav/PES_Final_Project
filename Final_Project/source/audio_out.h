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
*    File name   : audio_out.h
*    Description : Audio output header file for dac functions
*    definitions
*
*    Author: Bhargav Dharmendra Chauhan
*    Tools : MCUXpresso
*    Date  : 04/19/2021
*
*	 Reference: Dean's github and book
*	 			https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code
*	 			Prof Howdy's Lecture and slides
*****************************************************************************/

#ifndef AUDIO_OUT_H_
#define AUDIO_OUT_H_

/*****************************************************************************
* Initializes the DAC module of KL25Z
*
*****************************************************************************/
void init_DAC0(void);

/*****************************************************************************
* Initializes the TPM0 module to generate periodic interrupts used for DMA
* transfer
* Generates interrupt at every 1000 ticks to get 48khz sampling rate
*
*****************************************************************************/
void init_TPM0(void);

/*****************************************************************************
* Initializes the DMA module to transfer the DAC buffer values efficiently
*
*****************************************************************************/
void init_DMA0(void);

/*****************************************************************************
* This function starts the DMA transfer after the respective registers are
* loaded
*
*****************************************************************************/
void start_DMA0_transfer();

/*****************************************************************************
* Populated the DMA buffer by performing from DAC input buffer for DMA transfer
*
* Parameters:
*   *buffer			buffer to store DAC input values
*   buffer_size		size of the input buffer
*
*****************************************************************************/
void generate_dma_buffer(uint16_t *buffer, uint16_t samples);

/*****************************************************************************
* DMA0 ISR routine function, performs DMA transfer repeatedly
*
*****************************************************************************/
void DMA0_IRQHandler(void);

/*****************************************************************************
* This function calculates the number of samples generated for a given input
* frequency and returns the value
*
* Parameters:
*   *buffer			buffer to store DAC input values
*   size			size of the input buffer
*   tone_frequency	input tone frequency
*****************************************************************************/
uint32_t tone_to_samples(uint32_t tone_frequency, uint16_t *buff, uint16_t size);

#endif /* AUDIO_OUT_H_ */
