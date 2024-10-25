//------------------------------------------------------------------------------
//  TITLE :          menu.h
//  DESCRIPTION :    Menu creation navigation and handling routines.
//------------------------------------------------------------------------------
#ifndef __MENU_H__
#define __MENU_H__

#include "project.h"

/// @defgroup menus Menu Functions
/// Functions for creating and controlling the Menus
/// @{

/// Callback function type
typedef void(*MENUCALLBACKFUNC_T)(void* userData);
typedef void(*MENUCREATEFUNC_T)(void* userData);

void MenuInit(void);

/// Handles the navigation between menus
void MenuScreenEnter(void);

/// Clear the screen, creates the title bar and draws the menu items
void MenuScreenCreate(void);

/// Function that is called when completely exiting a menu. 
void MenuScreenExit(void);

/// Create a new menu or sub-menu.
void MenuCreateNew(MENUCREATEFUNC_T pCreateFunc, void* userData);

/// Sets the menu title
void MenuSetTitle(const char* pTitleStr);

void MenuDrawTitleBar(const char * titleString);

uint32_t MenuTitleBarGetHeight(void);

/// Adds a menu item.
void MenuSetItem(uint32_t itemNumber, const char* pLableText, const char* pValueText, MENUCALLBACKFUNC_T pSelectedCB, void* userData);

void MenuSetupButtons(uint32_t itemNumber, const char* button3Text, MENUCALLBACKFUNC_T pbutton3CB, void* button3UserData, const char* button4Text, MENUCALLBACKFUNC_T pbutton4CB, void* button4UserData);

/// Reset the stack for next time and go to the screen from which we came from
void MenuQuit(void);

/// Function that handles "go back" navigation of a menu. If the current menu was invoked from another one go back to the previous menu, otherwise quit.	
void MenuBack(void* userData = nullptr);

/// @} // endgroup Menus

#endif // #ifndef __MENU_H__
