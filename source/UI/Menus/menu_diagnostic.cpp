
#include "project.h"

static void _MenuDiagnosticsCreate(void* userData);
static void _MenuDiagsCANViewer(void* userData);
static void _MenuDiagsUARTViewer(void* userData);
static void _MenuDiagsDBViewer(void* userData);

void MenuDiagnosticsEnter(void* userData)
{
	// Register our menu
	MenuCreateNew(_MenuDiagnosticsCreate, nullptr);
}

static void _MenuDiagnosticsCreate(void* userData)
{
	uint32_t currentItem = 0;

	MenuSetTitle(cucSystemWords[wDIAGNOSTICS_MENU][SettingsGetLanguageIndex()]);

	MenuSetItem(currentItem++, cucSystemWords[wCANVIEWER][SettingsGetLanguageIndex()], 0, _MenuDiagsCANViewer, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wUARTVIEWER][SettingsGetLanguageIndex()], 0, _MenuDiagsUARTViewer, nullptr);
	MenuSetItem(currentItem++, cucSystemWords[wDBVIEWER][SettingsGetLanguageIndex()], 0, _MenuDiagsDBViewer, nullptr);
}

static void _MenuDiagsCANViewer(void* userData)
{
	MMIScreenGoto(SCREENID_CANVIEWER);
}

static void _MenuDiagsUARTViewer(void* userData)
{
	MMIScreenGoto(SCREENID_UARTVIEWER);
}

static void _MenuDiagsDBViewer(void* userData)
{
	MMIScreenGoto(SCREENID_DBVIEWER);
}