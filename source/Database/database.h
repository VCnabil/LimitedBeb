//------------------------------------------------------------------------------
//  TITLE :-  Database.h
//  AUTHOR :
//  DATE :
//  DESCRIPTION :  Database declarations
//
//------------------------------------------------------------------------------
#ifndef DATABASE_H
#define DATABASE_H

#include "type.h"

typedef enum
{
	DB_INSTANCE_1 = 0,
	DB_INSTANCE_2,
	DB_INSTANCE_3,
	DB_INSTANCE_4,

	//////////////////////////////////////////////////////////////////////////
	// Used for checking is "Valid" so must be LAST!
	DB_INSTANCE_INVALID,
} DB_INSTANCE_T;

// Data source
typedef enum
{
	DBSOURCE_NONE = 0,
	DBSOURCE_DEMO,
	DBSOURCE_LOCAL,
	DBSOURCE_CAN,
    DBSOURCE_NMEA0183,

	//////////////////////////////////////////////////////////////////////////
	// Used for checking is "Valid" so must be LAST!
	DBSOURCE_INVALID,
} DBSOURCE_T;

// Unit types for parameters in database
typedef enum
{
    DBUNITS_PRESSURE = 0,
    DBUNITS_TEMPERATURE,
    DBUNITS_VOLUME,
    DBUNITS_DISTANCE,
    DBUNITS_PERCENT,
    DBUNITS_FLOW_RATE,
    DBUNITS_TIME,
    DBUNITS_MASS,
    DBUNITS_RPM,
    DBUNITS_SPEED,
    DBUNITS_ELECTRICAL,
    DBUNITS_ECONOMY,
	DBUNITS_COMMAND,
	DBUNITS_DEPTH,
	DBUNITS_BEARING,
	DBUNITS_ANGLE,
	DBUNITS_CURRENT,
	DBUNITS_FREQUENCY,
	DBUNITS_REVS,
	DBUNITS_RESISTANCE,
	DBUNITS_NONE,
    DBUNITS_END
} DBUNITS_T;

// This Enum is used for the database to control its size as well as to provide translations for the database descriptor's
typedef enum
{
	db_MagneticVariation,
	db_GaugeDemo,

	db_Speed_NavBased,

	db_TotalHoursOfOperation_247,
	db_TotalRevolutions_249,

	db_SupplyVoltage,

	db_AnalogueInput_01_Resistance,
	db_AnalogueInput_02_Resistance,
	db_AnalogueInput_03_Resistance,
	db_AnalogueInput_04_Resistance,
	db_AnalogueInput_05_Resistance,
	db_AnalogueInput_06_Resistance,
	db_AnalogueInput_07_Resistance,

	db_AnalogueInput_01_Voltage,
	db_AnalogueInput_02_Voltage,
	db_AnalogueInput_03_Voltage,
	db_AnalogueInput_04_Voltage,
	db_AnalogueInput_05_Voltage,
	db_AnalogueInput_06_Voltage,
	db_AnalogueInput_07_Voltage,

	db_DigitalInput_01_State,
	db_DigitalInput_02_State,

	db_DigitalInput_01_Count,
	db_DigitalInput_02_Count,

	db_FrequencyInput,

	db_DigitalOutputSenseVoltage_01,
	db_DigitalOutputSenseVoltage_02,
	db_DigitalOutputSenseVoltage_03,
	db_DigitalOutputSenseVoltage_04,

	//////////////////////////////////////////////////////////////////////////
	//	Items added here may need to be added in other places depending on your code.
	//	There is a Structure in database.cpp that is checked at runtime
	//  If you add something to this enum and dont add it to that array the code will assert
	//
	//	If you add anything similar that is the same size as the database enum or is related to it in any way
	//	It is highly recommended adding a similar check.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// This must always be the last entry
	DATABASEINDEX_MAX,

	// This is used on the Screens and checked before accessing the DB so it knows if the item needs drawing or not.
	ITEM_NOT_SHOWN = DATABASEINDEX_MAX
} DATABASEINDEX_T;

//	enum used to go with DBVAR_T so we know the current type
typedef enum
{
	DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT = 0,
	DBVARTYPE_UNSIGNED_INT,
	DBVARTYPE_INT,
	DBVARTYPE_UNSIGNED_LONG,
	DBVARTYPE_LONG,
	DBVARTYPE_FLOAT,
	DBVARTYPE_POINTER
} DBVARTYPE_T;

//	Variable Union Structure, used by the database to hold data
typedef union
{
    uint8_t				array[4];
    uint32_t			ui;
    int32_t				i;
    uint32_t			ulg;
    int32_t				lg;
    float				flt;
    const uint8_t	*	ptr;
} DBVAR_T;

// Database element definition
typedef struct
{
	DBVAR_T					Data;					// displayed data
	DBVAR_T					PreviousData;			// displayed data
	DBVARTYPE_T				DataType;				// tells you how the data is stored
	DBSOURCE_T				OwnerSource;			// DB Source that set the data

	CAN_PORTS_T				canPort;				// Shows 0 or 1 depending on the can port. non can data should show CAN_MAX_PORTS
	uint8_t					sourceAddress;			// This is the Source Address of the device the data came from. for non-can data, it is 255 as that is invalid in CAN.
	uint8_t					nmeaInstance;			// This is the NMEA2000 "Instance" and is used primarily to pick the "engine" but is saved just in case.
	
	uint32_t				Timer;					// 1ms resolution down counter
	uint32_t				TimerDefault;			// reload value for timer
	DBUNITS_T				UnitType;				// DB_UNITS_T Type of Units
	uint32_t				MessageRxTime;			// get_uptime_milliseconds() when the message was received
	uint32_t				PreviousMessageRxTime;	// get_uptime_milliseconds() when the message was received
	uint32_t				DatabaseIndex;			// a Database Index so when the element is on its own, we know what it is.
}  DBELEMENT_T;

// To be called from Main Init, Initializes the database
void DataBase_Init(void);

// Returns TRUE if the database item specified is valid (it has a non-expired timer value)
BOOL DataBase_IsItemValid(int DataBaseIndex, int InstanceIndex);

// Get the item specified from the database. Returns TRUE if item returned, FALSE on error
BOOL DataBase_Get(DBELEMENT_T * p_DB_Element, int DataBaseIndex, int InstanceIndex);

// Returns TRUE if the database has been loaded, otherwise FALSE
// but REALLY, why does the _J1939 function need DBSOURCE_T DBSource! the answer is it doesnt, but it made changin the function 1000x easier as it meant the parameter didnt need to be removed from all 130 calls.
BOOL Database_Set_CAN(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, CAN_PORTS_T canPort, uint8_t nmeaInstanceOrSequenceID, uint8_t sourceAddress);
BOOL Database_Set_IO(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex);
BOOL Database_Set_NMEA0183(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex);
BOOL Database_Set_Demo(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex);

// Returns a Float, no matter how the Data is stored.
float DataBase_GetDataFromElementAsFloat(DBELEMENT_T * Element);

// Called in place of snprintf to avoid having switch statements EVERYWHERE
// The formattingString should contain a single specifier expecting a floating-point value
// Some database entries may ignore the formattingString and use a special format
int Database_snprintf(char * destination, size_t destinationSize, const char * formattingString, DBELEMENT_T * source);

// Same as Database_snprintf but it adds a space, then concatenates the units in
int Database_snprintfWithUnits(char * destination, size_t destinationSize, const char * formattingString, DBELEMENT_T * source);

// Loads unitString with the Units
int Database_GetUnitString(char * unitString, size_t unitStringSize, DBELEMENT_T * source);
int Database_GetUnitStringSpecificUnits(char * unitString, size_t unitStringSize, DBELEMENT_T * source, uint8_t unitIndex);

// Used by the Ini file to convert the strings in the ini file into DB Index's
// Returns FALSE if string invalid
BOOL Database_GetIndexFromString(const char * String, uint32_t * DBIndex);

// loads the descriptor into the destination buffer
int Database_GetItemDescription(char* destination, size_t destinationSize, int DataBaseIndex);
void Database_SetItemDescription(int DataBaseIndex, const char * pDescription, uint32_t descriptionLength);

uint32_t DatabaseGetDisplayedPrecision(int DataBaseIndex);
const char * DatabaseGetPrecisionString(int DataBaseIndex);

DBUNITS_T DatabaseGetUnitType(uint32_t DBIndex);

#endif	//	DATABASE_H