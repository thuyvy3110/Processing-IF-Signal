/*
 * FFT_signal.h
 *
 *  Created on: Feb 8, 2016
 *      Author: Elmeo
 */

#ifndef DSP_FFT_SIGNAL_H_
#define DSP_FFT_SIGNAL_H_

#include "math_helper.h"
#include "arm_math.h"

//*****************************************************************************
//
// Private predefines and variables used for the FFT portion of the DSP loop
//
//*****************************************************************************

// Flag used to determine whether to calculate forward (0) or inverse(1) fft
#define INVERT_FFT				0

// Flag used to determine if fft result will be output in standard bit order(1)
// or reversed bit order (0)
#define BIT_ORDER_FFT			1

typedef struct fft {
	// The range in Hertz of each frequency bin
	float32_t HerztPerBin;
	// For finding bin with highest power
	float32_t maxValue;
	float32_t averageValue;
	uint32_t maxIndex;
	float32_t hertz;
} FFT;
typedef enum fft_length {
	FFT_LENGTH_256 = 256, FFT_LENGTH_512 = 512, FFT_LENGTH_1024 = 1024
} FFT_LENGTH_;

void static ComplexBufFFT(uint32_t* input, float32_t* output,
		uint32_t FFTlength);
void InitDSP1(void);
void InitDSP2(void);

void FFTprocessing1(uint32_t *inFFT, float32_t* outFFT, FFT_LENGTH_ FFTlength);
void FFTprocessing2(uint32_t *inFFT, float32_t* outFFT);

#endif /* DSP_FFT_SIGNAL_H_ */
