
#include "project.h"

static void _MenuUnitsEnterCreate(void* userData = nullptr);
static void _UnitsMenu_Pressure(void* userData);
static void _UnitsMenu_Temp(void* userData);
static void _UnitsMenu_Volume(void* userData);
static void _UnitsMenu_Distance(void* userData);
static void _UnitsMenu_FlowRate(void* userData);
static void _UnitsMenu_Speed(void* userData);
static void _UnitsMenu_Depth(void* userData);
static void _UnitsMenu_Angle(void* userData);
static void _UnitsMenu_Bearing(void* userData);
static void _UnitsMenu_Economy(void* userData);

void MenuUnitsEnter(void* userData)
{
	// Register our menu
	MenuCreateNew(_MenuUnitsEnterCreate, nullptr);
}

static void _MenuUnitsEnterCreate(void* userData)
{
	uint32_t currentItem = 0;
	MenuSetTitle(cucSystemWords[wUNITS_MENU][SettingsGetLanguageIndex()]);

	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_PRESSURE), UnitGetStringCurrentUnits(DBUNITS_PRESSURE), _UnitsMenu_Pressure, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_TEMPERATURE), UnitGetStringCurrentUnits(DBUNITS_TEMPERATURE), _UnitsMenu_Temp, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_VOLUME), UnitGetStringCurrentUnits(DBUNITS_VOLUME), _UnitsMenu_Volume, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_DISTANCE), UnitGetStringCurrentUnits(DBUNITS_DISTANCE), _UnitsMenu_Distance, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_FLOW_RATE), UnitGetStringCurrentUnits(DBUNITS_FLOW_RATE), _UnitsMenu_FlowRate, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_SPEED), UnitGetStringCurrentUnits(DBUNITS_SPEED), _UnitsMenu_Speed, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_DEPTH), UnitGetStringCurrentUnits(DBUNITS_DEPTH), _UnitsMenu_Depth, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_ANGLE), UnitGetStringCurrentUnits(DBUNITS_ANGLE), _UnitsMenu_Angle, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_BEARING), UnitGetStringCurrentUnits(DBUNITS_BEARING), _UnitsMenu_Bearing, nullptr);
	MenuSetItem(currentItem++, UnitGetStringDescriptor(DBUNITS_ECONOMY), UnitGetStringCurrentUnits(DBUNITS_ECONOMY), _UnitsMenu_Economy, nullptr);
}

static void _UnitsMenu_Pressure(void* userData)
{
	SettingsUnitsTogglePressure();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Temp(void* userData)
{
	SettingsUnitsToggleTemperature();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Volume(void* userData)
{
	SettingsUnitsToggleVolume();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Distance(void* userData)
{
	SettingsUnitsToggleDistance();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_FlowRate(void* userData)
{
	SettingsUnitsToggleFuelRate();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Speed(void* userData)
{
	SettingsUnitsToggleSpeed();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Depth(void* userData)
{
	SettingsUnitsToggleDepth();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Angle(void* userData)
{
	SettingsUnitsToggleAngle();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Bearing(void* userData)
{
	SettingsUnitsToggleBearing();
	_MenuUnitsEnterCreate();
}

static void _UnitsMenu_Economy(void* userData)
{
	SettingsUnitsToggleEconomy();
	_MenuUnitsEnterCreate();
}
