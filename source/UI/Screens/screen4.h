//------------------------------------------------------------------------------
//  TITLE :          screen3.h
//  DESCRIPTION :    Screen 3 drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREEN4_H__
#define __SCREEN4_H__

#include "project.h"

/// @addtogroup screens
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void Screen4Enter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void Screen4Create(void);

/// Drawing Function. 
/// Called every frame.
void Screen4Update(void);

/// Called when exiting a screen.
void Screen4Exit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREEN3_H__
