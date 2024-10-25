//------------------------------------------------------------------------------
//  TITLE :          uartviewer.h
//  DESCRIPTION :    UART viewer.
//------------------------------------------------------------------------------
#ifndef __UARTVIEWER_H__
#define __UARTVIEWER_H__

#include "project.h"

/// @addtogroup screens
/// @{

/// Create the UART Viewer Screen
void UARTViewerCreate(void);

/// Populate the UART Viewer with data and do the actual drawing
void UARTViewerUpdate(void);

/// @} // endgroup screens

#endif // #ifndef __UARTVIEWER_H__
