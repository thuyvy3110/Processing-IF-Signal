//*****************************************************************************
//
// touch.c - Touch screen driver for the DK-TM4C123GXL board.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup touch_api
//! @{
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_adc.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"

#include "touch.h"
#include "lcd_ssd2119_8bit.h"
#include "gui.h"
//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide, and the flex connector is
//              on the left of the display.  This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall, and the flex connector is
//               on the bottom of the display.  This is selected by defining
//               LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide, and the flex
//                   connector is on the right of the display.  This is
//                   selected by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall, and the flex
//                    connector is on the top of the display.  This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if portrait mode is
// 0 degrees of screen rotation, landscape is 90 degrees of counter-clockwise
// rotation, portrait flip is 180 degrees of rotation, and landscape flip is
// 270 degress of counter-clockwise rotation.
//
// If no screen orientation is selected, "landscape flip" mode will be used.
//
//*****************************************************************************
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define LANDSCAPE
//#define PORTRAIT_FLIP
#endif

//*****************************************************************************
//
// The GPIO pins to which the touch screen is connected.
//
//*****************************************************************************
#define TS_P_PERIPH             SYSCTL_PERIPH_GPIOD
#define TS_P_BASE               GPIO_PORTD_BASE
#define TS_YP_PIN               GPIO_PIN_0//U
#define TS_XP_PIN               GPIO_PIN_1//R

#define TS_XN_PERIPH             SYSCTL_PERIPH_GPIOF
#define TS_XN_BASE               GPIO_PORTF_BASE
#define TS_XN_PIN               GPIO_PIN_4//L

#define TS_YN_PERIPH             SYSCTL_PERIPH_GPIOA
#define TS_YN_BASE               GPIO_PORTA_BASE
#define TS_YN_PIN               GPIO_PIN_2//D

//*****************************************************************************
//
// The ADC channels connected to each of the touch screen contacts.
//
//*****************************************************************************
#define ADC_CTL_CH_YP ADC_CTL_CH7
#define ADC_CTL_CH_XP ADC_CTL_CH6

//*****************************************************************************
//
// The coefficients used to convert from the ADC touch screen readings to the
// screen pixel positions.
//
//*****************************************************************************
#define NUM_TOUCH_PARAM_SETS 2
#define NUM_TOUCH_PARAMS     7

#define SET_NORMAL           0
#define SET_SRAM_FLASH       1

//*****************************************************************************
//
// Touchscreen calibration parameters.  We store several sets since different
// LCD configurations require different calibration.  Screen orientation is a
// build time selection but the calibration set used is determined at runtime
// based on the hardware configuration.
//
//*****************************************************************************
const int32_t g_lTouchParameters[NUM_TOUCH_PARAM_SETS][NUM_TOUCH_PARAMS] = {
//
// Touchscreen calibration parameters for use when no LCD-controlling
// daughter board is attached.
//
		{
#ifdef PORTRAIT
				-4320,          // M0
				187008,// M1
				-175805280,// M2
				290528,// M3
				-14208,// M4
				-129632480,// M5
				1605992,// M6
#endif
#ifdef LANDSCAPE
				307456,	// M0
				-6528,	// M1
				-175440160,	// M2
				1464,		// M3
				-308736,		// M4
				992064864,	// M5
				2808747,		// M6
#endif
#ifdef PORTRAIT_FLIP
		1152,           // M0
		-194880,// M1
		586541664,// M2
		-291584,// M3
		29984,// M4
		625271584,// M5
		1680556,// M6
#endif
#ifdef LANDSCAPE_FLIP
		-291200,         // M0
		18048,// M1
		644276768,// M2
		2712,// M3
		191856,// M4
		-187423752,// M5
		1654753,// M6
#endif
	},
	//
	// Touchscreen calibration parameters for use when the SRAM/Flash daughter
	// board is attached.
	//
		{
#ifdef PORTRAIT
				-449664,          // M0
				187008,// M1
				63226752,// M2
				-751424,// M3
				-14912,// M4
				377755392,// M5
				-4356880,// M6
#endif
#ifdef LANDSCAPE
				303616,         // M0
				-6912,           // M1
				-171108160,       // M2
				5928,          // M3
				-307128,         // M4
				973099392,       // M5
				2758286,         // M6
#endif
#ifdef PORTRAIT_FLIP
		2496,           // M0
		-94368,// M1
		74406768,// M2
		-104000,// M3
		-1600,// M4
		100059200,// M5
		290550,// M6
#endif
#ifdef LANDSCAPE_FLIP
		-289920,        // M0
		17280,// M1
		638897760,// M2
		1488,// M3
		191112,// M4
		-185258712,// M5
		1640415,// M6
#endif
	}, };

//*****************************************************************************
//
// A pointer to the current touchscreen calibration parameter set.
//
//*****************************************************************************
const int32_t *g_plParmSet;

//*****************************************************************************
//
// The minimum raw reading that should be considered valid press.
//
//*****************************************************************************
short g_sTouchMin = TOUCH_MIN;

//*****************************************************************************
//
// The current state of the touch screen driver's state machine.  This is used
// to cycle the touch screen interface through the powering sequence required
// to read the two axes of the surface.
//
//*****************************************************************************
static uint32_t g_ulTSState;
#define TS_STATE_INIT           0
#define TS_STATE_READ_X         1
#define TS_STATE_READ_Y         2
#define TS_STATE_SKIP_X         3
#define TS_STATE_SKIP_Y         4

//*****************************************************************************
//
// The most recent raw ADC reading for the X position on the screen.  This
// value is not affected by the selected screen orientation.
//
//*****************************************************************************
volatile short g_sTouchX;

//*****************************************************************************
//
// The most recent raw ADC reading for the Y position on the screen.  This
// value is not affected by the selected screen orientation.
//
//*****************************************************************************
volatile short g_sTouchY;

//*****************************************************************************
//
// A pointer to the function to receive messages from the touch screen driver
// when events occur on the touch screen (debounced presses, movement while
// pressed, and debounced releases).
//
//*****************************************************************************
static int32_t (*g_pfnTSHandler)(uint32_t ulMessage, int32_t lX, int32_t lY);

//*****************************************************************************
//
// The current state of the touch screen debouncer.  When zero, the pen is up.
// When three, the pen is down.  When one or two, the pen is transitioning from
// one state to the other.
//
//*****************************************************************************
static unsigned char g_cState = 0;

//*****************************************************************************
//
// The queue of debounced pen positions.  This is used to slightly delay the
// returned pen positions, so that the pen positions that occur while the pen
// is being raised are not send to the application.
//
//*****************************************************************************
static short g_psSamples[8];

//*****************************************************************************
//
// The count of pen positions in g_psSamples.  When negative, the buffer is
// being pre-filled as a result of a detected pen down event.
//
//*****************************************************************************
static signed char g_cIndex = 0;

void Timer1AIntHandler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	ADCProcessorTrigger(ADC1_BASE, 3);
}

//*****************************************************************************
//
//! Debounces presses of the touch screen.
//!
//! This function is called when a new X/Y sample pair has been captured in
//! order to perform debouncing of the touch screen.
//!
//! \return None.
//
//*****************************************************************************
static void TouchScreenDebouncer(void) {
	int32_t lX, lY, lTemp;

	//
	// Convert the ADC readings into pixel values on the screen.
	//
	lX = g_sTouchX;
	lY = g_sTouchY;
	lTemp = (((lX * g_plParmSet[0]) + (lY * g_plParmSet[1]) + g_plParmSet[2])
			/ g_plParmSet[6]);
	lY = (((lX * g_plParmSet[3]) + (lY * g_plParmSet[4]) + g_plParmSet[5])
			/ g_plParmSet[6]);
	lX = lTemp;

	//
	// See if the touch screen is being touched.
	//
	if ((g_sTouchX < g_sTouchMin) || (g_sTouchY < g_sTouchMin)) {
		//
		// See if the pen is not up right now.
		//
		if (g_cState != 0x00) {
			//
			// Decrement the state count.
			//
			g_cState--;

			//
			// See if the pen has been detected as up three times in a row.
			//
			if (g_cState == 0x80) {
				//
				// Indicate that the pen is up.
				//
				g_cState = 0x00;

				//
				// See if there is a touch screen event handler.
				//
				if (g_pfnTSHandler) {
					//
					// Send the pen up message to the touch screen event
					// handler.
					//
					g_pfnTSHandler(WIDGET_MSG_PTR_UP, g_psSamples[g_cIndex],
							g_psSamples[g_cIndex + 1]);
				}
			}
		}
	} else {
		//
		// See if the pen is not down right now.
		//
		if (g_cState != 0x83) {
			//
			// Increment the state count.
			//
			g_cState++;

			//
			// See if the pen has been detected as down three times in a row.
			//
			if (g_cState == 0x03) {
				//
				// Indicate that the pen is up.
				//
				g_cState = 0x83;

				//
				// Set the index to -8, so that the next 3 samples are stored
				// into the sample buffer before sending anything back to the
				// touch screen event handler.
				//
				g_cIndex = -8;

				//
				// Store this sample into the sample buffer.
				//
				g_psSamples[0] = lX;
				g_psSamples[1] = lY;
			}
		} else {
			//
			// See if the sample buffer pre-fill has completed.
			//
			if (g_cIndex == -2) {
				//
				// See if there is a touch screen event handler.
				//
				if (g_pfnTSHandler) {
					//
					// Send the pen down message to the touch screen event
					// handler.
					//
					g_pfnTSHandler(WIDGET_MSG_PTR_DOWN, g_psSamples[0],
							g_psSamples[1]);
				}

				//
				// Store this sample into the sample buffer.
				//
				g_psSamples[0] = lX;
				g_psSamples[1] = lY;

				//
				// Set the index to the next sample to send.
				//
				g_cIndex = 2;
			}

			//
			// Otherwise, see if the sample buffer pre-fill is in progress.
			//
			else if (g_cIndex < 0) {
				//
				// Store this sample into the sample buffer.
				//
				g_psSamples[g_cIndex + 10] = lX;
				g_psSamples[g_cIndex + 11] = lY;

				//
				// Increment the index.
				//
				g_cIndex += 2;
			}

			//
			// Otherwise, the sample buffer is full.
			//
			else {
				//
				// See if there is a touch screen event handler.
				//
				if (g_pfnTSHandler) {
					//
					// Send the pen move message to the touch screen event
					// handler.
					//
					g_pfnTSHandler(WIDGET_MSG_PTR_MOVE, g_psSamples[g_cIndex],
							g_psSamples[g_cIndex + 1]);
				}

				//
				// Store this sample into the sample buffer.
				//
				g_psSamples[g_cIndex] = lX;
				g_psSamples[g_cIndex + 1] = lY;

				//
				// Increment the index.
				//
				g_cIndex = (g_cIndex + 2) & 7;
			}
		}
	}
}

//*****************************************************************************
//
//! Handles the ADC interrupt for the touch screen.
//!
//! This function is called when the ADC sequence that samples the touch screen
//! has completed its acquisition.  The touch screen state machine is advanced
//! and the acquired ADC sample is processed appropriately.
//!
//! It is the responsibility of the application using the touch screen driver
//! to ensure that this function is installed in the interrupt vector table for
//! the ADC3 interrupt.
//!
//! \return None.
//
//*****************************************************************************
void TouchScreenIntHandler(void) {
	//
	// Clear the ADC sample sequence interrupt.
	//
	HWREG(ADC1_BASE + ADC_O_ISC) = 1 << 3;
//	ADCIntClear(ADC1_BASE, 3);
//	while (!ADCIntStatus(ADC1_BASE, 3, false)) {
//	}
	//
	// Determine what to do based on the current state of the state machine.
	//
	switch (g_ulTSState) {
	//
	// The new sample is an X axis sample that should be discarded.
	//
	case TS_STATE_SKIP_X: {
		//
		// Read and throw away the ADC sample.
		//
		HWREG(ADC1_BASE + ADC_O_SSFIFO3);

		//
		// Set the analog mode select for the YP pin.
		//
		HWREG(TS_P_BASE + GPIO_O_AMSEL) =
		HWREG(TS_P_BASE + GPIO_O_AMSEL) | TS_YP_PIN;

		//
		// Configure the Y axis touch layer pins as inputs.
		//
		HWREG(TS_P_BASE + GPIO_O_DIR) =
		HWREG(TS_P_BASE + GPIO_O_DIR) & ~TS_YP_PIN;
		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_YN_BASE + GPIO_O_DIR) =
			HWREG(TS_YN_BASE + GPIO_O_DIR) & ~TS_YN_PIN;
		}
		//else if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
		//{
		//    HWREGB(LCD_CONTROL_SET_REG) = LCD_CONTROL_YN;
		//}

		//
		// The next sample will be a valid X axis sample.
		//
		g_ulTSState = TS_STATE_READ_X;

		//
		// This state has been handled.
		//
		break;
	}

		//
		// The new sample is an X axis sample that should be processed.
		//
	case TS_STATE_READ_X: {
		//
		// Read the raw ADC sample.
		//
		g_sTouchX = HWREG(ADC1_BASE + ADC_O_SSFIFO3);
		//
		// Clear the analog mode select for the YP pin.
		//
		HWREG(TS_P_BASE + GPIO_O_AMSEL) =
		HWREG(TS_P_BASE + GPIO_O_AMSEL) & ~TS_YP_PIN;

		//
		// Configure the X and Y axis touch layers as outputs.
		//
		HWREG(TS_P_BASE + GPIO_O_DIR) =
		HWREG(TS_P_BASE + GPIO_O_DIR) | TS_XP_PIN | TS_YP_PIN;

		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_YN_BASE + GPIO_O_DIR) =
			HWREG(TS_YN_BASE + GPIO_O_DIR) | TS_XN_PIN | TS_YN_PIN;
		}

		//
		// Drive the positive side of the Y axis touch layer with VDD and
		// the negative side with GND.  Also, drive both sides of the X
		// axis layer with GND to discharge any residual voltage (so that
		// a no-touch condition can be properly detected).
		//
		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_XN_BASE + GPIO_O_DATA +
					((TS_XN_PIN) << 2)) = 0;
			HWREG(TS_YN_BASE + GPIO_O_DATA +
					((TS_YN_PIN) << 2)) = 0;
		}
		//else if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
		//{
		//    HWREGB(LCD_CONTROL_CLR_REG) = LCD_CONTROL_XN | LCD_CONTROL_YN;
		//}
		HWREG(TS_P_BASE + GPIO_O_DATA + ((TS_XP_PIN | TS_YP_PIN) << 2)) =
		TS_YP_PIN;

		//
		// Configure the sample sequence to capture the X axis value.
		//
		HWREG(ADC1_BASE + ADC_O_SSMUX3) = ADC_CTL_CH_XP;

		//
		// The next sample will be an invalid Y axis sample.
		//
		g_ulTSState = TS_STATE_SKIP_Y;

		//
		// This state has been handled.
		//
		break;
	}

		//
		// The new sample is a Y axis sample that should be discarded.
		//
	case TS_STATE_SKIP_Y: {
		//
		// Read and throw away the ADC sample.
		//
		HWREG(ADC1_BASE + ADC_O_SSFIFO3);

		//
		// Set the analog mode select for the XP pin.
		//
		HWREG(TS_P_BASE + GPIO_O_AMSEL) =
		HWREG(TS_P_BASE + GPIO_O_AMSEL) | TS_XP_PIN;

		//
		// Configure the X axis touch layer pins as inputs.
		//
		HWREG(TS_P_BASE + GPIO_O_DIR) =
		HWREG(TS_P_BASE + GPIO_O_DIR) & ~TS_XP_PIN;
		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_XN_BASE + GPIO_O_DIR) =
			HWREG(TS_XN_BASE + GPIO_O_DIR) & ~TS_XN_PIN;
		}
		//else if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
		//{
		//    HWREGB(LCD_CONTROL_SET_REG) = LCD_CONTROL_XN;
		//}

		//
		// The next sample will be a valid Y axis sample.
		//
		g_ulTSState = TS_STATE_READ_Y;

		//
		// This state has been handled.
		//
		break;
	}

		//
		// The new sample is a Y axis sample that should be processed.
		//
	case TS_STATE_READ_Y: {
		//
		// Read the raw ADC sample.
		//
		g_sTouchY = HWREG(ADC1_BASE + ADC_O_SSFIFO3);

		//
		// The next configuration is the same as the initial configuration.
		// Therefore, fall through into the initialization state to avoid
		// duplicating the code.
		//
	}

		//
		// The state machine is in its initial state
		//
	case TS_STATE_INIT: {
		//
		// Clear the analog mode select for the XP pin.
		//
		HWREG(TS_P_BASE + GPIO_O_AMSEL) =
		HWREG(TS_P_BASE + GPIO_O_AMSEL) & ~TS_XP_PIN;

		//
		// Configure the X and Y axis touch layers as outputs.
		//
		HWREG(TS_P_BASE + GPIO_O_DIR) =
		HWREG(TS_P_BASE + GPIO_O_DIR) | TS_XP_PIN | TS_YP_PIN;
		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_XN_BASE + GPIO_O_DIR) =
			HWREG(TS_XN_BASE + GPIO_O_DIR) | TS_XN_PIN;
			HWREG(TS_YN_BASE + GPIO_O_DIR) =
			HWREG(TS_YN_BASE + GPIO_O_DIR) | TS_YN_PIN;
		}

		//
		// Drive one side of the X axis touch layer with VDD and the other
		// with GND.  Also, drive both sides of the Y axis layer with GND
		// to discharge any residual voltage (so that a no-touch condition
		// can be properly detected).
		//
		HWREG(TS_P_BASE + GPIO_O_DATA + ((TS_XP_PIN | TS_YP_PIN) << 2)) =
		TS_XP_PIN;
		//if(g_eDaughterType == DAUGHTER_NONE)
		{
			HWREG(TS_XN_BASE + GPIO_O_DATA +
					((TS_XN_PIN) << 2)) = 0;
			HWREG(TS_YN_BASE + GPIO_O_DATA +
					((TS_YN_PIN) << 2)) = 0;
		}
		//else if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
		//{
		//    HWREGB(LCD_CONTROL_CLR_REG) = LCD_CONTROL_XN | LCD_CONTROL_YN;
		//}
		//
		// Configure the sample sequence to capture the Y axis value.
		//
		HWREG(ADC1_BASE + ADC_O_SSMUX3) = ADC_CTL_CH_YP;

		//
		// If this is the valid Y sample state, then there is a new X/Y
		// sample pair.  In that case, run the touch screen debouncer.
		//
		if (g_ulTSState == TS_STATE_READ_Y) {
			TouchScreenDebouncer();
		}

		//
		// The next sample will be an invalid X axis sample.
		//
		g_ulTSState = TS_STATE_SKIP_X;

		//
		// This state has been handled.
		//
		break;
	}
	}
}

//*****************************************************************************
//
//! Initializes the touch screen driver.
//!
//! This function initializes the touch screen driver, beginning the process of
//! reading from the touch screen.  This driver uses the following hardware
//! resources:
//!
//! - ADC sample sequence 3
//! - Timer 1 subtimer A
//!
//! \return None.
//
//*****************************************************************************
void TouchScreenInit(void) {
	//
	// Set the initial state of the touch screen driver's state machine.
	//
	g_ulTSState = TS_STATE_INIT;

	//
	// Determine which calibration parameter set we will be using.
	//
	g_plParmSet = g_lTouchParameters[SET_NORMAL];
	//if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
	{
		//
		// If the SRAM/Flash daughter board is present, select the appropriate
		// calibration parameters and reading threshold value.
		//
		//    g_plParmSet = g_lTouchParameters[SET_SRAM_FLASH];
		//    g_sTouchMin = 40;
	}

	//
	// There is no touch screen handler initially.
	//
	g_pfnTSHandler = 0;

	//
	// Enable the peripherals used by the touch screen interface.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(TS_P_PERIPH);
	SysCtlPeripheralEnable(TS_XN_PERIPH);
	SysCtlPeripheralEnable(TS_YN_PERIPH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	//
	// Configure the ADC sample sequence used to read the touch screen reading.
	//
	ADCHardwareOversampleConfigure(ADC1_BASE, 4);
	//ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_TIMER, 0);
	ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE, 3, 0,
	ADC_CTL_CH_YP | ADC_CTL_END | ADC_CTL_IE);
	ADCSequenceEnable(ADC1_BASE, 3);

	//
	// Enable the ADC sample sequence interrupt.
	//
	ADCIntEnable(ADC1_BASE, 3);
	IntEnable(INT_ADC1SS3);

	//
	// Configure the GPIOs used to drive the touch screen layers.
	//
	GPIOPinTypeGPIOOutput(TS_P_BASE, TS_XP_PIN | TS_YP_PIN);

	//
	// If no daughter board is installed, set up GPIOs to drive the XN and YN
	// signals.
	//
	//if(g_eDaughterType == DAUGHTER_NONE)
	{
		GPIOPinTypeGPIOOutput(TS_XN_BASE, TS_XN_PIN);
		GPIOPinTypeGPIOOutput(TS_YN_BASE, TS_YN_PIN);
	}

	GPIOPinWrite(TS_P_BASE, TS_XP_PIN | TS_YP_PIN, 0x00);
	//if(g_eDaughterType == DAUGHTER_NONE)
	{
		GPIOPinWrite(TS_XN_BASE, TS_XN_PIN, 0x00);
		GPIOPinWrite(TS_YN_BASE, TS_YN_PIN, 0x00);
	}
	//else if(g_eDaughterType == DAUGHTER_SRAM_FLASH)
	{
		//    HWREGB(LCD_CONTROL_CLR_REG) = LCD_CONTROL_XN | LCD_CONTROL_YN;
	}
	//
	// See if the ADC trigger timer has been configured, and configure it only
	// if it has not been configured yet.
	//
	if ((HWREG(TIMER1_BASE + TIMER_O_CTL) & TIMER_CTL_TAEN) == 0) {
		//
		// Configure the timer to trigger the sampling of the touch screen
		// every millisecond.
		//
		TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR |
		TIMER_CFG_A_PERIODIC |
		TIMER_CFG_B_PERIODIC));
		TimerLoadSet(TIMER1_BASE, TIMER_A, (SysCtlClockGet() / 1000) - 1);
		//TimerLoadSet(TIMER1_BASE, TIMER_A, (SysCtlClockGet() / 4) - 1);
		//TimerControlTrigger(TIMER1_BASE, TIMER_A, true);
		//
		//JTW: We can't have multiple timers trigger different ADC peripherals,
		// so I am changing this ADC to be software triggered via the timer
		// interrupt handler.
		//
		IntEnable(INT_TIMER1A);
		TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		//
		// Enable the timer.  At this point, the touch screen state machine
		// will sample and run once per millisecond.
		//
		TimerEnable(TIMER1_BASE, TIMER_A);
	}
}

//*****************************************************************************
//
//! Sets the callback function for touch screen events.
//!
//! \param pfnCallback is a pointer to the function to be called when touch
//! screen events occur.
//!
//! This function sets the address of the function to be called when touch
//! screen events occur.  The events that are recognized are the screen being
//! touched (``pen down''), the touch position moving while the screen is
//! touched (``pen move''), and the screen no longer being touched (``pen
//! up'').
//!
//! \return None.
//
//*****************************************************************************
void TouchScreenCallbackSet(
		int32_t (*pfnCallback)(uint32_t ulMessage, int32_t lX, int32_t lY)) {
	//
	// Save the pointer to the callback function.
	//
	g_pfnTSHandler = pfnCallback;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
