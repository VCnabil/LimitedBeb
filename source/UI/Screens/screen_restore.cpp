
#include "project.h"

static void _ScreenRestoreConfirmed(void* userData);
static void _ScreenRestoreExit(void* userData);

void ScreenRestoreCreate(void)
{
	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	//Setup Keys
	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(48);
	ButtonBarSetKeyImages(KEYINDEX_1, &blank, &blank_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _ScreenRestoreConfirmed, nullptr);
	ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 18, BLACK, "OK",0);
	ButtonBarSetKeyImages(KEYINDEX_2, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_3, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_4, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _ScreenRestoreExit, nullptr);

	// Title background
	MenuDrawTitleBar(cucSystemWords[wRESTORE_DEFAULTS][SettingsGetLanguageIndex()]);

	// Ask user if they really want to restore the default settings
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 18, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(lcd_get_width() / 2, (lcd_get_height() / 2) - 10, cucSystemWords[wRESTORE_DEFAULTS_QUESTION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
}
// --------------------------------------------------------------------------->

static void _ScreenRestoreConfirmed(void* userData)
{
	// Restore the defaults
	load_eeprom_defaults();

	// Tell user settings have been restored to defaults
	blockfillEx(0, (lcd_get_height() / 2) - 30, lcd_get_width() - 1, (lcd_get_height() / 2) + 30, WHITE, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 18, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 2);
	SimpleTextDraw(lcd_get_width() / 2, (lcd_get_height() / 2) - 10, cucSystemWords[wRESTORE_DEFAULTS_DONE][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
}
// --------------------------------------------------------------------------->

static void _ScreenRestoreExit(void* userData)
{
	MMIScreenGotoPrev();
}
// --------------------------------------------------------------------------->
