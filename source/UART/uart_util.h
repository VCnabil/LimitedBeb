//------------------------------------------------------------------------------
//  TITLE :          uart_util.h
//  DESCRIPTION :    Processing PVCI Serial String.
//------------------------------------------------------------------------------
#ifndef __UARTUTIL_H__
#define __UARTUTIL_H__

#include "project.h"


#define MAX_RAWINPUTSTRING_LENGTH 50
#define MAX_RAWINPUTSTRING_DIGITS 4

extern int lcd_config;

extern uint8_t sta1;
extern uint8_t sta2;
extern uint8_t sta3;
extern uint8_t nfe;
extern uint8_t sfe;
extern uint8_t cfe;
extern uint8_t gal;

extern int portBucketPosition;
extern int stbdBucketPosition;
extern int portNozzlePosition;
extern int stbdNozzlePosition;
extern int portInterceptorPosition;
extern int stbdInterceptorPosition;

void processSerial(void);

#endif // #ifndef __UARTUTIL_H__