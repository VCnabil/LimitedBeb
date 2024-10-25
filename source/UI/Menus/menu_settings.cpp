
#include "project.h"

static void _MenuSettingsBleep(void* userData);
static void _MenuSettingsCreate(void* userData);
// --------------------------------------------------------------------------->

void MenuSettingsEnter(void* userData)
{
	// Creating our menu
	MenuCreateNew(_MenuSettingsCreate, nullptr);
}
// --------------------------------------------------------------------------->

static void _MenuSettingsCreate(void* userData)
{
	uint32_t currentItem = 0;
	char strLeft[100];
	char strRight[100];
	
	MenuSetTitle(cucSystemWords[wSETTINGS_MENU][SettingsGetLanguageIndex()]);

	MenuSetItem(currentItem++, cucSystemWords[wLANGUAGE][SettingsGetLanguageIndex()], 0, MenuLanguageEnter, nullptr);

	if (SettingsIsBleepEnabled())
	{
		sprintf(strLeft,
			"%s", 
			cucSystemWords[wBLEEP][SettingsGetLanguageIndex()]);
		sprintf(strRight,
			"%s", 
			cucSystemWords[wOFF][SettingsGetLanguageIndex()]);
		MenuSetItem(currentItem++, strLeft, strRight, _MenuSettingsBleep, nullptr);
	}
	else
	{
		sprintf(strLeft,
			"%s", 
			cucSystemWords[wBLEEP][SettingsGetLanguageIndex()]);
		sprintf(strRight,
			"%s", 
			cucSystemWords[wON][SettingsGetLanguageIndex()]);
		MenuSetItem(currentItem++, strLeft, strRight, _MenuSettingsBleep, nullptr);
	}

	MenuSetItem(currentItem++, cucSystemWords[wUNITS_MENU][SettingsGetLanguageIndex()], NULL, MenuUnitsEnter, nullptr);
}
// --------------------------------------------------------------------------->

static void _MenuSettingsBleep(void* userData)
{
	char strLeft[255];
	char strRight[255];

	SettingsToggleBleep();

	sprintf(strLeft,
		"%s", 
		cucSystemWords[wBLEEP][SettingsGetLanguageIndex()]);
	sprintf(strRight,
		"%s", 
		cucSystemWords[SettingsIsBleepEnabled() ? wOFF : wON][SettingsGetLanguageIndex()]);
	MenuSetItem(1, strLeft, strRight, _MenuSettingsBleep, nullptr);

	MMIInvalidate();
}
// --------------------------------------------------------------------------->
