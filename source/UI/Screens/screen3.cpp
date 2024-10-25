#include "project.h"
#include <string>

// Private Variables and Functions

//Selected Item
int item = 0;

//Button Bar
static void _Key1Release(void* userData);
static void _Key2Release(void* userData);
static void _Key3Release(void* userData);
static void _Key4Release(void* userData);
static void _Key5Release(void* userData);

void Screen3Enter(void)
{

}

void Screen3Create(void)
{
	vLcdBlankerEx(WHITE, ALPHA_COLOR);

	ScreensSetupDefaultKeys();
	ButtonBarSetKeyImages(KEYINDEX_1, &view_meters, &view_meters);
	ButtonBarSetKeyImages(KEYINDEX_2, &up, &up);
	ButtonBarSetKeyImages(KEYINDEX_3, &down, &down);
	ButtonBarSetKeyImages(KEYINDEX_4, &toggle, &toggle);
	ButtonBarSetKeyImages(KEYINDEX_5, &blank, &blank);

	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);

	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _Key2Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _Key3Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _Key4Release, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _Key5Release, nullptr);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 25, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 0);
	SimpleTextDraw(lcd_get_width() / 2, 5, "System Options", BLACK, 100, LAYER_BACK);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);

	SimpleTextDraw(10, 60, "DatMode = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(10, 90, "232 Xmit = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(10, 120, "NozMap = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(10, 150, "BktMap = ", BLACK, 100, LAYER_BACK);

	SimpleTextDraw(150, 60, "IntMap = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(150, 90, "IntSteer = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(150, 120, "StaType = ", BLACK, 100, LAYER_BACK);
	SimpleTextDraw(150, 150, "Comm Mode = ", BLACK, 100, LAYER_BACK);
}

void Screen3Update(void)
{
	fill_lcd_screen(WHITE, LAYER_FRONT);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 15, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 0);
	for (int i = 0; i < system_options_count / 2; i++) {
		SimpleTextDraw(25 + (strlen(m_SYSTEMOPTIONS_DB[i].name) * 10), 60 + (i * 30), m_SYSTEMOPTIONS_DB[i].status ? m_SYSTEMOPTIONS_DB[i].on : m_SYSTEMOPTIONS_DB[i].off, BLACK, 100, LAYER_FRONT);
	}
	for (int i = 4; i < system_options_count; i++) {
		SimpleTextDraw(165 + (strlen(m_SYSTEMOPTIONS_DB[i].name) * 10), 60 + ((i - 4) * 30), m_SYSTEMOPTIONS_DB[i].status ? m_SYSTEMOPTIONS_DB[i].on : m_SYSTEMOPTIONS_DB[i].off, BLACK, 100, LAYER_FRONT);
	}

	if (item < 4) {
		rectangleEx(5, 55 + (item * 30), 145, 85 + (item * 30), NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
	}
	else {
		rectangleEx(145, 55 + ((item - 4) * 30), 315, 85 + ((item - 4) * 30), NORMAL, SOLID, BLACK, 100, LAYER_FRONT);
	}


}

void Screen3Exit(void)
{

}

static void _Key1Release(void* userData)
{
	MMIScreenGoto(SCREENID_1);
}

static void _Key2Release(void* userData)
{
	
	if(item > 0) item--;
}

static void _Key3Release(void* userData)
{
	if(item < 7) item++;
}

static void _Key4Release(void* userData)
{
	m_SYSTEMOPTIONS_DB[item].status = !m_SYSTEMOPTIONS_DB[item].status;
}

static void _Key5Release(void* userData)
{

}