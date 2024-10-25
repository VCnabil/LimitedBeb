//------------------------------------------------------------------------------
//  TITLE :          screens.h
//  DESCRIPTION :    Routines for screen creation and navigation between them.
//------------------------------------------------------------------------------
#ifndef __SCREENS_H__
#define __SCREENS_H__

#include "project.h"

/// @defgroup screens Screens Functions
/// Functions for handling the Screens.
/// @{

// Uncomment this to enable frames per second counter display
//#define ENABLE_FPS_COUNTER

/// Ids of all the screens within the application
typedef enum
{
/// Diagnostic UART Viewer Screen
 	SCREENID_UARTVIEWER,

/// Diagnostic CAN Viewer Screen
 	SCREENID_CANVIEWER,

	/// Diagnostic Database Viewer Screen
	SCREENID_DBVIEWER,

/// Main Menu Screen
 	SCREENID_MENU,

/// About Screen
	SCREENID_ABOUT,

/// Restore Drfaults Screen	
	SCREENID_RESTORE,

/// User screen 1
	SCREENID_1,

/// User screen 2
	SCREENID_2,

/// User screen 3
	SCREENID_3,

/// User screen 4
	SCREENID_4,

/// Passcode screen
	SCREENID_PASSCODE,

/// Debug screen
	SCREENID_DEBUG,

/// Gauge Demo Screen
	SCREENID_GaugeDemo,

/// Keyboard screen
	SCREENID_KEYBOARD,

/// Must be last!
	SCREENID_MAX
} SCREENID_T;

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

/// @brief Registers and initializes all the screens
void ScreensInit(void);

/// @brief Set the default key bind for all screens
void ScreensSetupDefaultKeys(void);

/// @} // endgroup screens

#endif // #ifndef __SCREENS_H__
