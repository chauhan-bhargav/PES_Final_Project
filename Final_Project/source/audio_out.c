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
*    File name   : audio_out.c
*    Description : Audio output source file for dac functions
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

// including required libraries
#include <stdio.h>
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "sine.h"

// macros for constant values
#define DAC0_POS 			(30)
#define DAC_FREQ 			(48000)
#define SYSTEM_CLOCK 		(24000000U)
#define TPM_MOD_VALUE 		(SYSTEM_CLOCK/DAC_FREQ)
#define TPM0_DMAMUX_NUMBER 	(54)
#define BUFFER_SIZE 		(1024)

// global variables for dma
static uint16_t dma_buffer[BUFFER_SIZE];
static uint32_t dma_byte_count = 0;

// function definition in header file
uint32_t tone_to_samples(uint32_t tone_frequency, uint16_t *buffer,uint16_t size)
{
	// declaring variables for calculation
	uint16_t sin_value;
	uint32_t samples_per_period, total_samples;

	// this is done to get more accurate output frequency as some values of
	// frequencies when divide the dac freq has greater decimal value the 0.5
	// samples in one period is dac freq / req freq
	if(tone_frequency == 587 || tone_frequency == 659)
		samples_per_period = (DAC_FREQ / tone_frequency) + 1;
	else
		samples_per_period = DAC_FREQ / tone_frequency;

	// calculate total samples
	total_samples = samples_per_period * (size / samples_per_period);

	// populate the buffer by total number of sine values samples
	for (int i = 0; i < total_samples; i++) {

		// calculate sine value using the integer sine and scale it on positive axis
		sin_value = fp_sin((i * TWO_PI) / samples_per_period) + TRIG_SCALE_FACTOR;
		buffer[i] = sin_value;

	}

	return total_samples;
}

// function definition in header file
void init_DAC0(void)
{
	// enabling clock gating
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// configuring dac port and pin
	PORTE->PCR[DAC0_POS] &= ~(PORT_PCR_MUX(7));

	// disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// enabling DAC, selecting VDDA as reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

// function definition in header file
void init_TPM0(void)
{
	// enabling clock gating
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	// enabling 48Mhz PLL clock
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	// disable tpm before configuration
	TPM0->SC = 0;

	// set mod and counter value
	// interrupt at 48M/48k = 1000 ticks
	TPM0->MOD = TPM_MOD_VALUE;
	TPM0->CNT = 0;

	// configure the TPM status register
	TPM0->SC =  TPM_SC_DMA_MASK | 	// enable dma transfer
				TPM_SC_CMOD(1) 	| 	// enable the timer
				TPM_SC_PS(0) 	| 	// set prescaler to 1
				TPM_SC_CPWMS(0);	// set the timer to up counting mode

}

// function definition in header file
void init_DMA0(void)
{
	// Enabling clock gating to DMA and DMA Mux modules
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// Before changing the trigger or source settings,
	// DMA channel must be disabled via CHCFGn[ENBL] bit.
	DMAMUX0->CHCFG[0] = 0;

	// Configuring the DMA0 module
	DMA0->DMA[0].DCR = 	DMA_DCR_EINT_MASK 	|	// Enable interrupt on completion of transfer
						DMA_DCR_SINC_MASK 	|	// Source increment on transfer
						DMA_DCR_SSIZE(2) 	|	// 16-bit source data size
						DMA_DCR_DSIZE(2) 	|	// 16-bit destination data size
						DMA_DCR_ERQ_MASK 	|	// Enable peripheral request
						DMA_DCR_CS_MASK;		// cycle steal- single read/write transfer per request

	// Configuring NVIC for DMA interrupt service routine
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	// select the DMA0 trigger source as TPM0
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(TPM0_DMAMUX_NUMBER);

}

// function definition in header file
void start_DMA0_transfer()
{
	// initialize source and destination pointers
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)dma_buffer);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)(&(DAC0->DAT[0])));
	// byte count for trnsfer
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(dma_byte_count*2);
	// clear done flag
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
	// set enable flag
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;

}

// function definition in header file
void DMA0_IRQHandler(void)
{
	// clear done flag
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
	// start next dma transfer
	start_DMA0_transfer();
}

// function definition in header file
void generate_dma_buffer(uint16_t *buffer, uint32_t samples)
{
	// stop tpm0
	TPM0->SC &= ~TPM_SC_CMOD_MASK;

	// copy the input buffer to dma_buffer
	memcpy(dma_buffer, buffer, samples * 2);

	// update dma byte count to number of samples generated
	dma_byte_count = samples;

	// start tpm0
	TPM0->SC |= TPM_SC_CMOD(1);
}

