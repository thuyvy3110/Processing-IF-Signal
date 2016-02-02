/*
 * signal_dsp.c
 *
 *  Created on: Nov 26, 2015
 *      Author: Thuy Vy
 */
#include "include.h"
#include "math_helper.h"
#include "getSAMPLE/sampling.h"
#include "DSP/signal_dsp.h"
#include "arm_const_structs.h"

/*+++++++++++++++++++++++++++++++++++++ Choice FILTER FUNCTION +++++++++++++++++++++++++++++++++++*/

ProcessingType choiceTypeProcessing(ProcessingType cF) {
	return cF;
}

//----------------------------------------------------------------------------------------------//
uint32_t g_fFIRResult[NUM_SAMPLES] = { 0 };
static float32_t firStateF32[NUM_SAMPLES + NUM_TAPS - 1];
/*
 *
 * Filter coefficients
 * file : fdacoefs.h
 *
 * */
/* ------------------------------------------------------------------
 * Global variables for FIR LPF Example
 * ------------------------------------------------------------------- */
static const FILTER FilterNode[2] = { { .blockSize = BLOCK_SIZE, .numBlocks =
NUM_SAMPLES / BLOCK_SIZE } };
/*
 *
 * */
void FIRprocessing(uint32_t* input, float32_t* output) {
	arm_fir_instance_f32 S;

	input = &output2[0];
	output = &g_fFIRResult[0];

	arm_fir_init_f32(&S, NUM_TAPS, (float32_t*) &firCoeffs32[0],
			&firStateF32[0], FilterNode[0].blockSize);
	uint32_t i = 0;
	for (i = 0; i < FilterNode[0].blockSize; i++)
		arm_fir_f32(&S, (float32_t*) input + (i * FilterNode[0].blockSize),
				output + (i * FilterNode[0].blockSize),
				FilterNode[0].blockSize);
	setAgainSampling();
}

/*++++++++++++++++++++++++ END FIR FILTER FUNCTION ++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++ FAST FOURIER TRANSFORM FUNCTION +++++++++++++++++++++++++++++++++++*/
/* -------------------------------------------------------------------
 * External Input and Output buffer Declarations for FFT Bin Example
 * ------------------------------------------------------------------- */
float32_t g_fFFTResult[NUM_SAMPLES * 2] = { 0 };

//
// The range in Hertz of each frequency bin
//
FFT fftNode[2] = { { .g_HzPerBin = 0, .maxValue = 0, .meanValue = 0 } };

arm_rfft_instance_f32 fftStructure;
arm_rfft_fast_instance_f32 FastfftStructure;
arm_cfft_radix4_instance_f32 cfftStruture;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len1024;
/*
 *Calculats the Hz per bin infrequency domain.
 *  Also initializes the fft structure.
 *
 */
void InitDSP(void) {

	// Determine the
	fftNode[0].g_HzPerBin = (float) adcNode[0].g_uiSamplingFreq
			/ (float) NUM_SAMPLES;

	// Call the CMSIS real fft init function
//	arm_rfft_init_f32(&fftStructure, &cfftStruture, NUM_SAMPLES, INVERT_FFT,
//	BIT_ORDER_FFT);
	arm_rfft_fast_init_f32(&FastfftStructure, NUM_SAMPLES);
}
/*
 *
 * */
float32_t velocity = 0.0;
void FFTprocessing(uint32_t* inFFT, float32_t* outFFT) {
	uint32_t i = 0;

	float32_t dataHB100 = 0.0174954627;
	for (i = 0; i < NUM_SAMPLES; i++)
		outFFT[i] = (float) inFFT[i] - (float) 0x800;

//	arm_cfft_f32(&arm_cfft_sR_f32_len1024, (float*) inFFT, ifftFlag,
//			doBitReverse); //da test nhung khong chinh xac

//arm_mult_f32(output, ti_hamming_window_vector, output, NUM_SAMPLES);

//	arm_rfft_f32(&fftStructure, output, output);
	arm_rfft_fast_f32(&FastfftStructure, outFFT, outFFT, ifftFlag);

	arm_cmplx_mag_f32(outFFT, outFFT, NUM_SAMPLES * 2);

	arm_max_f32(outFFT, NUM_SAMPLES, &fftNode[0].maxValue, &i);

	arm_mean_f32(outFFT, NUM_SAMPLES, &fftNode[0].meanValue);

	velocity = fftNode[0].meanValue * dataHB100;

	setAgainSampling();
}
/*++++++++++++++++++++++++++++++++++++++++++++ END FFT FUNCTION ++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++ FIR FILTER FUNCTION +++++++++++++++++++++++++++++++++++*/
/*
 * 21/12/2015
 * circle buffer + fir filter
 */
void wrap_(int M, uint32_t* w, uint32_t** p) {
	if (*p > w + M)
		*p -= M + 1;
	if (*p < w)
		*p += M + 1;
}
/*
 * --> a = (*p)
 *--> b = *a
 *--> c = b-- = x
 * */
uint32_t cfir(int M, uint32_t* h, uint32_t* w, uint32_t** p, uint32_t x) {
	int i = 0;
	uint32_t y;

	*(*p)-- = x; // gan gia tri roi tru dia chi di 1
	wrap_(M, w, p);
	for (y = 0, h += M; i >= 0; i--) {
		y += (*h--) * (*(*(p))--); // gan gia tri roi tru dia chi di 1
		wrap_(M, w, p);
	}
	return y;
}

/*+++++++++++++++++++++++++++++++++++++ FILTER FUNCTION +++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++ FILTER FUNCTION +++++++++++++++++++++++++++++++++++*/
/*
 *
 */
void LowpassFilter(uint32_t* input, uint32_t* output, float Fcutoff,
		float SAMPLE_RATE) {
//Bad: Here, i want to checking if input number sample all is 0, but it not worked!!
//	if (input == NULL)
//		setPointErr = 1;
//static float output[NUM_SAMPLES] = { 0 };
	float RC = 1.0 / (Fcutoff * 2 * 3.14);
	float dt = 1.0 / SAMPLE_RATE;
	float alpha = dt / (RC + dt);
	output[0] = input[0];
	int i = 0;
	for (i = 0; i < NUM_SAMPLES; i++) {
		output[i] = output[i - 1] + alpha * (input[i] - output[i]);
	}
	setAgainSampling();
}
/*
 *
 */
void HighPassFilter(uint32_t* input, uint32_t* output, float Fcutoff,
		float SAMPLE_RATE) {
	float RC = 1.0 / (Fcutoff * 2 * 3.14);
	float dt = 1.0 / SAMPLE_RATE;
	float alpha = RC / (RC + dt);
	output[0] = input[0];
	int i = 0;
	for (i = 0; i < NUM_SAMPLES; i++)
		output[i] = alpha * (output[i - 1] + input[i] - input[i - 1]);
	setAgainSampling();
}
/*
 *
 */
void PRE_filter(PRE_Obj *pre, int input[], int length) {
	int i, tmp;
	for (i = 0; i < length; i++) {
		tmp = input[i] - pre->z0 + (13 * pre->z1 + 16) / 32;
		pre->z1 = pre->z0;
		pre->z0 = input[i];
		input[i] = tmp;
	}
}
/*+++++++++++++++++++++++++++++++++++++ END FILTER FUNCTION +++++++++++++++++++++++++++++++++++*/

