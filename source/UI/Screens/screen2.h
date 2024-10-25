//------------------------------------------------------------------------------
//  TITLE :          screen2.h
//  DESCRIPTION :    Screen 2 drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREEN2_H__
#define __SCREEN2_H__

#include "project.h"

/// @addtogroup screens 
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void Screen2Enter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void Screen2Create(void);

/// Drawing Function. 
/// Called every frame.
void Screen2Update(void);

/// Called when exiting a screen.
void Screen2Exit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREEN2_H__
