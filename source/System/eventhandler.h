//------------------------------------------------------------------------------
//  TITLE :          eventhandler.h
//  DESCRIPTION :    Event handling functions.
//------------------------------------------------------------------------------
#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include "project.h"

/// @defgroup eventhandle Event Handler Functions
/// Functions for controlling the Event Handler
/// @{

/// Initialise the Event system
void EventHandlerInit(void);

/// Call this from the main loop to process any events
void EventHandlerProcess(void);

/// @} // endgroup eventhandle

#endif // #ifndef __EVENT_HANDLER_H__
