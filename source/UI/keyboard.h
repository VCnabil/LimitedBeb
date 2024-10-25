//------------------------------------------------------------------------------
//  TITLE :          keyboard.h
//  DESCRIPTION :    Handles keyboard input
//------------------------------------------------------------------------------

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "project.h"
#include <string>

/// @defgroup keyboard Keyboard input
/// Function for keyboard input
/// @{

/// Inits any keyboard related stuff
void KeyboardInit(void);

/// Shows the keyboard
void KeyboardEnter(void);

/// Set the input string
void KeyboardSetInputString(const std::string& inputString);

/// Get the input string
const std::string& KeyboardGetInputString(void);

/// Set a confirm callback for the keyboard
void KeyboardSetConfirmCallback(void(*callback)(void*));

/// Set the min and max allowed length of the input string
/// Note that the maximum length is limited while typing
/// The minimum length is limited at confirm time
/// The default values of min and max length are 3 and 16
void KeyboardSetLengthLimits(const uint32_t minLength, const uint32_t maxLength);

/// @} // endgroup lighting

#endif
