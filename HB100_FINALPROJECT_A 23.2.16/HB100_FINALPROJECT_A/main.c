#include "include.h"
/* Include arm_math.h mathematic functions */

#include <stdarg.h>
#include "main.h"

extern FFT fftNode[2];

extern volatile unsigned char flagTransBuffDone;
//------------------------------------
uint32_t output3[NUM_SAMPLES ] = { 0 };
//------------------------------------
extern volatile unsigned char LPFdone;
extern uint32_t g_fFIRResult[NUM_SAMPLES ];

extern SAMPLING_ADC adcNode[2];
extern uint32_t g_ulADCValues[NUM_SAMPLES ];

//#include "utils/uartstdio.c"

volatile uint16_t g_uiDSPPerSec = 0; // khong dat khoa static o day de tinh so vong lap
float32_t g_fFFTResult[NUM_SAMPLES ] = { 0 };

/*------------------------------------------------------------------------------*/
int main(void) {
	uint32_t *h, *w, *p; //test cfir filter, ok!

	g_uiDSPPerSec = 0;
	SysCtlClockSet(
	SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //200 / 5 = 40MHZ

	FPUEnable();
	FPULazyStackingEnable();

	InitSamplingTimer();
	InitADC3Transfer();
	//InitDSPtimer();
	InitDebugTimer();
	InitGPIO();
	InitConsole();
#ifdef FFT1_
	InitDSP1();
#endif
#ifdef FFT2_
	InitDSP2();
#endif

	IntEnable(INT_ADC0SS3);
	IntMasterEnable();

	int firloop1 = 0, i = 0;
	while (1) {
		if (adcNode[0].g_ucDataReady) {
			switch (choiceTypeProcessing(NONE)) {
			case LOWPASS_FILTER:
				LowpassFilter(g_ulADCValues, output3, 10,
						adcNode[0].g_uiSamplingFreq);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDGREEEN | LEDRED,
				LEDGREEEN | LEDRED);
				break;
			case HIGHPASS_FILTER:
				HighPassFilter(g_ulADCValues, output3, 5,
						adcNode[0].g_uiSamplingFreq);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDGREEEN, LEDGREEEN);
				break;
			case MY_FIR_FILTER:
				for (firloop1 = 0; firloop1 < NUM_SAMPLES ; firloop1++)
					output3[firloop1] = cfir(NUM_SAMPLES, h, w, &p,
							g_ulADCValues[firloop1]);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDRED, LEDRED);
				break;
			case STANDARD_FIR_FILTER:
				FIRprocessing(g_ulADCValues, g_fFIRResult);
				GPIOPinWrite(GPIO_PORTF_BASE, LEDBLUE, LEDBLUE);
				break;
			default:
				break;
			} //switch
// for debugging ------------------------------------------------------------
#ifdef FFT1_
			FFTprocessing1(g_ulADCValues, g_fFFTResult);
#endif
#ifdef FFT2_
			FFTprocessing2(g_ulADCValues, g_fFFTResult);
#endif
			g_uiDSPPerSec++; //neu gia tri cua bien khong tang thi "Processing" qua trinh bi loi

			DisplayData(fftNode[0].hertz);
			//
			// Display the setup on the console.
			//

			//FIX HERE AGAIN
//			if (g_fFFTResult[fftNode[0].maxIndex] < 3.0f) {
//				fftNode[0].hertz = 0.0f;
//			}

//------------------------------------------------------------------------
			/*i have a bug here, chi co gia tri thu 0 la cap nhat*/
//			for (i = 0; i < 10; i++)
//				output4[i] = (uint32_t) (147.5
//						- ((75.0 * 3.3 * (float) output3[i])) / 4096.0);
		}
		//if
	} //while
}

/*
 * */
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

/*
 *
 * */

void DisplayData(float32_t hertz) {
// calculate the number of Doppler Hertz per m/s for this radar frequency;
	static float32_t HertzPerMs = 2.0f * RADAR_FREQUENCY / SPEED_OF_LIGHT;

	float32_t MetersPerSecond;
	float mph = 0.0f;
	float kmh = 0.0f;

	MetersPerSecond = hertz / HertzPerMs;
	mph = MetersPerSecond * MPH_FACTOR;
	kmh = MetersPerSecond * KMH_FACTOR;
//	char str[10];
	float c = mph - (uint16_t) mph;
	float d = kmh - (uint16_t) kmh;
	uint16_t g = c * 100;
	uint16_t k = d * 100;

	switch (MenuGetDisplayMode(DISPLAY_KMH)) {
	case DISPLAY_MPH:
#ifdef DEBUG_UART
//		UARTprintf("MPH = %d \t\t MaxFreq Index = %d\n", (uint16_t) mph,
//				fftNode[0].maxIndex);
		UARTprintf("MPH = %d.%3d \t\t MaxFreq Index = %d\n", (uint16_t) mph, g,
				fftNode[0].maxIndex);
//		sprintf(str, "%3.2f", mph); // da test nhung gia tri khong cap nhat, khong hieu cho nay
//		UARTprintf(str);
#endif
		break;
	case DISPLAY_KMH:
#ifdef DEBUG_UART
		UARTprintf("KMH = %d.%3d \t\t MaxFreq Index = %d\n", (uint16_t) kmh, k,
				fftNode[0].maxIndex);
#endif
		break;
	case DISPLAY_MS:

		break;
	case DISPLAY_HZ:

		break;
	default:
		break;
	}
}
/*
 *
 * */
uint8_t static MenuGetDisplayMode(typeSpeedMode mode) {
	return mode;
}
/*
 *
 * */
void PeripheralPrintf(uint16_t peripheral, char *format, ...) {
	char buffer[64] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	switch (peripheral) {
	case PERIPHERAL_LCD:
		break;
	case PERIPHERAL_UART:
		break;
	default:
		break;
	}
}
