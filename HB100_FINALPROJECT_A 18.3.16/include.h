/*
 * include.h
 *
 *  Created on: Nov 21, 2015
 *      Author: Thuy Vy
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include "arm_math.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/fpu.h"
#include "utils/uartstdio.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"

#include "lcd_ssd2119_8bit.h"
#include "touch.h"

#include "utils/uartstdio.h"

#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "inc/hw_pwm.h"
#include "inc/hw_adc.h"
//#include "define.h"
#include "sampling.h"
#include "buffer.h"
//#include "Bluetooth/uartstdio.h"
//#include "SystemConfig.h"

//#include "IR/IR.h"
#include "rtwtypes.h"
//#include "Str/Uocluong.h"
//#include "Str/STR_Indirect.h"
//#include "Str/Control_initialize.h"
//#include "Bluetooth/ustdlib.h"
//#include "Bluetooth/Bluetooth.h"
//#include "QEI/qei.h"
//#include "Button/Button.h"
//#include "Str/speed_control.h"
//#include "WallFollow/PID.h"
//#include "WallFollow/WallFollow.h"
//#include "Buzzer/Buzzer.h"
//#include "Battery/Battery.h"
//#include "HostComm/HostComm.h"
#include "signal_dsp.h"
#include "FFT_signal.h"
/*
 * truong hop dung FFT2_ van tam chinh xac nhung can test lai lan nua
 * gia tri maxFFTIndex vot len toi 510 la sai
 * chua do ro gia tri maxFFTIndex nho nhat la bao nhieu
 *
 * truong hop dung FFT1_ con gap loi khi touch len LCD thi bi nhay vao uDMA ERROR HANDLER
 * */
//#define FFT1_
//#define USE_TEMPORARY_BUFFER

#define fix_minFFTIndex	10
#define fix_maxFFTIndex	127

//#define USING_BUFFER2
#define DEBUG_UART

#endif /* INCLUDE_H_ */
