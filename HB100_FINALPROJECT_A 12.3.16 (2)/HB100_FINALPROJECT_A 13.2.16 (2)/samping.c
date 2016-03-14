/*
 * sampling.c
 *
 *  Created on: Nov 9, 2015
 *      Author: Thuy Vy
 */
#include "include.h"
#include "utils/uartstdio.c"
#include "FFT_signal.h"

extern FILTER mFilter;
extern volatile uint16_t g_uiDSPPerSec; // khong dat khoa static o day de tinh so vong lap

#pragma DATA_ALIGN(ucControlTable, UDMA_XFER_MAX)
unsigned char ucControlTable[UDMA_XFER_MAX ];

#ifdef USE_TEMPORARY_BUFFER
uint32_t g_ulADCValues[NUM_SAMPLES ] = {0};
uint32_t g_ulADCValues_B[NUM_SAMPLES ] = {0};
#endif

uint32_t g_ulADCValues[NUM_SAMPLES ] = { 0 };

uint32_t *currentAdcBuffer = g_ulADCValues;

#ifdef USING_BUFFER2
uint32_t output2[NUM_SAMPLES ] = {0};
#endif

//
// Variables used for storing ADC values and signalling which uDMA method to
// use
//
volatile unsigned char g_ucDMAMethod;
unsigned short g_usDMAping[DMA_SIZE];
unsigned short g_usDMApong[DMA_SIZE];
volatile unsigned char g_ucDMApingpong;

volatile unsigned short int flagTransBuffDone = 0;
volatile unsigned short int countFullData = 0;
volatile unsigned short int countTimerISRLoopSample = 0;

/*
 * F_sampling = 1/8000 = 125 (us)
 * 125 (us) x 1024 (sample) = 0.128 (s)
 * O day chinh lay 8000 boi vi test chay lay gia tri max index khong bi troi
 *
 *  */
SAMPLING_ADC adcNode[2] =
		{ { .g_uiSamplingFreq = 8000, .g_ucDataReady = 0,
				.g_ulBadPeriphIsr1 = 0, .g_ulBadPeriphIsr2 = 0,
				.g_uluDMAErrCount = 0 } };
//------------------------------------------------------------------------
#define LoadTimer	SysCtlClockGet() / (adcNode[0].g_uiSamplingFreq - 1)
//#define	LoadTimer	SysCtlClockGet() / calib_adc
//-------------------------------------------------------------------------
/*
 * function: Timer0AIntHandler
 *
 * */
void Timer0AIntHandler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	flagTransBuffDone = 1;
	countTimerISRLoopSample++;
	if (countTimerISRLoopSample > NUM_SAMPLES)
		countTimerISRLoopSample = 0;
#ifdef USE_TEMPORARY_BUFFER
	if (countTimerISRLoopSample >= FFT_LENGTH_256) {
		if (adcNode[0].g_ucDataReady == 0) {
			if (currentAdcBuffer == g_ulADCValues_B)
			currentAdcBuffer = g_ulADCValues;
			else
			currentAdcBuffer = g_ulADCValues_B;
		}
		countTimerISRLoopSample = 0;
	}
#endif
}

//volatile unsigned int count1 = 0;
/*
 * function: Timer0AIntHandler
 *
 * */
//void Timer1AIntHandler(void) {
//	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//	if (adcNode[0].g_ucDataReady) {
//		memcpy(g_ulADCValues, output2, 1024);
//		stfflag = 1;
//	}
//	count1++;
//}
/*
 *
 * */
void Timer2AIntHandler(void) {
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	g_uiDSPPerSec = 0;

	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet());
	TimerEnable(TIMER2_BASE, TIMER_A);
}
/*
 * function: uDMAErrorHandler
 *
 * */
void uDMAErrorHandler(void) {
	unsigned long ulStatus;

// Check for uDMA error bit
	ulStatus = uDMAErrorStatusGet();

	if (ulStatus) {
		uDMAErrorStatusClear();
		adcNode[0].g_uluDMAErrCount++;
	}
}
/*
 * function: ADC3IntHandler
 *  interrupt handler ADC0, sequence 3
 * return: none
 * */
void ADC3IntHandler(void) {
	unsigned long ulStatus;
	static unsigned long uluDMACount = 0;
	static unsigned long ulDataXferd = 0;
	unsigned long ulNextuDMAXferSize = 0;

	unsigned short *pusDMABuffer;
	unsigned short *pusCopyBuffer;
	int i;

	ADCIntClear(ADC0_BASE, SEQUENCER);
// If the channel's not done capturing, we have an error
	if (uDMAChannelIsEnabled(UDMA_CHANNEL_ADC3)) {
		// Increment error counter
		adcNode[0].g_ulBadPeriphIsr2++;

		ADCIntDisable(ADC0_BASE, SEQUENCER);
		IntPendClear(INT_ADC0SS3);
		return;
	}
	ulStatus = uDMAChannelSizeGet(UDMA_CHANNEL_ADC3);
// If non-zero items are left in the transfer buffer
// Something went wrong
	if (ulStatus) {
		adcNode[0].g_ulBadPeriphIsr1++;
		return;
	}
	if (g_ucDMAMethod == DMA_METHOD_SLOW) {
		//
		// We are using the slow DMA method, meaning there are not enough
		// samples in a second to generate a new set of FFT values and still
		// have data frequent enough to refresh at 15 frames per second
		//

		//
		// when pingpong is 0, uDMA just finished transferring into ping, so next
		// we transfer into pong.
		//
		if (g_ucDMApingpong == 0) {
			pusDMABuffer = g_usDMApong;
			pusCopyBuffer = g_usDMAping;
			g_ucDMApingpong = 1;
		} else {
			pusDMABuffer = g_usDMAping;
			pusCopyBuffer = g_usDMApong;
			g_ucDMApingpong = 0;
		}

		//
		// Set up the next uDMA transfer
		//
		uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
		UDMA_MODE_BASIC, (void *) (ADC0_BASE + ADC_O_SSFIFO3),// + (0x20 * UDMA_ARB_1)),
				pusDMABuffer, DMA_SIZE);
		uDMAChannelEnable(UDMA_CHANNEL_ADC3);

		IntPendClear(INT_ADC0SS3);

		//
		// Shift everything back DMA_SIZE samples
		//
		for (i = 0; i < (NUM_SAMPLES - DMA_SIZE); i++) {
			g_ulADCValues[i] = g_ulADCValues[i + DMA_SIZE];
		}

		//
		// Copy the new samples from the copy buffer into the sample array
		//
		for (i = 0; i < DMA_SIZE; i++) {
			g_ulADCValues[i + NUM_SAMPLES - DMA_SIZE] = pusCopyBuffer[i];
		}

		//
		// Signal that we have new data to be processed
		//
		adcNode[0].g_ucDataReady = 1;
	} else {
// Disable the sampling timer
		TimerDisable(TIMER0_BASE, TIMER_A);
// how many times the DMA has been full without processing the data
		uluDMACount++;
// The amount of data transferred increments in sets of 1024
		ulDataXferd += UDMA_XFER_MAX;

		if (NUM_SAMPLES > ulDataXferd) {
			if ((NUM_SAMPLES - ulDataXferd) > UDMA_XFER_MAX) {
				ulNextuDMAXferSize = UDMA_XFER_MAX;
			} else {
				ulNextuDMAXferSize = NUM_SAMPLES - ulDataXferd;
			}
#ifdef USE_TEMPORARY_BUFFER
			if (currentAdcBuffer == g_ulADCValues_B) {
				uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
						UDMA_MODE_BASIC,
						(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
						g_ulADCValues + (UDMA_XFER_MAX * uluDMACount),
						ulNextuDMAXferSize);
			} else {
				uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
						UDMA_MODE_BASIC,
						(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
						g_ulADCValues_B + (UDMA_XFER_MAX * uluDMACount),
						ulNextuDMAXferSize);
			}
#endif

			uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
			UDMA_MODE_BASIC,
					(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
					g_ulADCValues + (UDMA_XFER_MAX * uluDMACount),
					ulNextuDMAXferSize);

			uDMAChannelEnable(UDMA_CHANNEL_ADC3);
			TimerLoadSet(TIMER0_BASE, TIMER_A, LoadTimer);
			TimerEnable(TIMER0_BASE, TIMER_A);

		} else {
			uluDMACount = 0;
			ulDataXferd = 0;
			ADCIntDisable(ADC0_BASE, SEQUENCER);
			IntPendClear(INT_ADC0SS3);
			// Signal that we have new data to be processed
			adcNode[0].g_ucDataReady = 1;
#ifdef USING_BUFFER2
			if (adcNode[0].g_ucDataReady) {
				memcpy(output2, g_ulADCValues, NUM_SAMPLES);
				countFullData++;
			}
#endif
		}
	}
}
/*
 * function: InitADC3Transfer
 * return: none
 * */
unsigned int uIdx = 0;
void InitADC3Transfer(void) {
// Set data as not ready to be processed
	adcNode[0].g_ucDataReady = 0;
// Init buffers by setting them all to 0
// Should go from 0 to 2048
	for (uIdx = 0; uIdx < NUM_SAMPLES ; uIdx++) {
		g_ulADCValues[uIdx] = 0;
	}

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralReset(SYSCTL_PERIPH_ADC0);

	IntEnable(INT_UDMAERR);
	uDMAEnable();
	uDMAControlBaseSet(ucControlTable);
	//
	// Configure the ADC to use PLL at 480 MHz divided by 24 to get an ADC
	// clock of 20 MHz.
	//
	//ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 24);
	ADCSequenceConfigure(ADC0_BASE, SEQUENCER, ADC_TRIGGER_TIMER, 0);

#ifdef test_w_internal_temp
	ADCSequenceStepConfigure(ADC0_BASE, SEQUENCER, 0, ADC_CTL_TS |
			ADC_CTL_IE | ADC_CTL_END); // internal temperator
#endif

	ADCSequenceStepConfigure(ADC0_BASE, SEQUENCER, 0, ADC_CTL_CH0 |
	ADC_CTL_IE | ADC_CTL_END); //PE3

	ADCSequenceEnable(ADC0_BASE, SEQUENCER);
	ADCIntEnable(ADC0_BASE, SEQUENCER);

	uDMAChannelAttributeDisable(UDMA_CHANNEL_ADC3,
	UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
	UDMA_ATTR_HIGH_PRIORITY |
	UDMA_ATTR_REQMASK);

	uDMAChannelControlSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
	UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
	UDMA_DST_INC_16 | UDMA_ARB_1);
	if (g_ucDMAMethod == DMA_METHOD_SLOW) {
		g_ucDMApingpong = 0;
		uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
		UDMA_MODE_BASIC,
				(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
				g_usDMAping, DMA_SIZE);
	} else {
		if (NUM_SAMPLES > UDMA_XFER_MAX) {
			uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
			UDMA_MODE_BASIC,
					(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
					g_ulADCValues, UDMA_XFER_MAX);
		} else {
			uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
			UDMA_MODE_BASIC,
					(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
					g_ulADCValues, NUM_SAMPLES);
		}
	}
	uDMAChannelEnable(UDMA_CHANNEL_ADC3);
}
/*
 * function: InitSamplingTimer
 * return: none
 * */
void InitSamplingTimer() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
	TimerLoadSet(TIMER0_BASE, TIMER_A, LoadTimer);
	/*
	 * timer set : 20 (us)
	 * */
// Enable the sampling interrupt
	IntEnable(INT_TIMER0A);
// When timer hits zero, call interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
// Start the sampling timer
	TimerEnable(TIMER0_BASE, TIMER_A);
}
/*
 *
 * */
//void InitDSPtimer(void) {
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
//	uint32_t ui32Period = 0;
//	ui32Period = SysCtlClockGet() / 100;
//
//	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
//	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / (ui32Period - 1)); // ...s update
//
//	IntEnable(INT_TIMER1A);
//// When timer hits zero, call interrupt
//	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//// Start the sampling timer
//	TimerEnable(TIMER1_BASE, TIMER_A);
//}
/*
 *
 * */
void setAgainSampling(void) {
// Clear the data ready bit and set up the next DMA transfer
	adcNode[0].g_ucDataReady = 0;
	if (NUM_SAMPLES > UDMA_XFER_MAX) {
		uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
		UDMA_MODE_BASIC,
				(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
				g_ulADCValues, UDMA_XFER_MAX);
	} else {
		uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
		UDMA_MODE_BASIC,
				(void *) (ADC0_BASE + ADC_O_SSFIFO3 + (0x20 * UDMA_ARB_1)),
				g_ulADCValues, NUM_SAMPLES);
	}
// Enable the timer and start the sampling timer
	uDMAChannelEnable(UDMA_CHANNEL_ADC3);
	TimerLoadSet(TIMER0_BASE, TIMER_A, LoadTimer);

	TimerEnable(TIMER0_BASE, TIMER_A);
}
/*
 *
 * */
void InitDebugTimer(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet());
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER2_BASE, TIMER_A);
}
