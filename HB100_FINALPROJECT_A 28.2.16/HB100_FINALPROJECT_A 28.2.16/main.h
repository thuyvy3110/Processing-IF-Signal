/*
 * main.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Thuy Vy
 */

#ifndef MAIN_H_
#define MAIN_H_

#define	RADAR_FREQUENCY	10525000000.0f	// 10.525GHz
#define	SPEED_OF_LIGHT	299792458.0f	//
#define KMH_FACTOR	(float32_t)3.6f		// 1 m/s in kmh
#define MPH_FACTOR	(float32_t)2.237f	// 1 m/s in mph

#define FFT1_
/*
 * truong hop dung FFT2_ van tam chinh xac nhung can test lai lan nua
 * gia tri maxFFTIndex vot len toi 510 la sai
 * chua do ro gia tri maxFFTIndex nho nhat la bao nhieu
 * */
//#define FFT2_
#define LEDRED		GPIO_PIN_1
#define	LEDGREEEN	GPIO_PIN_3
#define	LEDBLUE		GPIO_PIN_2

#ifdef USING_BUFFER2
extern uint32_t output2[NUM_SAMPLES];
#endif

//
enum {
	PERIPHERAL_LCD, // the built in LCD screen
	PERIPHERAL_UART
};

typedef enum typeSMode {
	DISPLAY_MPH = 0, DISPLAY_KMH, DISPLAY_MS, DISPLAY_HZ, DISPLAY_LAST
} typeSpeedMode;
typedef enum typeDMode {
	MODE_SPEED = 0, MODE_PEAK, MODE_WINDOW, MODE_FFT_LENGTH, MODE_LAST
} typeDisplayMode;

void delayMS(int ms);
void InitConsole(void);
void InitGPIO(void);
void PeripheralPrintf(uint16_t peripheral, char *format, ...);
void DisplayData(float32_t hertz);
uint8_t static MenuGetSpeedMode(typeSpeedMode);
uint8_t static MenuGetDisplayMode(typeDisplayMode);

#endif /* MAIN_H_ */
