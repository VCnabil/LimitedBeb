//------------------------------------------------------------------------------
//  TITLE :          can.h
//  DESCRIPTION :    CAN bus functions
//------------------------------------------------------------------------------
#ifndef __CAN_H__
#define __CAN_H__

#include "project.h"

//------------------------------------------------------------------------------
// DEFINES/MACROS
//------------------------------------------------------------------------------

/// @defgroup can CAN Functions
/// Functions for controlling the CAN
/// @{

/// Decode an extended (29-bit) CAN Id to get the Source Address
#define CANIDEXT_GETSOURCEADDR(CANID)  (CANID & 0x000000FF)

/// Decode an extended (29-bit) CAN Id to get the PGN
#define CANIDEXT_GETPGN(CANID)         ((CANID >> 8) & 0x0000FFFF)

/// Decode an extended (29-bit) CAN Id to get the Data Page
#define CANIDEXT_GETDATAPAGE(CANID)    ((CANID >> 24) & 0x00000001)

/// Decode an extended (29-bit) CAN Id to get the Priority
#define CANIDEXT_GETPRIORITY(CANID)    ((CANID >> 26) & 0x00000007)

/// Create an extended (29-bit) CAN Id
#define CANIDEXT_CREATE(SOURCE, PGN, DATAPAGE, PRIORITY)  (((uint32_t)SOURCE & 0x000000FF) |         \
                                                           (((uint32_t)PGN & 0xFFFF) << 8) |        \
                                                           (((uint32_t)DATAPAGE & 0x01) << 24) |    \
                                                           (((uint32_t)PRIORITY & 0x07) << 26))
#define DATA_NOT_AVAILABLE                0xFF
#define DATA_ERROR_INDICATOR              0xFE

//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

/// Initialise the CAN ports
void CANInit(void);

/// De-initialise the CAN ports
void CANDeInit(void);

/// Process any received CAN messages
void CANProcessRx(void);

/// Send a CAN message to the specified port
/// @param canPort - The Can Port
/// @param id - The message id number
/// @param bExtended - is the message extended (TRUE/FALSE)
/// @param pData - Pointer to the data to send
/// @param dataLen - Length of the message
BOOL CANSend(CAN_PORTS_T canPort, uint32_t id, BOOL bExtended, uint8_t* pData, uint32_t dataLen);

/// @} // endgroup can

#endif // #ifndef __CAN_H__
