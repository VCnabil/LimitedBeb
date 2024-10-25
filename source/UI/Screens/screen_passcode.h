//------------------------------------------------------------------------------
//  TITLE :          screen_passcode.h
//  DESCRIPTION :    Passcode screen drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREENPASSCODE_H__
#define __SCREENPASSCODE_H__

#include "project.h"

/// @addtogroup screens
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void ScreenPasscodeEnter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void ScreenPasscodeCreate(void);

/// Drawing Function. 
/// Called every frame.
void ScreenPasscodeUpdate(void);

/// Called when exiting a screen.
void ScreenPasscodeExit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREENPASSCODE_H__
