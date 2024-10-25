//----------------------------------------------------------------------------------
//  TITLE :          project.h
//  DESCRIPTION :    Single inclusion point of all header files used in the project.
//----------------------------------------------------------------------------------
#ifndef __PROJECT_H__
#define __PROJECT_H__

// Standard Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

// SDK Header files
#include "syslib.h"

// Software Version Number
#include "AppVersion.h"

#include "data.h"
#include "NVSettings/settings.h"

#include "Language/language.h"

#include "CAN/J1939/J1939_FEE5_HOURS.h"
#include "CAN/can.h"

#include "IO/iouser.h"
#include "System/eventhandler.h"
#include "System/sdk_init.h"
#include "System/lighting.h"
#include "System/ipAddress.h"

#include "UART/NMEA0183/extractField.h"
#include "UART/NMEA0183/VTG.h"
#include "UART/uart.h"
#include "UART/uart_util.h"

#include "Database/database.h"
#include "Database/unitconv.h"
#include "Database/serialinterpolate.h"
#include "Database/systemoptions.h"

#include "UI/ttfonts.h"
#include "UI/screens.h"
#include "UI/menu.h"
#include "UI/buttonbar.h"
#include "UI/keyboard.h"
#include "UI/graphs.h"

#include "UI/Menus/menu_diagnostic.h"
#include "UI/Menus/menu_home.h"
#include "UI/Menus/menu_language.h"
#include "UI/Menus/menu_settings.h"
#include "UI/Menus/menu_units.h"
#include "UI/Screens/screen1.h"
#include "UI/Screens/screen2.h"
#include "UI/Screens/screen3.h"
#include "UI/Screens/screen4.h"
#include "UI/Screens/screen_passcode.h"
#include "UI/Screens/screen_about.h"
#include "UI/Screens/screen_debug.h"

#endif // #ifndef __PROJECT_H__
