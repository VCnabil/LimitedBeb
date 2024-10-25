//------------------------------------------------------------------------------
//  TITLE :          canviewer.h
//  DESCRIPTION :    CAN viewer.
//------------------------------------------------------------------------------
#ifndef __CANVIEWER_H__
#define __CANVIEWER_H__

#include "project.h"

/// @addtogroup screens
/// @{

/// Initialises the CANViewer
void CANViewerInit(void);

/// Function triggered on entering the CAN Viewer.
void CANViewerEnter(void);

/// Clears the Screen, sets the Keys for the screen.
void CANViewerCreate(void);

/// Drawing Function. Called per frame.
void CANViewerUpdate(void);

/// Log a received CAN message.
/// @param canPort - Specifies the CAN port
/// @param pMsg - Pointer to a CAN Message type
void CANViewerLogMessageReceive(CAN_PORTS_T canPort, CAN_MSG_T* pMsg);

/// Log a transmitted CAN message.
/// @param canPort - Specifies the CAN port
/// @param pMsg - Pointer to a CAN Message type
void CANViewerLogMessageTransmit(CAN_PORTS_T canPort, CAN_MSG_T* pMsg);

/// Rotate the log file. Called from the Event system - see eventhandler.c
void CANViewerLogRotate(void);

/// @} // endgroup screens

#endif // #ifndef __CANVIEWER_H__
