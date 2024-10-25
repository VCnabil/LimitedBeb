//------------------------------------------------------------------------------
//  TITLE :          screen3.h
//  DESCRIPTION :    Screen 3 drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREEN3_H__
#define __SCREEN3_H__

#include "project.h"

/// @addtogroup screens
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void Screen3Enter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void Screen3Create(void);

/// Drawing Function. 
/// Called every frame.
void Screen3Update(void);

/// Called when exiting a screen.
void Screen3Exit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREEN3_H__
