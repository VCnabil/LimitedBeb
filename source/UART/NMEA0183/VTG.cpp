//------------------------------------------------------------------------------
//  TITLE :          VTG.c
//  DESCRIPTION :    Process the NMEA0183 related VTG Message
//------------------------------------------------------------------------------
#include "syslib.h"
#include "Database/database.h"
#include "UART/NMEA0183/extractField.h"

int NMEA0183_ProcessVTG(char * pBuffer)
{
	// VTG Decode data came from here;
	// http://www.gpsinformation.org/dale/nmea.htm#VTG
	//
	//	VTG - Velocity made good. The gps receiver may use the LC prefix instead of GP if it is emulating Loran output.
	//
	//	$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
	//
	//	where:
	//	VTG          Track made good and ground speed
	//	054.7,T      True track made good (degrees)
	//	034.4,M      Magnetic track made good
	//	005.5,N      Ground speed, knots
	//	010.2,K      Ground speed, Kilometers per hour
	//	*48          Checksum
	//	Note that, as of the 2.3 release of NMEA, there is a new field in the VTG sentence at the end just prior to the checksum. For more information on this field see here.
	//
	//	Receivers that don't have a magnetic deviation (variation) table built in will null out the Magnetic track made good.

	unsigned char field[255];

	// First we check if we have a Knots field
	if (_NMEA0183_ExtractField(5, (const char *)pBuffer, field, sizeof(field)))
	{
		DBVAR_T DBData;
		uint8_t engineIndex = 0;

		// No conversion is needed as it is already in knots (how we store speed)
		DBData.flt = (float)atof((const char *)field);
		Database_Set_NMEA0183(db_Speed_NavBased, &DBData, DBVARTYPE_FLOAT, engineIndex);
	}
	// We dont have a Knots Field, so lets check Km/h
	else if (_NMEA0183_ExtractField(7, (const char *)pBuffer, field, sizeof(field)))
	{
		DBVAR_T DBData;
		uint8_t engineIndex = 0;

		// Need to convert from Km/h to Knots
		DBData.flt = (float)(atof((const char *)field) * (1 / 1.85200f));
		Database_Set_NMEA0183(db_Speed_NavBased, &DBData, DBVARTYPE_FLOAT, engineIndex);
	}

	return 0;
}