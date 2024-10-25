
#include "project.h"
#include "revReg.h"
#include "UIGaugeVersion.h"

#define LEFT_TEXT		157
#define RIGHT_TEXT		(LEFT_TEXT + 3)

#define Y_START			34
#define Y_END			192
#define Y_TOTAL			(Y_END - Y_START)
#define TEXT_SIZE		11
#define NUMBER_OF_ROWS	13
#define Y_OFFSET		((Y_TOTAL - (NUMBER_OF_ROWS - TEXT_SIZE)) / (NUMBER_OF_ROWS + 1))

static void _ScreenAboutExit(void* userData);

void ScreenAboutCreate(void)
{
	char str[50];

	// Clear the Screen
	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	MenuDrawTitleBar("ABOUT");

	//Setup Keys
	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(48);
	ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _ScreenAboutExit, nullptr);

	uint32_t currentHeight = Y_START;

	// Part Number / Model Number
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_PART][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%s", PART_NUMBER);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Software Name
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SW_NAME][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%s", SOFTWARE);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Software Version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SW_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%u.%u.%u", SWVER_MAJOR, SWVER_MINOR, SWVER_PATCH);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Product ID
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_PRODUCT_ID][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "0x%x (%s)", GetPlatformProductId(), GetPlatformProductName(GetPlatformProductId()));
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Serial Number
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SERIAL][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%u", GetPlatformProductSerial());
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// SDKC version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SDKC_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%s", SDKC_VERSION_STR);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Firmware version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_OS_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "B%u-K%u-RF%u", GetPlatformBootloaderVersion(), GetPlatformOSVersion(), GetPlatformRootFSVersion());
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// Syslib version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SYSLIB_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%s", SYSLIB_VERSION_STR);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// UIGauge version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_UIGAUGE_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	snprintf(str, sizeof(str), "%i.%i.%i", UI_GAUGE_VERSION_MAJOR, UI_GAUGE_VERSION_MINOR, UI_GAUGE_VERSION_PATCH);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// IO version
	uint32_t bootversion;
	uint32_t appversion;
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_IO_VERSION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	if (io_get_version_info(&bootversion, &appversion) == IO_STATUS_SUCCESS)
	{
		sprintf(str, "M%u-B%u", appversion, bootversion);
	}
	else
	{
		sprintf(str, "NOT RUNNING");
	}
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// IP Address version
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_IP_ADDRESS][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	IpAddressGet(str, sizeof(str));
	SimpleTextDraw(RIGHT_TEXT, currentHeight, str, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;

	// SVN Revision
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(LEFT_TEXT, currentHeight, cucSystemWords[wABOUT_SVN_REVISION][SettingsGetLanguageIndex()], WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(RIGHT_TEXT, currentHeight, SVN_REVISION, WHITE_TEXT, 100, LAYER_FRONT);
	currentHeight += Y_OFFSET;
}

static void _ScreenAboutExit(void* userData)
{
	MMIScreenGotoPrev();
}
