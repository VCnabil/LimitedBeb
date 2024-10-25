//------------------------------------------------------------------------------
// screens.c
//------------------------------------------------------------------------------

#include "project.h"

static void _Key5Release(void* userData);
static void _Key5Hold(void* userData);

#if defined(ENABLE_FPS_COUNTER)
	// If this is defined it will display a frames per second count on the screen
	static void _DisplayFPSCounter(void* pUserData = nullptr);
#endif

void ScreensInit(void)
{
	// User screens
	MMIRegisterScreen(SCREENID_1, Screen1Enter, Screen1Create, Screen1Update, Screen1Exit);
	MMIRegisterScreen(SCREENID_2, Screen2Enter, Screen2Create, Screen2Update, Screen2Exit);
	MMIRegisterScreen(SCREENID_3, Screen3Enter, Screen3Create, Screen3Update, Screen3Exit);
	MMIRegisterScreen(SCREENID_4, Screen4Enter, Screen4Create, Screen4Update, Screen4Exit);

	// Passcode screen
	MMIRegisterScreen(SCREENID_PASSCODE, ScreenPasscodeEnter, ScreenPasscodeCreate, ScreenPasscodeUpdate, ScreenPasscodeExit);

	// Debug screen
	MMIRegisterScreen(SCREENID_DEBUG, ScreenDebugEnter, ScreenDebugCreate, ScreenDebugUpdate, ScreenDebugExit);

	// The menu
	MMIRegisterScreen(SCREENID_MENU, MenuScreenEnter, MenuScreenCreate, 0, MenuScreenExit);

	// About
	MMIRegisterScreen(SCREENID_ABOUT, 0, ScreenAboutCreate, 0, 0);

	// Restore Defaults

	// Keyboard
	MMIRegisterScreen(SCREENID_KEYBOARD, KeyboardEnter, 0, 0, 0);

	// Initial screen.
	MMIScreenGoto(SCREENID_1);

#if defined(ENABLE_FPS_COUNTER)
	// Enable the Frames Per Second counter on the screen
	MMIRegisterCallback(MMICALLBACKTYPE_POSTUPDATE, _DisplayFPSCounter, nullptr);
#endif
}

void ScreensSetupDefaultKeys(void)
{
	// Use this function to setup any keys that need to be setup the same for
	// all you screens. This can then be called from the screen create functions.

	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetAnimation(BUTTONBARANIMATION_SLIDE);
	ButtonBarSetHeight(48);

	// Setup Key 5 Hold for menu access
	ButtonBarSetKeyImages(KEYINDEX_5, &lighting_ico, &lighting_ico_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _Key5Release, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_5, _Key5Hold, nullptr, 1000, 0);
}

static void _Key5Release(void* userData)
{
	// Backlight change
	LightingEnter();
}

static void _Key5Hold(void* userData)
{
	// Display menu
	MMIScreenGoto(SCREENID_MENU);
}

#if defined(ENABLE_FPS_COUNTER)
	// If this is defined it will display a frames per second count on the screen
	static void _DisplayFPSCounter(void* pUserData)
	{
		(void)pUserData;
		
	#define FRAME_X (lcd_get_width() / 2 - 24)
	#define FRAME_Y 1

		char str[50];

		// Draw the FPS on the screen
		blockfillEx(FRAME_X, FRAME_Y, FRAME_X + 48, FRAME_Y + 12, YELLOW, 100, LAYER_FRONT);
		sprintf(str, "FPS %u", GetMainLoopCurrentFPS());
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 10, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);
		SimpleTextDraw((FRAME_X + 24), (FRAME_Y + 1), str, BLACK, 100, LAYER_FRONT);
	#undef FRAME_X
	#undef FRAME_Y
	}
#endif // #if defined(MMI_ENABLE_FRAME_COUNTER)
