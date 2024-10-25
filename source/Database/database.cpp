//------------------------------------------------------------------------------
//  TITLE :- Main parameter database - database.c
//  AUTHOR :
//  DATE :
//
//  DESCRIPTION :- Main database for all the parameters used in the system.
//
//------------------------------------------------------------------------------
#include <math.h>
#include "syslib.h"
#include "Database/database.h"
#include "Database/unitconv.h"
#include "Database/unitconv.h"
#include "CAN/can.h"

#include "UI/screens.h"
#include "UI/ttfonts.h"

#include <assert.h>
#include <vector>
#include <algorithm>
//------------------------------------------------------------------------------
// Local Enums and Types
//------------------------------------------------------------------------------

#define DBTIMER_MILLISECONDS	500

// Unit types for the initial database load
#define DATABASE_DESCRIPTION_MAX_LENGTH	50
typedef struct
{
    DBUNITS_T			Units;
	char				Description[DATABASE_DESCRIPTION_MAX_LENGTH];
    DATABASEINDEX_T		DatabaseItemCheck;
	std::vector<uint8_t>	DisplayedPrecision;
} DB_INFO_TABLE_T;

static uint32_t m_DatabaseInitUptime_ms = 0;

// Storage for all the database elements
static DBELEMENT_T		m_DBElements[DATABASEINDEX_MAX][DB_INSTANCE_INVALID];

// Mutex to protect against concurrent access from different threads for the above vars
static MUTEXHANDLE_T m_mutexHandle;

// The uptime when the timer process function was last called
static uint32_t m_timerFunctionLastUptime;

// Database units for each element. Only used in init
static DB_INFO_TABLE_T m_DBINFO[] =
{
	{ DBUNITS_ANGLE, "Magnetic Variation", db_MagneticVariation, { 1, 1 } },
	{ DBUNITS_RPM, "Demo Data", db_GaugeDemo, { 0 } },

	{ DBUNITS_SPEED, "Speed (Nav)", db_Speed_NavBased, { 0, 0, 0 } },

	{ DBUNITS_TIME, "Engine Hours", db_TotalHoursOfOperation_247, { 0, 0 } },
	{ DBUNITS_REVS, "Engine Revs (Total)", db_TotalRevolutions_249, { 0 } },

	{ DBUNITS_ELECTRICAL, "Supply Voltage", db_SupplyVoltage, { 1 } },

	{ DBUNITS_RESISTANCE, "Analogue Input 01", db_AnalogueInput_01_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 02", db_AnalogueInput_02_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 03", db_AnalogueInput_03_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 04", db_AnalogueInput_04_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 05", db_AnalogueInput_05_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 06", db_AnalogueInput_06_Resistance, {0}},
	{ DBUNITS_RESISTANCE, "Analogue Input 07", db_AnalogueInput_07_Resistance, {0}},

	{ DBUNITS_ELECTRICAL, "Analogue Input 01", db_AnalogueInput_01_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 02", db_AnalogueInput_02_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 03", db_AnalogueInput_03_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 04", db_AnalogueInput_04_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 05", db_AnalogueInput_05_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 06", db_AnalogueInput_06_Voltage, {1}},
	{ DBUNITS_ELECTRICAL, "Analogue Input 07", db_AnalogueInput_07_Voltage, {1}},

	{ DBUNITS_NONE, "Digital Input 01 State", db_DigitalInput_01_State, { 0 }},
	{ DBUNITS_NONE, "Digital Input 02 State", db_DigitalInput_02_State, { 0 }},

	{ DBUNITS_NONE, "Digital Input 01 Count", db_DigitalInput_01_Count, { 0 }},
	{ DBUNITS_NONE, "Digital Input 02 Count", db_DigitalInput_02_Count, { 0 }},

	{ DBUNITS_FREQUENCY, "Frequency Input", db_FrequencyInput, { 0 }},

	{ DBUNITS_ELECTRICAL, "Output 01 Sense Voltage", db_DigitalOutputSenseVoltage_01, { 1 }},
	{ DBUNITS_ELECTRICAL, "Output 02 Sense Voltage", db_DigitalOutputSenseVoltage_02, { 1 }},
	{ DBUNITS_ELECTRICAL, "Output 03 Sense Voltage", db_DigitalOutputSenseVoltage_03, { 1 }},
	{ DBUNITS_ELECTRICAL, "Output 04 Sense Voltage", db_DigitalOutputSenseVoltage_04, { 1 }},
};

// none of these thread lock, locking is handled by external functions
static void _DataBase_ProcessTimer(void* pUserData = nullptr);
static void _Database_InitValues(void);
static void _DataBase_ValidateDBINFO(void);

void DataBase_Init(void)
{
	// Setup Mutex
	MutexCreate(&m_mutexHandle);

	_DataBase_ValidateDBINFO();
	
	_Database_InitValues();

	// Register with the Timer System for the timer process callback
	m_timerFunctionLastUptime = get_uptime_milliseconds();
	timer_register(_DataBase_ProcessTimer, nullptr, DBTIMER_MILLISECONDS, DBTIMER_MILLISECONDS);

	// Register with the MMI system for the "once per loop" Callbacks
	m_DatabaseInitUptime_ms = get_uptime_milliseconds();
}

BOOL DataBase_IsItemValid(int DataBaseIndex, int InstanceIndex)
{
    BOOL IsValid = FALSE;
	DBELEMENT_T DBElement;

	// This is thread safe as it uses the public functions which are thread safe

    if ((DataBaseIndex < DATABASEINDEX_MAX) && (InstanceIndex < DB_INSTANCE_INVALID))
    {
		DataBase_Get(&DBElement, DataBaseIndex, InstanceIndex);

        if (DBElement.Timer > 0)
        {
            IsValid = TRUE;
        }
    }
	else
	{
		assert(FALSE);
	}

    return IsValid;
}

BOOL DataBase_Get(DBELEMENT_T * p_DB_Element, int DataBaseIndex, int InstanceIndex)
{
    BOOL bRet = FALSE;

    if ((DataBaseIndex < DATABASEINDEX_MAX) && (InstanceIndex < DB_INSTANCE_INVALID))
    {
        {// Thread safe
			MutexLock(&m_mutexHandle);
			{
				memcpy(p_DB_Element,&m_DBElements[DataBaseIndex][InstanceIndex],sizeof(DBELEMENT_T));
			}
			MutexUnlock(&m_mutexHandle);
		}// Thread safe
        bRet = TRUE;
    }
    else
    {
        assert(FALSE);
    }

    return bRet;
}

static BOOL _Database_IsSourceValid(int DataBaseIndex, int InstanceIndex, DBSOURCE_T DBSource, CAN_PORTS_T canPort, uint8_t sourceAddress, uint8_t nmeaInstanceOrSequenceID)
{
    BOOL isValid = TRUE;

    return isValid;
}

static BOOL _Database_Set_Individual(int databaseRow, 
									 int databaseColumn, 
									 const DBVAR_T* pData, 
									 DBVARTYPE_T pData_DataType,
									 DBSOURCE_T DBSource, 
									 CAN_PORTS_T canPort, 
									 uint8_t sourceAddress,
									 uint8_t nmeaInstanceOrSequenceID)
{
	BOOL IsItemLoaded = FALSE;

	if ((databaseRow < DATABASEINDEX_MAX) && (databaseColumn < DB_INSTANCE_INVALID) && (pData != 0))
	{
		// Check item is allowed to be set by the source
		if (_Database_IsSourceValid(databaseRow, databaseColumn, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID) != FALSE)
		{
			DBELEMENT_T* pElement;

			{// Thread safe
				MutexLock(&m_mutexHandle);
				{
					pElement = &m_DBElements[databaseRow][databaseColumn];

					pElement->PreviousMessageRxTime = pElement->MessageRxTime;
					pElement->MessageRxTime = get_uptime_milliseconds();

					pElement->OwnerSource = DBSource;

					pElement->nmeaInstance = nmeaInstanceOrSequenceID;
					pElement->canPort = canPort;
					pElement->sourceAddress = sourceAddress;

					pElement->PreviousData = pElement->Data;
					pElement->Data = *pData;

					// Timer needs to be updated AFTER the data as its used to catch a "new write" and skip the filtering if thats the case.
					pElement->Timer = pElement->TimerDefault;					

					pElement->DataType = pData_DataType;
					pElement->DatabaseIndex = databaseRow;
				}
				MutexUnlock(&m_mutexHandle);
			}// Thread safe

			// Succeeded
			IsItemLoaded = TRUE;
		}
	}
	else
	{
		assert(FALSE);
	}

	return IsItemLoaded;
}

static BOOL _Database_Set(int databaseRow, 
						  const DBVAR_T* pData, 
						  DBVARTYPE_T pData_DataType,
						  DBSOURCE_T DBSource, 
						  CAN_PORTS_T canPort, 
						  uint8_t sourceAddress,
						  uint8_t nmeaInstanceOrSequenceID)
{
    BOOL returnValue = FALSE;
	uint32_t numberOfEngines = DB_INSTANCE_INVALID;

	if (numberOfEngines >= 1)
	{
		if (_Database_Set_Individual(databaseRow, 0, pData, pData_DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID))
		{
			returnValue = TRUE;
		}
	}

	if (numberOfEngines >= 2)
	{
		if (_Database_Set_Individual(databaseRow, 1, pData, pData_DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID))
		{
			returnValue = TRUE;
		}
	}

	if (numberOfEngines >= 3)
	{
		if (_Database_Set_Individual(databaseRow, 2, pData, pData_DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID))
		{
			returnValue = TRUE;
		}
	}

	if (numberOfEngines >= 4)
	{
		if (_Database_Set_Individual(databaseRow, 3, pData, pData_DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID))
		{
			returnValue = TRUE;
		}
	}

	return returnValue;
}

BOOL Database_Set_CAN(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, CAN_PORTS_T canPort, uint8_t nmeaInstanceOrSequenceID, uint8_t sourceAddress)
{
	return _Database_Set(DataBaseIndex, pData, DataType, DBSOURCE_CAN, canPort, sourceAddress, nmeaInstanceOrSequenceID);
}

BOOL Database_Set_IO(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex)
{
	DBSOURCE_T DBSource = DBSOURCE_LOCAL;
		
	CAN_PORTS_T canPort = (CAN_PORTS_T)2;	// IO doesnt use this, set it to an OOR number.
	uint8_t sourceAddress = 255;	// IO doesnt use this, set it to an OOR number.
	uint8_t nmeaInstanceOrSequenceID = 255;	// IO doesnt use this, set it to an OOR number.

	// This one already has an engine Index so we dont need to multiply it up.
	return _Database_Set_Individual(DataBaseIndex, engineIndex, pData, DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID);
}

BOOL Database_Set_NMEA0183(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex)
{
	DBSOURCE_T DBSource = DBSOURCE_NMEA0183;

	CAN_PORTS_T canPort = (CAN_PORTS_T)2;	// NMEA0183 doesnt use this, set it to an OOR number.
	uint8_t sourceAddress = 255;	// NMEA0183 doesnt use this, set it to an OOR number.
	uint8_t nmeaInstanceOrSequenceID = 255;	// NMEA0183 doesnt use this, set it to an OOR number.

	// This one already has an engine Index so we dont need to multiply it up.
	return _Database_Set_Individual(DataBaseIndex, engineIndex, pData, DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID);
}

BOOL Database_Set_Demo(int DataBaseIndex, const DBVAR_T* pData, DBVARTYPE_T DataType, uint8_t engineIndex)
{
	DBSOURCE_T DBSource = DBSOURCE_DEMO;
		
	CAN_PORTS_T canPort = (CAN_PORTS_T)2;	// IO doesnt use this, set it to an OOR number.
	uint8_t sourceAddress = 255;	// IO doesnt use this, set it to an OOR number.
	uint8_t nmeaInstanceOrSequenceID = 255;	// IO doesnt use this, set it to an OOR number.

	// This one already has an engine Index so we dont need to multiply it up.
	return _Database_Set_Individual(DataBaseIndex, engineIndex, pData, DataType, DBSource, canPort, sourceAddress, nmeaInstanceOrSequenceID);
}

float DataBase_GetDataFromElementAsFloat(DBELEMENT_T * Element)
{
	float ReturnValue = 0.0f;
	float RealValue = 0.0f;

	// This is thread safe as it does not access anything!
	switch(Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		RealValue = (float)Element->Data.ui;
		break;
	case DBVARTYPE_INT:
		RealValue = (float)Element->Data.i;
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		RealValue = (float)Element->Data.ulg;
		break;
	case DBVARTYPE_LONG:
		RealValue = (float)Element->Data.lg;
		break;
	case DBVARTYPE_FLOAT:
		RealValue = (float)Element->Data.flt;
		break;
		//////////////////////////////////////////////////////////////////////////
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Type" && FALSE);
		break;
	}

	switch (Element->DatabaseIndex)
	{
		// You can add special cases here if needed.

	case DATABASEINDEX_MAX:
	default:
		ReturnValue = RealValue;
		break;
	}
	
	return ReturnValue;
}

int Database_snprintf(char * destination, size_t destinationSize, const char * formattingString, DBELEMENT_T * source)
{
	// this is thread safe as it does not access anything!

	int retValue = -1;

	if (source->Timer)
	{
		switch (source->DatabaseIndex)
		{

		// Special cases can be added here

		case DATABASEINDEX_MAX:
		default:
			switch(source->DataType)
			{
			case DBVARTYPE_UNSIGNED_INT:
				retValue = snprintf(destination, destinationSize, formattingString, (float)source->Data.ui);
				break;
			case DBVARTYPE_INT:
				retValue = snprintf(destination, destinationSize, formattingString, (float)source->Data.i);
				break;
			case DBVARTYPE_UNSIGNED_LONG:
				retValue = snprintf(destination, destinationSize, formattingString, (float)source->Data.ulg);
				break;
			case DBVARTYPE_LONG:
				retValue = snprintf(destination, destinationSize, formattingString, (float)source->Data.lg);
				break;
			case DBVARTYPE_FLOAT:
				retValue = snprintf(destination, destinationSize, formattingString, source->Data.flt);
				break;
			case DBVARTYPE_POINTER:
			case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
			default:
				assert("Bad Data Type" && FALSE);
				break;
			}
			break;
		}
	}
	else
	{
		retValue = snprintf(destination, destinationSize, "%s", "---");
	}

	return retValue;
}

int Database_snprintfWithUnits(char * destination, size_t destinationSize, const char * formattingString, DBELEMENT_T * source)
{
	// This is thread safe as it does not access anything!
	
	// get the length of the formatted value (return value of snprintf)
	// this is needed so we have a temporary buffer of the correct size for the formatted data
	int valueLength = Database_snprintf(nullptr, 0, formattingString, source);

	if (valueLength < 0)
	{
		//there was an error - possibly an invalid formatting string
		return valueLength;
	}

	// +1 for null character
	std::vector<char> formattedValue(valueLength + 1);
	Database_snprintf(formattedValue.data(), formattedValue.size(), formattingString, source);

	return snprintf(destination, destinationSize, "%s %s", formattedValue.data(), UnitGetStringCurrentUnits(source->UnitType));
}

int Database_GetUnitString(char * unitString, size_t unitStringSize, DBELEMENT_T * source)
{
	return snprintf(unitString, unitStringSize, "%s", UnitGetStringCurrentUnits(source->UnitType));
}

int Database_GetUnitStringSpecificUnits(char * unitString, size_t unitStringSize, DBELEMENT_T * source, uint8_t unitIndex)
{
	return snprintf(unitString, unitStringSize, "%s", UnitGetStringSpecificUnits(source->UnitType, unitIndex));
}

BOOL Database_GetIndexFromString(const char * pString, uint32_t * pDBIndex)
{
	BOOL isStringValid = FALSE;
	uint32_t LoopIndex = 0;

	assert(pString && pDBIndex);

	MutexLock(&m_mutexHandle);

	for(LoopIndex = 0; LoopIndex < DATABASEINDEX_MAX; LoopIndex++)
	{
		if (strcmp(pString, m_DBINFO[LoopIndex].Description) == 0)
		{
			// Strings MATCH!
			*pDBIndex = LoopIndex;
			isStringValid = TRUE;
			break;
		}
	}

	MutexUnlock(&m_mutexHandle);

	return isStringValid;
}

int Database_GetItemDescription(char* destination, size_t destinationSize, int DataBaseIndex)
{
	if (DataBaseIndex < DATABASEINDEX_MAX)
	{
		MutexLock(&m_mutexHandle);
		int retValue = snprintf(destination, destinationSize, "%s", m_DBINFO[DataBaseIndex].Description);
		MutexUnlock(&m_mutexHandle);
		return retValue;
	}
	else
	{
		return -1;
	}
}

void Database_SetItemDescription(int DataBaseIndex, const char * pDescription, uint32_t descriptionLength)
{
	if (DataBaseIndex < DATABASEINDEX_MAX)
	{
		// Database Index is Good.
		MutexLock(&m_mutexHandle);
		snprintf(m_DBINFO[DataBaseIndex].Description, sizeof(m_DBINFO[DataBaseIndex].Description), "%s", pDescription);
		MutexUnlock(&m_mutexHandle);
	}
}

uint32_t DatabaseGetDisplayedPrecision(int DataBaseIndex)
{
	uint32_t returnValue = 0;
	uint32_t	currentUnitsIndex = 0;

	if (DataBaseIndex < DATABASEINDEX_MAX && currentUnitsIndex < m_DBINFO[DataBaseIndex].DisplayedPrecision.size())
	{
		currentUnitsIndex = SettingsUnitGetUnits(m_DBINFO[DataBaseIndex].Units);
		returnValue = m_DBINFO[DataBaseIndex].DisplayedPrecision[currentUnitsIndex];
	}

	return returnValue;
}

const char * DatabaseGetPrecisionString(int DataBaseIndex)
{
	const char * ReturnValue = "%0.0f";
	uint32_t	currentUnitsIndex = 0;
	uint32_t displayedPrecision = 0;

	if (DataBaseIndex < DATABASEINDEX_MAX && currentUnitsIndex < m_DBINFO[DataBaseIndex].DisplayedPrecision.size())
	{
		// First we need to find out the type of units, and the "current" units, then use that to get the correct precision from the DBInfo.
		currentUnitsIndex = SettingsUnitGetUnits(m_DBINFO[DataBaseIndex].Units);
		displayedPrecision = m_DBINFO[DataBaseIndex].DisplayedPrecision[currentUnitsIndex];

		switch(displayedPrecision)
		{
		case 0:
			ReturnValue = "%0.0f";
			break;
		case 1:
			ReturnValue = "%0.1f";
			break;
		case 2:
			ReturnValue = "%0.2f";
			break;
		default:
			ReturnValue = "%0.0f";
			break;
		}
	}
	else
	{
		ReturnValue = "%0.0f";
	}

	return ReturnValue;
}

DBUNITS_T DatabaseGetUnitType(uint32_t DBIndex)
{
	DBUNITS_T returnValue = DBUNITS_END;

	if (DBIndex < DATABASEINDEX_MAX)
	{
		returnValue = m_DBINFO[DBIndex].Units;
	}

	return returnValue;
}

static void _DataBase_ProcessTimer(void* pUserData)
{
	(void)pUserData;
	
	int DBIndex = 0;
	int InstanceIndex = 0;

	uint32_t currentUptime = get_uptime_milliseconds();
	uint32_t timerDifference = currentUptime - m_timerFunctionLastUptime;
	m_timerFunctionLastUptime = currentUptime;

	MutexLock(&m_mutexHandle);
	for (DBIndex = 0; DBIndex < DATABASEINDEX_MAX; DBIndex++)
	{
		for (InstanceIndex = 0; InstanceIndex < DB_INSTANCE_INVALID; InstanceIndex++)
		{
			DBELEMENT_T* pElement;
			pElement = &m_DBElements[DBIndex][InstanceIndex];

			if (pElement->Timer != UINT32_MAX)
			{
				if (pElement->Timer > 0)
				{
					// Decrement timer
					pElement->Timer -= std::min(pElement->Timer, timerDifference);
				}

				if (pElement->Timer == 0)
				{
					// Data timed-out
					pElement->OwnerSource = DBSOURCE_NONE;
				}
			}
		}
	}
	MutexUnlock(&m_mutexHandle);
}

static void _Database_InitValues(void)
{
	uint32_t DBIndex = 0;
	uint32_t InstanceIndex = 0;

	{// Thread safe
		MutexLock(&m_mutexHandle);
		{
			// Loop through and init the DB now we have checked where the enum is used
			for (DBIndex = 0; DBIndex < DATABASEINDEX_MAX; DBIndex++)
			{
				for (InstanceIndex = 0; InstanceIndex < DB_INSTANCE_INVALID; InstanceIndex++)
				{
					DBELEMENT_T* pElement = &m_DBElements[DBIndex][InstanceIndex];
					
					pElement->Data.flt = 0;
					pElement->PreviousData.flt = 0;
					pElement->DataType = DBVARTYPE_FLOAT;
					pElement->OwnerSource = DBSOURCE_NONE;
					pElement->TimerDefault = 6000;	// ms
					pElement->Timer = 0;
					pElement->UnitType = m_DBINFO[DBIndex].Units;
					pElement->MessageRxTime = 0;
				}
			}

			// Exceptions
			for (InstanceIndex = 0; InstanceIndex < DB_INSTANCE_INVALID; InstanceIndex++)
			{
				// These are Requested Parameters and generaly update really slowly so they have a longer timeout.
			}
		}
		MutexUnlock(&m_mutexHandle);
	}// Thread safe
}

static void _DataBase_ValidateDBINFO(void)
{
	uint32_t DBIndex = 0;
	uint32_t DBINFO_Size = 0;

	DBINFO_Size = sizeof(m_DBINFO)/sizeof(DB_INFO_TABLE_T);

	// before we check this, we need to make sure it is the correct length/size
	if (DBINFO_Size != DATABASEINDEX_MAX)
	{
		assert(FALSE);
	}

	// Loop Through DBInfo and check that there is one slot for each Database Item, and that they lign up!
	for (DBIndex = 0; DBIndex < DATABASEINDEX_MAX; DBIndex++)
	{
		if (m_DBINFO[DBIndex].DatabaseItemCheck != (int32_t)DBIndex)
		{
			assert(FALSE);
		}
	}
}