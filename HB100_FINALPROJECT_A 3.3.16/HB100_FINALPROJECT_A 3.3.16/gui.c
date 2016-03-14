#include "include.h"
#include "gui.h"

extern tCanvasWidget g_sBackground;
extern tPushButtonWidget g_sPushBtn;

void OnButtonPress(tWidget *pWidget);

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

bool g_GreenLedOn = false;

void OnButtonPress(tWidget *pWidget) {
	g_GreenLedOn = !g_GreenLedOn;

	if (g_GreenLedOn) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08);
	} else {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
	}
}
