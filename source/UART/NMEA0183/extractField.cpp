//------------------------------------------------------------------------------
//  TITLE :          VTG.c
//  DESCRIPTION :    Serial Communications functions.
//------------------------------------------------------------------------------
#include "syslib.h"

BOOL _NMEA0183_ExtractField(uint8_t desiredField, const char * pBuffer, unsigned char * pDest, uint32_t MaxLengthOfpDest)
{
	BOOL returnValue = FALSE;
	uint32_t lengthOfBuffer = strlen(pBuffer);
	uint32_t characterIndex = 0;
	uint32_t commaCount = 0;
	uint32_t startCharacter = 0;
	uint32_t Length = 0;
	BOOL hasStartOfFieldBeenFound = FALSE;

	if (desiredField == 0)
	{
		startCharacter = 0;
		hasStartOfFieldBeenFound = TRUE;
	}

	// Step through the string buffer, and check each character
	// we are looking for comma's so we can find the desired field.
	for (characterIndex = 0; characterIndex < lengthOfBuffer; characterIndex++)
	{
		if (pBuffer[characterIndex] == ',')
		{
			// We have found a comma!
			commaCount++;
			if (commaCount == desiredField)
			{
				// We have found the start of the message!
				startCharacter = characterIndex + 1; // +1 as we dont want the comma, we want the character after it!
				hasStartOfFieldBeenFound = TRUE;
			}
			else if (commaCount == (uint8_t)(desiredField + 1))
			{
				// We have found the start of the NEXT Message!
				// We are done now.
				returnValue = TRUE;
				break;
			}
		}
		else if(hasStartOfFieldBeenFound == TRUE)
		{
			// Current character is NOT a Comma
			// And we have found the start of the field we want
			// So we need to increase the Length.
			Length++;
		}
	}

	// character index is the same as the string length, so we reached the end of the for loop.
	if (characterIndex == lengthOfBuffer)
	{
		// We reached the end of the buffer before finding the next comma
		// hopefully this means that the user asked for the last field!
		if (hasStartOfFieldBeenFound == TRUE
			&& Length != 0)
		{
			// We have had the start of the message, and it has a length, so lets give them what we have!
			returnValue = TRUE;
		}
	}

	// only want to ouch the dest if we have valid data to load.
	if (returnValue)
	{
		// check the length is safe
		if (Length >= MaxLengthOfpDest)
		{
			Length = MaxLengthOfpDest - 1;
		}

		memcpy(pDest,&pBuffer[startCharacter],Length);
		pDest[Length] = '\0';
	}

	return returnValue;
}