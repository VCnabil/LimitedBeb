//------------------------------------------------------------------------------
//  TITLE :          lightning.h
//  DESCRIPTION :    Handles Screen backlight and keys light.
//------------------------------------------------------------------------------

#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "project.h"

/// @defgroup lighting Back and Keys Light Functions
/// Functions for controlling the Backlight and Key lights
/// @{

/// Inits any Backlight or Keypad related stuff.
void LightingInit(void);

/// Shows the menu for the backlight and keylight settings
void LightingEnter(void);

/// @} // endgroup lighting

#endif