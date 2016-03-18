//*****************************************************************************
//
// gui.h - Predefines, public functions, and globals for the graphical user
// interface portion of the code.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
//
//*****************************************************************************

#ifndef __GUI_H__
#define __GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#define REFRESH_RATE			18

static void OnButtonPress(tWidget *pWidget);
static void OnConfigPress(tWidget *pWidget);
void InitGUI(void);
void UpdateGConfigs(void);
void GUIUpdateDisplay(void);

#endif // __GUI_H__
