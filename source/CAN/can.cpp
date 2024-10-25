//------------------------------------------------------------------------------
//  TITLE :          can.c
//  DESCRIPTION :    CAN bus functions.
//------------------------------------------------------------------------------

#include "project.h"

//------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// LOCAL FUNCTION PROTOTYPES
//------------------------------------------------------------------------------
static void CANDecodeRxStd(CAN_PORTS_T canPort, CAN_MSG_T* pMsg);
static void CANDecodeRxExt(CAN_PORTS_T canPort, CAN_MSG_T* pMsg);

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// CANInit
//
// Initialise the CAN ports
//------------------------------------------------------------------------------
void CANInit(void)
{
    // Set speed to 250k
    can_bitrate_set(CAN_PORT1, 250000);
    // Open CAN port 1
    can_open(CAN_PORT1);

    // Set speed to 250k
    can_bitrate_set(CAN_PORT2, 250000);
    // Open CAN port 2
    can_open(CAN_PORT2);
}

//------------------------------------------------------------------------------
// CANDeInit
//
// De-initialise the CAN ports
//------------------------------------------------------------------------------
void CANDeInit(void)
{
}

//------------------------------------------------------------------------------
// CANProcessRx
//
// Process any messages received on the CAN ports
//------------------------------------------------------------------------------
void CANProcessRx(void)
{
    // Check both ports
    uint32_t canPort;
    for (canPort = CAN_PORT1; canPort < CAN_MAX_PORTS; canPort++)
    {
        // Check for CAN messages received
        CAN_MSG_T canMsg;
        while (can_receive_message((CAN_PORTS_T)canPort, &canMsg) == 0)
        {
            // Is it an extended CAN frame?
            if (canMsg.extend != 0)
            {
                // Yes. Decode with the extended function then
                CANDecodeRxExt((CAN_PORTS_T)canPort, &canMsg);
            }
            else
            {
                // No. Decode with the standard function then
                CANDecodeRxStd((CAN_PORTS_T)canPort, &canMsg);
            }
        }
    }
}

// Send a CAN message to the specified port
BOOL CANSend(CAN_PORTS_T canPort, uint32_t id, BOOL bExtended, uint8_t* pData, uint32_t dataLen)
{
    BOOL bRet = FALSE;
    CAN_MSG_T canMsg;

	// This function should not be used to transmit on a J1939 or NMEA2000 bus unless the device has a Source Address.
	// It can obtain a source address by doing an address claim, this is not done in this template project.
	// We recommend using CT2KLib if you are on a J1939 or NMEA2000 bus and doing anything more than listening.
	assert(FALSE && "See comment in CANSend function");

    if (dataLen <= CAN_CONTENT_MAX_LENGTH)
    {
        uint32_t i;

        // Create the CAN frame for the message
        canMsg.extend = (bExtended != FALSE) ? 1 : 0;
        canMsg.request_remote = 0;
        canMsg.id = id;
        canMsg.msg_length = (uint8_t)dataLen;
        // copy data
        for (i = 0; i < dataLen; i++)
        {
            canMsg.msg_content[i] = pData[i];
        }

        // Send message
        if (can_transmit_message(canPort, &canMsg) == 0)
        {
            // Success
            bRet = TRUE;
        }
    }

    return bRet;
}

//------------------------------------------------------------------------------
// LOCAL FUNCTIONS
//------------------------------------------------------------------------------

// Decode standard (11-bit) CAN messages
static void CANDecodeRxStd(CAN_PORTS_T canPort,
                           CAN_MSG_T* pMsg)
{
	// Which Id was it?
	switch (pMsg->id)
	{
	case 0x7FF:
		// Do your frame decoding here...
		break;

	default:
		break;
	}
}

// Decode extended (29-bit) CAN messages
static void CANDecodeRxExt(CAN_PORTS_T canPort,
                           CAN_MSG_T* pMsg)
{
	// Which PGN was it?
	switch (CANIDEXT_GETPGN(pMsg->id))
	{
	case 0xFEE5:
		J1939_PGN_FEE5_HOURS(canPort, pMsg);
		break;

	default:
		break;
	}
}
