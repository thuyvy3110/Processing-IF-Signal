/*
 * signal_dsp.h
 *
 *  Created on: Nov 26, 2015
 *      Author: Thuy Vy
 */

#ifndef DSP_SIGNAL_DSP_H_
#define DSP_SIGNAL_DSP_H_

#include "math_helper.h"
#include "arm_math.h"
#include "getSAMPLE/buffer.h"

//#define TEST_LENGTH_SAMPLES  320
//#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            32
#define NUM_TAPS              318

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

extern SAMPLING_ADC adcNode[2];
extern uint32_t g_ulADCValues[NUM_SAMPLES];
extern uint32_t output2[NUM_SAMPLES];

extern float32_t refOutput[NUM_SAMPLES];

extern const float32_t firCoeffs32[NUM_TAPS];

extern float ti_hamming_window_vector[NUM_SAMPLES];

typedef struct fir {

} FIR_FILTER;
typedef struct filter {
	uint32_t blockSize;
	uint32_t numBlocks;
} FILTER;
typedef struct fft {
	// The range in Hertz of each frequency bin
	float g_HzPerBin;
	// For finding bin with highest power
	float32_t maxValue;
	float32_t meanValue;
} FFT;

typedef struct PRE_Obj { /* state obj for pre-emphasis alg */
	int z0;
	int z1;
} PRE_Obj;

typedef enum ft {
	MY_FIR_FILTER = 0, STANDARD_FIR_FILTER, LOWPASS_FILTER, HIGHPASS_FILTER
} ProcessingType;

ProcessingType choiceTypeProcessing(ProcessingType cF);

void wrap_(int M, uint32_t* w, uint32_t** p);
uint32_t cfir(int M, uint32_t* h, uint32_t* w, uint32_t** p, uint32_t x);
void FIRprocessing(uint32_t* input, float32_t* output);
void InitDSP(void);
void FFTprocessing(uint32_t* input, float32_t* output);
void LowpassFilter(uint32_t* input, uint32_t* output, float CUTOFF,
		float SAMPLE_RATE);
void HighPassFilter(uint32_t* input, uint32_t* output, float Fcutoff,
		float SAMPLE_RATE);
void PRE_filter(PRE_Obj *pre, int input[], int length);
#endif /* DSP_SIGNAL_DSP_H_ */
