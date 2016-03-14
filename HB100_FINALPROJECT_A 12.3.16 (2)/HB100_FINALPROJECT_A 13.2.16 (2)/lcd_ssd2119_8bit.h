//*****************************************************************************
//
// Kentec320x240x16_ssd2119_8bit.h - Prototypes for the Kentec K350QVG-V2-F
//                                     display driver with an SSD2119
//                                     controller.
//*****************************************************************************

#ifndef __KENTEC320X240X16_SSD2119_8BIT_H__
#define __KENTEC320X240X16_SSD2119_8BIT_H__

//	LCD_SCLK: PB4/SSI2Clk
//	LCD_SDA:	PB7/SSI2Tx

//#define SPI4	//8-bit 4-wire SPI mode (SCLK, SDA, SCS, SDC)
//#define SPI3	//9-bit 3-wire SPI mode (SCLK, SDA, SCS)
extern void LED_backlight_ON(void);
extern void LED_backlight_OFF(void);
extern void LCD_SSD2119Init(void);
extern const tDisplay g_sLCD_SSD2119;
#endif // __KENTEC320X240X16_SSD2119_H__
