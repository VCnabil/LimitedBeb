//------------------------------------------------------------------------------
//  TITLE :          settings.c
//------------------------------------------------------------------------------

#include "project.h"
static void _SettingsLoadDefaults(void);

#define CRC_START_VALUE 0x1D0F

// This is the structure for storing parameters in EEPROM.
typedef struct
{
	///////////////////////////////////////////////////////////////////////////
	// SYSTEM SECTION - DO NOT MODIFY - THESE 2 ENTRIES _MUST_ BE FIRST!
	///////////////////////////////////////////////////////////////////////////
	uint32_t settingsCRC;
	uint32_t settingsSaveCount;

	///////////////////////////////////////////////////////////////////////////
	// Default User Settings
	///////////////////////////////////////////////////////////////////////////
	// Selected language
	uint32_t language;

	// Key bleep on/off
	BOOL isBuzzerMuted;

	///////////////////////////////////////////////////////////////////////////
	// You _MUST_ add any new settings to be saved _AFTER_ this comment...
	///////////////////////////////////////////////////////////////////////////

	// Units
	int8_t	PressureUnits;
	int8_t	TemperatureUnits;
	int8_t	VolumeUnits;
	int8_t	DistanceUnits;
	int8_t	FuelRateUnits;
	int8_t	SpeedUnits;
	int8_t	DepthUnits;
	int8_t	AngleUnits;
	int8_t	BearingUnits;
	int8_t	EconomyUnits;
	int8_t	TimeUnits;

	// Device and System Instance found in the Name PGN
	uint8_t CANSystemInstance[CAN_MAX_PORTS];
	uint8_t CANDeviceInstance[CAN_MAX_PORTS];

} SETTINGS_T;

//------------------------------------------------------------------------------
// MODULE GLOBALS
//------------------------------------------------------------------------------

// Instances of settings structure
static SETTINGS_T m_settings;
static const void * m_crcStartingPoint = (char*)&m_settings.settingsCRC + sizeof(m_settings.settingsCRC);
static const uint32_t m_crcLength = sizeof(m_settings) - sizeof(m_settings.settingsCRC);

//------------------------------------------------------------------------------
// LOCAL FUNCTIONS
//------------------------------------------------------------------------------

static void SettingsSaveCallback(void* pUserData = nullptr);

static void SettingsSaveCallback(void* pUserData)
{
	(void)pUserData;
	SettingsSave();
}

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

void SettingsInit(void)
{
	// Register a 1 minute timer to automatically save the settings
	timer_register(SettingsSaveCallback, nullptr, 1000 * 60, 1000 * 60);

	// Load the settings
	SettingsLoad();
}

void SettingsLoad(void)
{
	if (eepromRead(EEPROMSimulated, 0, (uint8_t *)&m_settings, sizeof(m_settings)) == 0)
	{
		m_settings.settingsSaveCount = 0;
		load_eeprom_defaults();
	}
	else
	{
		if (m_settings.settingsCRC != crc16(CRC_START_VALUE, m_crcStartingPoint, m_crcLength))
		{
			// CRC Failed.
			m_settings.settingsSaveCount = 0;
			load_eeprom_defaults();
		}
	}

	// Initialise the key press beep state from settings
	buzzer_mute(m_settings.isBuzzerMuted);
}

void SettingsSave(void)
{
	uint16_t settingsCRC = crc16(CRC_START_VALUE, m_crcStartingPoint, m_crcLength);
	uint16_t savedDataCRC = 0;

	SETTINGS_T currentSavedData;

	uint32_t bytesRead = eepromRead(EEPROMSimulated, 0, (uint8_t *)&currentSavedData, sizeof(currentSavedData));

	if (bytesRead != 0)
	{
		savedDataCRC = currentSavedData.settingsCRC;
	}

	if (savedDataCRC != settingsCRC || bytesRead == 0)
	{
		// CRC's are different, save the data.
		m_settings.settingsSaveCount++;
		m_settings.settingsCRC = crc16(CRC_START_VALUE, m_crcStartingPoint, m_crcLength);
		eepromWrite(EEPROMSimulated, 0, (uint8_t *)&m_settings, sizeof(m_settings));
	}
}

uint32_t SettingsGetLanguageIndex(void)
{
	return m_settings.language;
}

void SettingsSetLanguageIndex(uint32_t languageIndex)
{
	// Changed?
	if (languageIndex != m_settings.language)
	{
		// Yes. Save
		m_settings.language = languageIndex;

		SettingsSave();
	}
}

BOOL SettingsIsBleepEnabled(void)
{
	return m_settings.isBuzzerMuted;
}

void SettingsToggleBleep(void)
{
	if (m_settings.isBuzzerMuted)
	{
		m_settings.isBuzzerMuted = FALSE;
	}
	else
	{
		m_settings.isBuzzerMuted = TRUE;
	}
	buzzer_mute(m_settings.isBuzzerMuted);

	SettingsSave();
}

void SettingsUnitSetPressureUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_PRESSURE))
	{
		m_settings.PressureUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetTemperatureUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_TEMPERATURE))
	{
		m_settings.TemperatureUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetVolumeUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_VOLUME))
	{
		m_settings.VolumeUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetDistanceUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_DISTANCE))
	{
		m_settings.DistanceUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetFuelRateUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_FLOW_RATE))
	{
		m_settings.FuelRateUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetSpeedUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_SPEED))
	{
		m_settings.SpeedUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetDepthUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_DEPTH))
	{
		m_settings.DepthUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetAngleUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_ANGLE))
	{
		m_settings.AngleUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetBearingUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_BEARING))
	{
		m_settings.BearingUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetEconomyUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_ECONOMY))
	{
		m_settings.EconomyUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitSetTimeUnits(uint8_t NewUnits)
{
	if (NewUnits <= UnitGetMaxValid(DBUNITS_TIME))
	{
		m_settings.TimeUnits = NewUnits;
		SettingsSave();
	}
}

void SettingsUnitsTogglePressure(void* userData)
{
	if (SettingsUnitGetPressureUnits() == UnitGetMaxValid(DBUNITS_PRESSURE))
	{
		SettingsUnitSetPressureUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetPressureUnits();
		SettingsUnitSetPressureUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleTemperature(void* userData)
{
	if (SettingsUnitGetTemperatureUnits() == UnitGetMaxValid(DBUNITS_TEMPERATURE))
	{
		SettingsUnitSetTemperatureUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetTemperatureUnits();
		SettingsUnitSetTemperatureUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleVolume(void* userData)
{
	if (SettingsUnitGetVolumeUnits() == UnitGetMaxValid(DBUNITS_VOLUME))
	{
		SettingsUnitSetVolumeUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetVolumeUnits();
		SettingsUnitSetVolumeUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleDistance(void* userData)
{
	if (SettingsUnitGetDistanceUnits() == UnitGetMaxValid(DBUNITS_DISTANCE))
	{
		SettingsUnitSetDistanceUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetDistanceUnits();
		SettingsUnitSetDistanceUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleFuelRate(void* userData)
{
	if (SettingsUnitGetFuelRateUnits() == UnitGetMaxValid(DBUNITS_FLOW_RATE))
	{
		SettingsUnitSetFuelRateUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetFuelRateUnits();
		SettingsUnitSetFuelRateUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleSpeed(void* userData)
{
	if (SettingsUnitGetSpeedUnits() == UnitGetMaxValid(DBUNITS_SPEED))
	{
		SettingsUnitSetSpeedUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetSpeedUnits();
		SettingsUnitSetSpeedUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleDepth(void* userData)
{
	if (SettingsUnitGetDepthUnits() == UnitGetMaxValid(DBUNITS_DEPTH))
	{
		SettingsUnitSetDepthUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetDepthUnits();
		SettingsUnitSetDepthUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleAngle(void* userData)
{
	if (SettingsUnitGetAngleUnits() == UnitGetMaxValid(DBUNITS_ANGLE))
	{
		SettingsUnitSetAngleUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetAngleUnits();
		SettingsUnitSetAngleUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleBearing(void* userData)
{
	if (SettingsUnitGetBearingUnits() == UnitGetMaxValid(DBUNITS_BEARING))
	{
		SettingsUnitSetBearingUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetBearingUnits();
		SettingsUnitSetBearingUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleEconomy(void* userData)
{
	if (SettingsUnitGetEconomyUnits() == UnitGetMaxValid(DBUNITS_ECONOMY))
	{
		SettingsUnitSetEconomyUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetEconomyUnits();
		SettingsUnitSetEconomyUnits(++CurrentUnits);
	}
}

void SettingsUnitsToggleTime(void* userData)
{
	if (SettingsUnitGetTimeUnits() == UnitGetMaxValid(DBUNITS_TIME))
	{
		SettingsUnitSetTimeUnits(0);
	}
	else
	{
		uint8_t CurrentUnits = SettingsUnitGetTimeUnits();
		SettingsUnitSetTimeUnits(++CurrentUnits);
	}
}

uint8_t SettingsUnitGetPressureUnits(void)
{
	return m_settings.PressureUnits;
}

uint8_t SettingsUnitGetTemperatureUnits(void)
{
	return m_settings.TemperatureUnits;
}

uint8_t SettingsUnitGetVolumeUnits(void)
{
	return m_settings.VolumeUnits;
}

uint8_t SettingsUnitGetDistanceUnits(void)
{
	return m_settings.DistanceUnits;
}

uint8_t SettingsUnitGetFuelRateUnits(void)
{
	return m_settings.FuelRateUnits;
}

uint8_t SettingsUnitGetSpeedUnits(void)
{
	return m_settings.SpeedUnits;
}

uint8_t SettingsUnitGetDepthUnits(void)
{
	return m_settings.DepthUnits;
}

uint8_t SettingsUnitGetAngleUnits(void)
{
	return m_settings.AngleUnits;
}

uint8_t SettingsUnitGetBearingUnits(void)
{
	return m_settings.BearingUnits;
}

uint8_t SettingsUnitGetEconomyUnits(void)
{
	return m_settings.EconomyUnits;
}

uint8_t SettingsUnitGetTimeUnits(void)
{
	return m_settings.TimeUnits;
}

uint8_t SettingsUnitGetUnits(uint32_t UnitType)
{
	int8_t ReturnValue = 0;

	switch (UnitType)
	{
	case DBUNITS_PRESSURE:
		ReturnValue = SettingsUnitGetPressureUnits();
		break;
	case DBUNITS_TEMPERATURE:
		ReturnValue = SettingsUnitGetTemperatureUnits();
		break;
	case DBUNITS_VOLUME:
		ReturnValue = SettingsUnitGetVolumeUnits();
		break;
	case DBUNITS_DISTANCE:
		ReturnValue = SettingsUnitGetDistanceUnits();
		break;
	case DBUNITS_FLOW_RATE:
		ReturnValue = SettingsUnitGetFuelRateUnits();
		break;
	case DBUNITS_SPEED:
		ReturnValue = SettingsUnitGetSpeedUnits();
		break;
	case DBUNITS_DEPTH:
		ReturnValue = SettingsUnitGetDepthUnits();
		break;
	case DBUNITS_BEARING:
		ReturnValue = SettingsUnitGetBearingUnits();
		break;
	case DBUNITS_ANGLE:
		ReturnValue = SettingsUnitGetAngleUnits();
		break;
	case DBUNITS_ECONOMY:
		ReturnValue = SettingsUnitGetEconomyUnits();
		break;
	case DBUNITS_TIME:
		ReturnValue = SettingsUnitGetTimeUnits();
		break;
	case DBUNITS_PERCENT:
	case DBUNITS_RPM:
	case DBUNITS_ELECTRICAL:
	case DBUNITS_COMMAND:
	case DBUNITS_CURRENT:
	case DBUNITS_FREQUENCY:
	case DBUNITS_REVS:
	case DBUNITS_NONE:
	case DBUNITS_RESISTANCE:
		ReturnValue = 0;
		break;

	case DBUNITS_END:
	default:
		assert("Missing Units!" && FALSE);
		break;
	}

	return ReturnValue;
}

uint8_t SettingsGetCANSystemInstance(uint32_t CANPort)
{
	if (CANPort < CAN_MAX_PORTS)
	{
		return m_settings.CANSystemInstance[CANPort];
	}
	else
	{
		return 0;
	}
}

void SettingsSetCANSystemInstance(uint32_t CANPort, uint8_t systemInstance)
{
	if (CANPort < CAN_MAX_PORTS)
	{
		m_settings.CANSystemInstance[CANPort] = systemInstance;
		SettingsSave();
	}
	else
	{
		assert("Index out of range!" && FALSE);
	}
}

uint8_t SettingsGetCANDeviceInstance(uint32_t CANPort)
{
	if (CANPort < CAN_MAX_PORTS)
	{
		return m_settings.CANDeviceInstance[CANPort];
	}
	else
	{
		return 0;
	}
}

void SettingsSetCANDeviceInstance(uint32_t CANPort, uint8_t deviceInstance)
{
	if (CANPort < CAN_MAX_PORTS)
	{
		m_settings.CANDeviceInstance[CANPort] = deviceInstance;
		SettingsSave();
	}
	else
	{
		assert("Index out of range!" && FALSE);
	}
}

// Load default settings to EEPROM and force a save
// This is called from syslib if the settings are being reset to defaults
void load_eeprom_defaults(void)
{
    // Set all the other defaults
    _SettingsLoadDefaults();

    // Force a save of the settings
    SettingsSave();

    // Make a 500ms beep
    buzzer_out_timed(500);
}

// Load settings defaults
static void _SettingsLoadDefaults(void)
{
	// Buzzer not muted
	m_settings.isBuzzerMuted = FALSE;                                      
	buzzer_mute(m_settings.isBuzzerMuted);

	m_settings.language = 0;

	m_settings.PressureUnits = 0;
	m_settings.TemperatureUnits = 0;
	m_settings.VolumeUnits = 0;
	m_settings.DistanceUnits = 0;
	m_settings.FuelRateUnits = 0;
	m_settings.SpeedUnits = 0;
	m_settings.DepthUnits = 0;
	m_settings.AngleUnits = 0;
	m_settings.BearingUnits = 0;
	m_settings.EconomyUnits = 0;
	m_settings.TimeUnits = 1;

	m_settings.CANDeviceInstance[CAN_PORT1] = 0;
	m_settings.CANSystemInstance[CAN_PORT1] = 0;
	m_settings.CANDeviceInstance[CAN_PORT2] = 0;
	m_settings.CANSystemInstance[CAN_PORT2] = 0;
}
