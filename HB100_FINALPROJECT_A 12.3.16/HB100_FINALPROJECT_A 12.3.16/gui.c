#include "include.h"
#include "gui.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/slider.h"
#include "sampling.h"

extern tCanvasWidget g_sBackground;
extern tPushButtonWidget g_sPushBtn;
extern volatile unsigned short int countTimerISRLoopSample;
//static test_button(tWidget *pWidget);
//-------------------------------------------------------------------------------------------//
// Flag used to inform the system that a screen refresh needs to happen as soon
// as sanely possible
//
volatile unsigned short int g_ucDispRefresh = 0;
unsigned short int g_ucCfgDisplay = 0;
//-------------------------------------------------------------------------------------------//

Canvas(g_sHeading2, &g_sBackground, 0, 0, &g_sLCD_SSD2119, 0, 200, 320, 40,
		(CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
		ClrMediumVioletRed, ClrLightYellow, ClrWhite, g_psFontCm20,
		"Hello DnThuyVy", 0, 0);
Canvas(g_sHeading, &g_sBackground, &g_sHeading2, &g_sPushBtn, &g_sLCD_SSD2119,
		0, 0, 320, 23,
		(CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
		ClrOrange, ClrLightYellow, ClrBlack, g_psFontCm20, "Led demo", 0, 0);
Canvas(g_sBackground, WIDGET_ROOT, 0, &g_sHeading, &g_sLCD_SSD2119, 0, 23, 320,
		(200 - 23), CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);
RectangularButton(g_sPushBtn, &g_sHeading, 0, 0, &g_sLCD_SSD2119, 60, 60, 200,
		40,
		(PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_STYLE_FILL),
		ClrChocolate, ClrMediumVioletRed, ClrLightYellow, ClrGreenMask,
		g_psFontCmss22b, "Toggle green led", 0, 0, 0, 0, OnButtonPress);

//bool g_GreenLedOn = false;
int Timer3Count = 0;
bool ButtonTF = false;
//-----------------------------------------------------------------------------------------------------------//
void Timer3AIntHandler(void) {
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	g_ucDispRefresh = 1;
	Timer3Count++;
	if (Timer3Count == 1000)
		Timer3Count = 0;
}
static void InitDisplayTimer() {
	//
	// Set up timer3A to be the display timer, interrupting at 15 Hz
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);
	TimerLoadSet(TIMER3_BASE, TIMER_A, SysCtlClockGet() / REFRESH_RATE);
	IntEnable(INT_TIMER3A);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER3_BASE, TIMER_A);
}
//-----------------------------------------------------------------------------------------------------------//

static void OnButtonPress(tWidget *pWidget) {
	ButtonTF = !ButtonTF;
	if (ButtonTF) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08);
	} else {
		//UpdateGConfigs();
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
	}
#if 0
	UpdateGConfigs();
	countTimerISRLoopSample = 0;
#endif
}

//*****************************************************************************
//
// Function handler for when the config button at the bottom is pressed
//
//*****************************************************************************
static void OnConfigPress(tWidget *pWidget) {
	if (g_ucCfgDisplay == 0) {
		g_ucDispRefresh = 0;
		TimerDisable(TIMER3_BASE, TIMER_A);
		g_ucCfgDisplay = 1;
	} else if (g_ucCfgDisplay == 1) {
		g_ucCfgDisplay = 0;
		g_ucDispRefresh = 2;
	}
}

//void BUTTON(tWidget *pWidgetR) {
//	ButtonTF = !ButtonTF;
//	if (ButtonTF) {
//
//		if (stop == 1) {
//			stopped = 1;
//		} else {
//			stop = 1;
//			stopped = 0;
//		}
//		WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sContainerAcquire);
//		WidgetPaint((tWidget * )&g_sContainerAcquire);
//	} else {
//		ClrMyWidget();
//		if (stopped == 1) {
//			stopped = 0;
//		} else {
//			stop = 0;
//			stopped = 0;
//		}
//	}
//
//}
void UpdateGConfigs(void) {
	InitSamplingTimer();
	InitDSP1();
#ifdef FFT2_
	InitDSP2();
#endif
}
void GUIUpdateDisplay(void) {
	if (g_ucDispRefresh) {
		if (g_ucDispRefresh == 2) {
			UpdateGConfigs();
		}
		g_ucDispRefresh = 0;
		TimerEnable(TIMER3_BASE, TIMER_A);
	}
}

void InitGUI(void) {
	InitDisplayTimer();
	LCD_SSD2119Init();
	/*-------------------------------------------------*/
	TouchScreenInit();
	TouchScreenCallbackSet(WidgetPointerMessage);

	WidgetPaint(WIDGET_ROOT);
	WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sBackground);
}
void ClrMyWidget() {
	WidgetRemove((tWidget *) &g_sBackground);
}
