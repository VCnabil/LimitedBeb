//------------------------------------------------------------------------------
//  TITLE :          extractField.h
//  DESCRIPTION :    Simple Extract function from a char array based on a delimiter.
//------------------------------------------------------------------------------
#ifndef __EXTRACT_FIELD_H__
#define __EXTRACT_FIELD_H__

#include "type.h"

// Pass in the VTG messages
// Returns TRUE If the string was extracted, FALSE if it was not.
BOOL _NMEA0183_ExtractField(uint8_t desiredField, const char * pBuffer, unsigned char * pDest, uint32_t MaxLengthOfpDest);

#endif // #ifndef __EXTRACT_FIELD_H__
