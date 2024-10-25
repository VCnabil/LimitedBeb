//------------------------------------------------------------------------------
// File:        settings.h
// Description: System settings.
//------------------------------------------------------------------------------

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "project.h"

/// @defgroup settings Settings Functions
/// Functions for handling the Settings.
/// @{

//------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// PUBLIC FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

/// Initialize the settings
void SettingsInit(void);

/// Function to Load the settings and apply them.
void SettingsLoad(void);

/// Function to Save the current settings.
void SettingsSave(void);

/// Get the currently used language.
/// @returns the language ID as a number
uint32_t SettingsGetLanguageIndex(void);

/// Sets the currently used language 
///@param languageIndex - Index number for the language to be used.
void SettingsSetLanguageIndex(uint32_t languageIndex);

/// Get the current state of the buzzer
///@returns TRUE if the buzzer is enabled, FALSE otherwise
BOOL SettingsIsBleepEnabled(void);

/// Toggles the Buzzer ON/OFF.
void SettingsToggleBleep(void);

/// Units related Functions
void SettingsUnitSetPressureUnits(uint8_t NewUnits);
void SettingsUnitSetTemperatureUnits(uint8_t NewUnits);
void SettingsUnitSetVolumeUnits(uint8_t NewUnits);
void SettingsUnitSetDistanceUnits(uint8_t NewUnits);
void SettingsUnitSetFuelRateUnits(uint8_t NewUnits);
void SettingsUnitSetSpeedUnits(uint8_t NewUnits);
void SettingsUnitSetDepthUnits(uint8_t NewUnits);
void SettingsUnitSetAngleUnits(uint8_t NewUnits);
void SettingsUnitSetBearingUnits(uint8_t NewUnits);
void SettingsUnitSetEconomyUnits(uint8_t NewUnits);
void SettingsUnitSetTimeUnits(uint8_t NewUnits);

void SettingsUnitsTogglePressure(void* userData = nullptr);
void SettingsUnitsToggleTemperature(void* userData = nullptr);
void SettingsUnitsToggleVolume(void* userData = nullptr);
void SettingsUnitsToggleDistance(void* userData = nullptr);
void SettingsUnitsToggleFuelRate(void* userData = nullptr);
void SettingsUnitsToggleSpeed(void* userData = nullptr);
void SettingsUnitsToggleDepth(void* userData = nullptr);
void SettingsUnitsToggleAngle(void* userData = nullptr);
void SettingsUnitsToggleBearing(void* userData = nullptr);
void SettingsUnitsToggleEconomy(void* userData = nullptr);
void SettingsUnitsToggleTime(void* userData = nullptr);

uint8_t SettingsUnitGetPressureUnits(void);
uint8_t SettingsUnitGetTemperatureUnits(void);
uint8_t SettingsUnitGetVolumeUnits(void);
uint8_t SettingsUnitGetDistanceUnits(void);
uint8_t SettingsUnitGetFuelRateUnits(void);
uint8_t SettingsUnitGetSpeedUnits(void);
uint8_t SettingsUnitGetDepthUnits(void);
uint8_t SettingsUnitGetAngleUnits(void);
uint8_t SettingsUnitGetBearingUnits(void);
uint8_t SettingsUnitGetEconomyUnits(void);
uint8_t SettingsUnitGetTimeUnits(void);
uint8_t SettingsUnitGetUnits(uint32_t UnitType);

uint8_t SettingsGetCANSystemInstance(uint32_t CANPort);
void SettingsSetCANSystemInstance(uint32_t CANPort, uint8_t systemInstance);
uint8_t SettingsGetCANDeviceInstance(uint32_t CANPort);
void SettingsSetCANDeviceInstance(uint32_t CANPort, uint8_t deviceInstance);

/// Load default settings and force a save
void load_eeprom_defaults(void);

/// @} // endgroup settings

#endif  // #ifndef __SETTINGS_H__
