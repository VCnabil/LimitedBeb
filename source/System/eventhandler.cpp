
// Event Handling
#include "project.h"

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////
static void _EventHandlerCallback(EVENTID_T eventId);
static void _EventHandlerProcessEvent1(void);
static void _EventHandlerProcessEvent2(void);
static BOOL _EventHandlerGetCommand(char* pCommandMsg, uint32_t commandMsgMaxStrLen);
static void _EventHandlerSendReply(const char* pReplyMsg);

///////////////////////////////////////////////////////////////////////////////
// Private Variables and Constants
///////////////////////////////////////////////////////////////////////////////

// Have we received an event?
static BOOL m_bEventReceived = FALSE;
// Id of received event. 
static EVENTID_T m_eventId = EVENTID_EVENT1;

// Name of the event command file
static const char EVENT_COMMAND_FILENAME[] = "command";
// Name of the event reply file
static const char EVENT_REPLY_FILENAME[] = "reply";

// Event command list
static const char* pCmdCANViewerLogRotate = "CANVIEWER_LOGROTATE";

// Event reply list
static const char* pReplyOk = "OK";
static const char* pReplyNotOk = "KO";

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////
void EventHandlerInit(void)
{
	// Register with the platform for event callbacks
	event_register(_EventHandlerCallback);
}

void EventHandlerProcess(void)
{
    // Have we received and event?
    if (m_bEventReceived != FALSE)
    {
        // Clear the event
        m_bEventReceived = FALSE;

        // Which event was it?
        switch (m_eventId)
        {
        case EVENTID_EVENT1:
            _EventHandlerProcessEvent1();
            break;

        case EVENTID_EVENT2:
            _EventHandlerProcessEvent2();
            break;

        default:
            // Error
            assert("Invalid event code received!" && 0);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

// This is called by the system when an event is triggered
// EventId - The Id of the event.
static void _EventHandlerCallback(EVENTID_T eventId)
{
	// Save the event for later
	m_eventId = eventId;
    // and set the flag
    m_bEventReceived = TRUE;
}

static void _EventHandlerProcessEvent1(void)
{
    char cmd[1024];

    // Make sure there is no old reply left hanging around!
    FileRemove(FOLDERLOC_EVENT, EVENT_REPLY_FILENAME);

    // Get the command
    if (_EventHandlerGetCommand(cmd, sizeof(cmd) - 1) != FALSE)
    {
        // Is it a command to rotate log file?
        if (strncmp(cmd, pCmdCANViewerLogRotate, strlen(pCmdCANViewerLogRotate)) == 0)
        {

            // Reply to message
            _EventHandlerSendReply(pReplyOk);
        }
        else
        {
            // Error! Reply to message
            _EventHandlerSendReply(pReplyNotOk);
        }
    }

    // Remove the command file
    // This should ONLY be done once we have replied!
    FileRemove(FOLDERLOC_EVENT, EVENT_COMMAND_FILENAME);
}

static void _EventHandlerProcessEvent2(void)
{
    // Nothing to do here
}

static BOOL _EventHandlerGetCommand(char* pCommandMsg, 
                                    uint32_t commandMsgMaxStrLen)
{
    BOOL bRet = FALSE;
    FILE_T* pFile;

    // Make sure command is NUL terminated
    pCommandMsg[0] = '\0';

    // Open event command file
    pFile = FileOpen(FOLDERLOC_EVENT, EVENT_COMMAND_FILENAME, "r");
    if (pFile != 0)
    {
        int32_t msgSize;
        // Read message from file (if there is one)
        msgSize = FileRead(pFile, pCommandMsg, commandMsgMaxStrLen);
        // Handle -1 return
        if (msgSize < 0)
        {
            msgSize = 0;
        }
        // NUL terminate the message
        pCommandMsg[msgSize] = '\0';

        // Close command file
        FileClose(pFile);

        bRet = TRUE;
    }

    return bRet;
}

static void _EventHandlerSendReply(const char* pReplyMsg)
{
	FILE_T* pFile;

	// Open event reply file
	pFile = FileOpen(FOLDERLOC_EVENT, EVENT_REPLY_FILENAME, "w");
	if (pFile != 0)
	{
		// Is there a reply message?
		if (pReplyMsg != 0)
		{
			// Yep. write it to the file
			FileWrite(pFile, pReplyMsg, strlen(pReplyMsg));
		}

		// Close the file
		FileClose(pFile);
	}
}
