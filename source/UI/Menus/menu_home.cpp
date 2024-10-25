
#include "project.h"

static void _MenuHomeCreate(void* userData);
static void _MenuHomeRestoreDefaults(void* userData);
static void _MenuHomeAbout(void* userData);

void MenuHomeEnter(void)
{
	// Create our menu
	MenuCreateNew(_MenuHomeCreate, nullptr);
}

static void _MenuHomeCreate(void* userData)
{
	uint32_t currentItem = 0;

	MenuSetTitle(cucSystemWords[wCONFIG_MENU][SettingsGetLanguageIndex()]);
	
	MenuSetItem(currentItem++, cucSystemWords[wSETTING][SettingsGetLanguageIndex()], 0, MenuSettingsEnter, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wDIAGNOSTICS][SettingsGetLanguageIndex()], 0, MenuDiagnosticsEnter, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wRESTORE_DEFAULTS][SettingsGetLanguageIndex()], 0, _MenuHomeRestoreDefaults, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wABOUT][SettingsGetLanguageIndex()], 0, _MenuHomeAbout, nullptr);
}

static void _MenuHomeRestoreDefaults(void* userData)
{
	MMIScreenGoto(SCREENID_RESTORE);
}

static void _MenuHomeAbout(void* userData)
{
	MMIScreenGoto(SCREENID_ABOUT);
}