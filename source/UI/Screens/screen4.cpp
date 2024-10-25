#include "project.h"

// Private Variables and Functions

//Button Bar
static void _Key1Release(void* userData);
static void _Key2Release(void* userData);
static void _Key3Release(void* userData);
static void _Key4Release(void* userData);
static void _Key5Release(void* userData);


void Screen4Enter(void)
{

}

void Screen4Create(void)
{
	vLcdBlankerEx(WHITE, ALPHA_COLOR);

	ScreensSetupDefaultKeys();
	ButtonBarSetKeyImages(KEYINDEX_1, &view_meters, &view_meters);
	ButtonBarSetKeyImages(KEYINDEX_2, &blank, &blank);
	ButtonBarSetKeyImages(KEYINDEX_3, &thrust_calib, &thrust_calib);
	ButtonBarSetKeyImages(KEYINDEX_4, &init_cal, &init_cal);
	ButtonBarSetKeyImages(KEYINDEX_5, &blank, &blank);

	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);

	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _Key2Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _Key3Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _Key4Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _Key5Release, nullptr);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 25, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 0);
	SimpleTextDraw(lcd_get_width() / 2, 5, "System Calibration", BLACK, 100, LAYER_BACK);



}

void Screen4Update(void)
{
	fill_lcd_screen(WHITE, LAYER_FRONT);


}

void Screen4Exit(void)
{

}

static void _Key1Release(void* userData)
{
	MMIScreenGoto(SCREENID_1);
}

static void _Key2Release(void* userData)
{

}

static void _Key3Release(void* userData)
{

}

static void _Key4Release(void* userData)
{

}

static void _Key5Release(void* userData)
{

}