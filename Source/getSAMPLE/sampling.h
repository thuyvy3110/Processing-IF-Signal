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

#define calib_adc	5

#define SEQUENCER0	0
#define SEQUENCER1	1
#define SEQUENCER2	2
#define SEQUENCER3	3

extern volatile uint16_t g_uiDSPPerSec;

typedef struct sampling {
	const unsigned int g_uiSamplingFreq;
	volatile unsigned char g_ucDataReady;
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
