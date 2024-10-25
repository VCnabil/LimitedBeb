//------------------------------------------------------------------------------
//  TITLE :          uart.cpp
//  DESCRIPTION :    Serial Communications functions.
//------------------------------------------------------------------------------

#include "project.h"
#include <ctype.h>

#define IS_0183_SOF(c) (((c)=='$')||((c)=='!'))

typedef int(*DecodeFunc_t)(char* msgstring);

typedef struct
{
	const char *msgId;
	DecodeFunc_t decode;
} HandlerMapping_t;

//------------------------------------------------------------------------------
// LOCAL VARIABLES
//------------------------------------------------------------------------------

// Receive Buffer


static const HandlerMapping_t m_mapping[] = {
	{"VTG", NMEA0183_ProcessVTG},
};

static char m_RxMsgBuf[UART_MAX_MESSAGE_SIZE];
static uint8_t m_RxMsgIdx;

// Receive queue
static char m_MsgQueue[UART_MAX_MESSAGES][UART_MAX_MESSAGE_SIZE];
static MessageQueueInfo_t m_QueueInfo;
static UARTStats_t m_UartStats;
// Mutex to lock access to message queue
static MUTEXHANDLE_T m_queueLock;

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------
static void _UARTProcess(uint8_t* pBuffer, uint32_t bufLen);
static bool _DecodeNextMessage(void);

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

// Initialise the UART port
void UARTInit(void)
{
	memset(m_RxMsgBuf, 0, sizeof(m_RxMsgBuf));
	memset(m_MsgQueue, 0, sizeof(m_MsgQueue));
	memset(&m_QueueInfo, 0, sizeof(m_QueueInfo));
	memset(&m_UartStats, 0, sizeof(m_UartStats));
	m_RxMsgIdx = 0;
	m_QueueInfo.QueueSize = UART_MAX_MESSAGES;

	if (MutexCreate(&m_queueLock))
	{
		// Open the UART port with the correct settings.
		if (uart_open(uart_get_rs232_rs422_port(), UART_BAUDRATE_19200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_NONE, _UARTProcess))
		{
			uart_flush(uart_get_rs232_rs422_port(), TRUE, TRUE);
		}
	}
	else
	{
		assert(FALSE && "Failed to create mutex in UARTInit");
	}
}

// Send buffer pData of length dataLen over the UART port.
BOOL UARTSend(uint8_t *pData, uint32_t dataLen)
{
    return uart_write_buffer(uart_get_rs232_rs422_port(), pData, dataLen);
}

// Called from main loop to decode any messages received.
void UARTDecode(void)  
{
	// Have we received a message which hasn't been processed?
	while (_DecodeNextMessage())
	{
		// kick the watchdog so it doesn't reset the device
		watchdog_refresh();
	}
}

// Get the current uart stats
void GetUARTStats(UARTStats_t *stats)
{
	if (stats)
	{
		MutexLock(&m_queueLock);
		memcpy(stats, &m_UartStats, sizeof(UARTStats_t));
		MutexUnlock(&m_queueLock);
	}
}

//Get the current queue info
void GetQueueInfo(MessageQueueInfo_t *info)
{
	if (info)
	{
		MutexLock(&m_queueLock);
		memcpy(info, &m_QueueInfo, sizeof(MessageQueueInfo_t));
		MutexUnlock(&m_queueLock);
	}
}

uint8_t* PeekMessage(peek_t peekType, int32_t queueIndex, uint8_t  *buffer, uint32_t bufferLength)
{
	uint32_t abs_idx = UINT32_MAX;
	uint8_t* retValue = nullptr;

	MutexLock(&m_queueLock);

	switch (peekType)
	{
	case PeekAbsolute:
		abs_idx = queueIndex;
		break;
	case PeekHead:
		abs_idx = (m_QueueInfo.NextReadIndex + queueIndex) % m_QueueInfo.QueueSize;
		break;
	case PeekTail:
		abs_idx = (m_QueueInfo.NextWriteIndex + m_QueueInfo.QueueSize - queueIndex) % m_QueueInfo.QueueSize;
		break;
	default:
		assert(false && "Invalid peekType");
		break;
	}

	if (abs_idx < m_QueueInfo.QueueSize)
	{
		// abs_idx was set by the switch and is less than the size of the queue
		strncpy((char *)buffer, m_MsgQueue[abs_idx], bufferLength);
		retValue = buffer;
	}

	MutexUnlock(&m_queueLock);
	return retValue;
}


//------------------------------------------------------------------------------
// PRIVATE FUNCTIONS
//------------------------------------------------------------------------------

static bool _DecodeNextMessage(void)
{
	bool processed = false;
	MutexLock(&m_queueLock);
	if (m_QueueInfo.NextReadIndex != m_QueueInfo.NextWriteIndex)
	{
		processed = true;
		watchdog_refresh();

		if (strlen(m_MsgQueue[m_QueueInfo.NextReadIndex]) > 6)
		{
			// Check 1st byte is $ or !
			if (IS_0183_SOF(m_MsgQueue[m_QueueInfo.NextReadIndex][0]))
			{
				// Check not $$
				if (m_MsgQueue[m_QueueInfo.NextReadIndex][1] != m_MsgQueue[m_QueueInfo.NextReadIndex][0])
				{
					for (uint32_t mappingIndex = 0; mappingIndex < (sizeof(m_mapping)/sizeof(m_mapping[0])); mappingIndex++)
					{

						if (strncmp(&m_MsgQueue[m_QueueInfo.NextReadIndex][3], m_mapping[mappingIndex].msgId, 3) == 0)
						{
							m_mapping[mappingIndex].decode(m_MsgQueue[m_QueueInfo.NextReadIndex]);
						}
					}
				}
			}
		}
		// Increment the buffer read index
		m_QueueInfo.NextReadIndex = (m_QueueInfo.NextReadIndex + 1) % UART_MAX_MESSAGES;
	}
	MutexUnlock(&m_queueLock);
	return processed;
}

// This function is ONLY called by the system with the raw data received from the UART port.
// It's purpose is to decode the raw data into messages for later processing by UARTDecode().
static void _UARTProcess(uint8_t* pBuffer, uint32_t bufLen)
{
	uint32_t rxIdx = 0;
	uint8_t nextChar;

	uint32_t& queueSize = m_QueueInfo.QueueSize;
	uint32_t& nextReadIdx = m_QueueInfo.NextReadIndex;
	uint32_t& nextWriteIdx = m_QueueInfo.NextWriteIndex;

	while ((m_RxMsgIdx < UART_MAX_MESSAGE_SIZE) && (rxIdx < bufLen))
	{
		nextChar = pBuffer[rxIdx++];
		// we allow CR (0x0D) or LF (0x0A) to terminate the message
		// note that this can cause "blank" messages to appear between actual messages if the mesage is terminated with both CR and LF
		if ((nextChar == 0x0D) | (nextChar == 0x0A))
		{
			m_RxMsgBuf[m_RxMsgIdx++] = '\0'; //null terminate the incoming string
			//store the message at the next location
			MutexLock(&m_queueLock);

			if ((nextWriteIdx + 1) == nextReadIdx)
			{
				//We've filled the buffer, all we can do is discard the oldest (next to read) message
				nextReadIdx = (nextReadIdx + 1) % queueSize;
			}
			strcpy(m_MsgQueue[nextWriteIdx], m_RxMsgBuf);
			nextWriteIdx = (nextWriteIdx + 1) % queueSize;
			m_RxMsgIdx = 0;
			MutexUnlock(&m_queueLock);
		}
		else if ((m_RxMsgIdx != 0) || !isspace(nextChar))
		{
			// do not allow a space character at the beginning of a message
			m_RxMsgBuf[m_RxMsgIdx++] = nextChar;
		}

		//If we've now filled the rxbuffer without seeing CR or LF, throw away and start again
		if (m_RxMsgIdx == sizeof(m_RxMsgBuf))
		{
			m_RxMsgIdx = 0;
		}
	}
}
