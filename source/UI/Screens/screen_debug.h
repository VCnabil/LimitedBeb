//------------------------------------------------------------------------------
//  TITLE :          screen_debug.h
//  DESCRIPTION :    Debug drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREENDEBUG_H__
#define __SCREENDEBUG_H__

#include "project.h"

/// @addtogroup screens 
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void ScreenDebugEnter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void ScreenDebugCreate(void);

/// Drawing Function. 
/// Called every frame.
void ScreenDebugUpdate(void);

/// Called when exiting a screen.
void ScreenDebugExit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREENDEBUG_H__