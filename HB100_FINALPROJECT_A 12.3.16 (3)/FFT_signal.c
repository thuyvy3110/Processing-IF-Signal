/*
 * FFT_signal.c
 *
 *  Created on: Feb 8, 2016
 *      Author: Elmeo
 */
/*
 * signal_dsp.c
 *
 *  Created on: Nov 26, 2015
 *      Author: Thuy Vy
 */
#include "include.h"
#include "math_helper.h"
#include "arm_const_structs.h"

extern float ti_hamming_window_vector[NUM_SAMPLES ];
extern volatile unsigned char g_ucDMAMethod;
/*+++++++++++++++++++++++++++++++++++++ FAST FOURIER TRANSFORM FUNCTION +++++++++++++++++++++++++++++++++++*/
FFT fftNode[2] = { { .HerztPerBin = 0.0f, .maxValue = 0.0f,
		.averageValue = 0.0f, .maxIndex = 0, .hertz = 0.0f } };

arm_rfft_instance_f32 fftStructure;
arm_rfft_fast_instance_f32 FastfftStructure;
arm_cfft_radix4_instance_f32 cfftStruture;

uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
/*
 *
 */
void InitDSP1(void) {
	float32_t NyquistFreq = (float32_t) adcNode[0].g_uiSamplingFreq / 2;
	fftNode[0].HerztPerBin = NyquistFreq / ((float) NUM_SAMPLES / 2);
//	fftNode[0].HerztPerBin = (float32_t) adcNode[0].g_uiSamplingFreq
//			/ (float32_t) NUM_SAMPLES;
	if ((adcNode[0].g_uiSamplingFreq / NUM_SAMPLES ) > 16) {
		g_ucDMAMethod = DMA_METHOD_FAST;
	} else {
		g_ucDMAMethod = DMA_METHOD_SLOW;
	}
}
void InitDSP2(void) {
	fftNode[0].HerztPerBin = (float) adcNode[0].g_uiSamplingFreq
			/ (float) NUM_SAMPLES;
	// Call the CMSIS real fft init function
//	arm_rfft_init_f32(&fftStructure, &cfftStruture, NUM_SAMPLES, INVERT_FFT,
//	BIT_ORDER_FFT);
	arm_rfft_fast_init_f32(&FastfftStructure, NUM_SAMPLES);
}
/*
 *
 * */
void static ComplexBufFFT(uint32_t* input, float32_t* output,
		uint32_t FFTlength) {
	uint16_t i;
	for (i = 0; i < FFTlength * 2; i += 2) {
		output[i] = (float32_t) (input[i / 2] & 0x03ff) / 1023.0f; // FIXED HERE! ==== 0x0fff) / 4095.0f -> 0x03ff) / 1023.0f;
		output[i + 1] = 0.0f;
	}
}
/*
 *
 * */
void FFTprocessing1(uint32_t *inFFT, float32_t* outFFT, FFT_LENGTH_ FFTlength) {
	if (g_ucDMAMethod == DMA_METHOD_SLOW) {
		adcNode[0].g_ucDataReady = 0;
	}
	float32_t complexBuffer[NUM_SAMPLES ];
	ComplexBufFFT(inFFT, complexBuffer, FFTlength);
	arm_cfft_f32(&arm_cfft_sR_f32_len256, complexBuffer, ifftFlag,
			doBitReverse); //da test voi 512 nhung chay mot luc thi gia tri khong cap nhat nua
	arm_cmplx_mag_f32(complexBuffer, outFFT, FFTlength);

	// ignore the DC value
	outFFT[0] = 0.0f;
	uint16_t n = 0;
	///(50*SAMPLE_RATE)/fftLength;
	// squash everything under 100Hz
	for (n = 0; n < fix_minFFTIndex; n++) {
		outFFT[n] = 0.0f;
	}

	arm_max_f32(outFFT, FFTlength / 2, &fftNode[0].maxValue,
			&fftNode[0].maxIndex);
	arm_mean_f32(outFFT, FFTlength, &fftNode[0].averageValue);
	// calculate frequency value of peak bin
	fftNode[0].hertz = fftNode[0].HerztPerBin * (float32_t) fftNode[0].maxIndex
			* 2;

	setAgainSampling();
}
/*
 * using with InitDSP2 function
 * */

void FFTprocessing2(uint32_t *inFFT, float32_t* outFFT) {
	uint16_t m;
	for (m = 0; m < NUM_SAMPLES ; m++)
		outFFT[m] = (float) inFFT[m];	//- (float) 0x800;
//	arm_rfft_f32(&fftStructure, outFFT, outFFT);
	arm_rfft_fast_f32(&FastfftStructure, outFFT, outFFT, ifftFlag);
	arm_cmplx_mag_f32(outFFT, outFFT, NUM_SAMPLES * 2);
	// ignore the DC value
	outFFT[0] = 0.0f;
	uint16_t n = 0;
	///(50*SAMPLE_RATE)/fftLength;
	// squash everything under 100Hz
	for (n = 0; n < fix_minFFTIndex; n++) {
		outFFT[n] = 0.0f;
	}
	arm_max_f32(outFFT, NUM_SAMPLES, &fftNode[0].maxValue,
			&fftNode[0].maxIndex);
	// calculate frequency value of peak bin
	fftNode[0].hertz =
			(fftNode[0].HerztPerBin * (float32_t) fftNode[0].maxIndex) / 2;

	setAgainSampling();
}

/*++++++++++++++++++++++++++++++++++++++++++++ END FFT FUNCTION ++++++++++++++++++++++++++++++++++++++++*/

