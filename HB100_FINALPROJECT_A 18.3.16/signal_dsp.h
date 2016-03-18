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

//#define TEST_LENGTH_SAMPLES  320
//#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            32
#define NUM_TAPS              318

extern SAMPLING_ADC adcNode[2];
extern uint32_t g_ulADCValues[NUM_SAMPLES];
extern uint32_t output2[NUM_SAMPLES];

extern const float32_t firCoeffs32[NUM_TAPS];

typedef struct fir {

} FIR_FILTER;
typedef struct filter {
	uint32_t blockSize;
	uint32_t numBlocks;
} FILTER;

typedef struct PRE_Obj { /* state obj for pre-emphasis alg */
	int z0;
	int z1;
} PRE_Obj;

typedef enum ft {
	MY_FIR_FILTER = 0,
	STANDARD_FIR_FILTER,
	LOWPASS_FILTER,
	HIGHPASS_FILTER,
	NONE
} ProcessingType;

ProcessingType choiceTypeProcessing(ProcessingType cF);

void wrap_(int M, uint32_t* w, uint32_t** p);
uint32_t cfir(int M, uint32_t* h, uint32_t* w, uint32_t** p, uint32_t x);
void FIRprocessing(uint32_t* input, float32_t* output);
void LowpassFilter(uint32_t* input, uint32_t* output, float CUTOFF,
		float SAMPLE_RATE);
void HighPassFilter(uint32_t* input, uint32_t* output, float Fcutoff,
		float SAMPLE_RATE);
void PRE_filter(PRE_Obj *pre, int input[], int length);
#endif /* DSP_SIGNAL_DSP_H_ */
