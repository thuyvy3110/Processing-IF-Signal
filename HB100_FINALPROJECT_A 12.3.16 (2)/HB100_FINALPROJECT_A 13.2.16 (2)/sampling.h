/*
 * sampling.h
 *
 *  Created on: Nov 9, 2015
 *      Author: Thuy Vy
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_

#define NUM_SAMPLES				(uint16_t)1024

#define UDMA_XFER_MAX			(uint16_t)512
#define DMA_METHOD_SLOW			0
#define DMA_METHOD_FAST			1
//
// The size of the ping pong buffer used for the slow uDMA method
//
#define DMA_SIZE 256

#define calib_adc	1

#define SEQUENCER	3

//#define test_w_internal_temp

typedef struct sampling {
	const unsigned int g_uiSamplingFreq;
	volatile unsigned short int g_ucDataReady;
	volatile uint32_t TempValueC;
	// The count of times various uDMA error conditions detected.
	volatile unsigned long g_ulBadPeriphIsr1;
	volatile unsigned long g_ulBadPeriphIsr2;
	volatile unsigned long g_uluDMAErrCount;
} SAMPLING_ADC;

void ADC_config(void);
void ADC_read(void);

void ADC3IntHandler(void);
void InitADC3Transfer(void);
void Timer0AIntHandler(void);
//void Timer1AIntHandler(void);
void InitSamplingTimer(void);
void InitDSPtimer(void);
void uDMAErrorHandler(void);
void setAgainSampling(void);
void InitDebugTimer(void);

#endif /* SAMPLING_H_ */
