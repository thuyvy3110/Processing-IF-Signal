#include "include.h"

/* Include arm_math.h mathematic functions */
#include "arm_math.h"
#include "driverlib/debug.h"

#define FFT_

#define LEDRED		GPIO_PIN_1
#define	LEDGREEEN	GPIO_PIN_3
#define	LEDBLUE		GPIO_PIN_2

extern SAMPLING_ADC adcNode[2];
extern uint32_t g_ulADCValues[NUM_SAMPLES ];
extern uint32_t output2[NUM_SAMPLES ];
extern volatile unsigned char flagTransBuffDone;
//------------------------------------
uint32_t output3[NUM_SAMPLES ] = { 0 };
//------------------------------------
extern volatile unsigned char LPFdone;
extern uint32_t g_fFIRResult[NUM_SAMPLES ];
//
volatile uint16_t g_uiDSPPerSec = 0; // khong dat khoa static o day de tinh so vong lap

//
extern float32_t g_fFFTResult[NUM_SAMPLES * 2];

/*------------------------------------------------------------------------------*/
void delayMS(int ms);
void InitConsole(void);
void InitGPIO(void);

int main() {
	uint32_t *h, *w, *p; //test cfir filter, ok!
	ProcessingType choiceProcessing;

	g_uiDSPPerSec = 0;
	SysCtlClockSet(
	SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80MHZ

	FPUEnable();
	FPULazyStackingEnable();

	InitSamplingTimer();
	InitADC3Transfer();
	//InitDSPtimer();
	InitDebugTimer();
	InitGPIO();
	InitDSP();

	IntEnable(INT_ADC0SS3);
	IntMasterEnable();
	choiceProcessing = choiceTypeProcessing(HIGHPASS_FILTER);

	int firloop1 = 0, i = 0;
	while (1) {
		if (adcNode[0].g_ucDataReady) {
			switch (choiceProcessing) {
			case LOWPASS_FILTER:
				LowpassFilter(output2, output3, 1000,
						adcNode[0].g_uiSamplingFreq);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDGREEEN, LEDGREEEN);
				break;
			case HIGHPASS_FILTER:
				HighPassFilter(output2, output3, 5,
						adcNode[0].g_uiSamplingFreq);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDGREEEN, LEDGREEEN);
				break;
			case MY_FIR_FILTER:
				for (firloop1 = 0; firloop1 < NUM_SAMPLES ; firloop1++)
					output3[firloop1] = cfir(NUM_SAMPLES, h, w, &p,
							output2[firloop1]);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDRED, LEDRED);
				break;
			case STANDARD_FIR_FILTER:
				FIRprocessing(output2, g_fFIRResult);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDBLUE, LEDBLUE);
				break;
			default:
				break;
			} //switch
// for debugging ------------------------------------------------------------
#ifdef FFT_
			FFTprocessing(output3, g_fFFTResult);
#endif
			g_uiDSPPerSec++; //neu gia tri cua bien khong tang thi "Processing" qua trinh bi loi
//------------------------------------------------------------------------
			/*i have a bug here, chi co gia tri thu 0 la cap nhat*/
//			for (i = 0; i < 10; i++)
//				output4[i] = (uint32_t) (147.5
//						- ((75.0 * 3.3 * (float) output3[i])) / 4096.0);
		} //if
	} //while
}
void delayMS(int ms) {
	SysCtlDelay((SysCtlClockGet() / (3 * 1000)) * ms);
}
/*
 *
 * */
void InitConsole(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioConfig(0, 115200, 16000000);
}
/*
 *
 * */
void InitGPIO(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
	GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
}

