//------------------------------------------------------------------------------
//  TITLE :          uart.h
//  DESCRIPTION :    Serial Communications functions.
//------------------------------------------------------------------------------
#ifndef __UART_H__
#define __UART_H__

#include "project.h"

/// @defgroup uart UART Functions
/// Functions for controlling the UART
/// @{
//------------------------------------------------------------------------------
// GENERAL #DEFINES
//------------------------------------------------------------------------------
/// Maximum Message Size
#define UART_MAX_MESSAGE_SIZE       120
/// Maximum Messages
#define UART_MAX_MESSAGES           255

//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

//statistics about message processing
typedef struct {
	uint32_t TotalMessages;
	uint32_t LostMessages;
} UARTStats_t;


typedef struct {
	uint32_t QueueSize;  //number of entries in buffer queue
	uint32_t NextReadIndex;	 //next message to read
	uint32_t NextWriteIndex; //next message to write
} MessageQueueInfo_t;


typedef enum
{
	PeekHead,				//peek at the offset from the head of the queue
	PeekTail,				//peek at the offset from the back of the queue
	PeekAbsolute			//peek at the absolute index in the buffer
} peek_t;

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------
/// Initialise the UART port
void UARTInit(void);

/// Send buffer pData of length dataLen over the UART port.
BOOL UARTSend(uint8_t *pData, uint32_t dataLen);

/// Process any received messages
void UARTDecode(void);

//Get the current uart stats
void GetUARTStats(UARTStats_t *stats);

//Get the current queue info
void GetQueueInfo(MessageQueueInfo_t *info);

/// Get a specified message from the Queue
/// Returns a copy of a message in the queue.  We copy the message since there's no
/// guarantee the message won't be processed in another thread by the time a simple 
/// returned pointer is used
///
/// @param peekType the start position for the queue peek
/// @param queueIndex the position in the queue 
/// @param buffer - pointer to buffer to receive copy of data 
/// @param bufferLenght - the length of the buffer passed
/// @return pointer to the passed in buffer containing null terminated string or null if 
///          the requested message couldn't be retrieved.
uint8_t* PeekMessage(peek_t peekType, int32_t queueIndex, uint8_t* buffer, uint32_t bufferLength);

/// @} // endgroup uart

#endif // #ifndef __UART_H__
