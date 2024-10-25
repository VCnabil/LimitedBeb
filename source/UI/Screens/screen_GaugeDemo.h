//------------------------------------------------------------------------------
//  TITLE :          screen_GaugeDemo.h
//  DESCRIPTION :    Demo Screen to show hwo to setup/Draw a Gauge
//------------------------------------------------------------------------------
#ifndef __SCREEN_DEMO_GAUGE_H__
#define __SCREEN_DEMO_GAUGE_H__

#include "project.h"

/// @addtogroup screens 
/// @{

/// Called once when the screen is entered.
void Screen_GaugeDemo_Enter(void);

/// Called to redraw the entire screen.
void Screen_GaugeDemo_Create(void);

/// Called every frame.
void Screen_GaugeDemo_Update(void);

/// Called when exiting a screen.
void Screen_GaugeDemo_Exit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREEN_DEMO_GAUGE_H__
