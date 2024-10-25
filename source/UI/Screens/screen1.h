//------------------------------------------------------------------------------
//  TITLE :          screen1.h
//  DESCRIPTION :    Screen 1 drawing functions.
//------------------------------------------------------------------------------
#ifndef __SCREEN1_H__
#define __SCREEN1_H__

#include "project.h"

extern int passcodeScreen;

/// @addtogroup screens
/// @{

/// Called once when the screen is entered.
/// Clears the Screen, sets the Keys for the screen.
void Screen1Enter(void);

/// Called to redraw the entire screen.
/// Clears the Screen, sets the Keys for the screen.
void Screen1Create(void);

/// Drawing Function. 
/// Called every frame.
void Screen1Update(void);

/// Called when exiting a screen.
void Screen1Exit(void);

/// @} // endgroup screens

#endif // #ifndef __SCREEN1_H__
