#include "project.h"

static uint32_t m_eidHeaderText = ELEMENTID_INVALID;
static uint32_t m_eidData = ELEMENTID_INVALID;

static void _Key1Release(void* userData);


void ScreenDebugEnter(void)
{

}

void ScreenDebugCreate(void)
{
	// Setup default keys
	ScreensSetupDefaultKeys();

	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _Key1Release, nullptr);

	ButtonBarSetMode(BUTTONBARMODE_INVISIBLE);

	// Clear layers
	vLcdBlankerEx(WHITE, ALPHA_COLOR);

	// Text output (to layer 1)
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 25, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 0);
	SimpleTextDraw(lcd_get_width() / 2, 5, "Debug Screen", BLACK, 100, LAYER_BACK);
}

void ScreenDebugUpdate(void)
{
	
	fill_lcd_screen(WHITE, LAYER_FRONT);
	
	const uint32_t dataStart_Y = MenuTitleBarGetHeight() + 5;
	const uint32_t dataGap_X = 40;
	const uint32_t dataGap_Y = 11;
	const uint32_t fontSize = 8;
	const uint32_t messagesPerScreen = (uint32_t)((lcd_get_height() - dataStart_Y - back.y) / dataGap_Y);

	char str[255] = { 0 };
	uint32_t screenPos;
	MessageQueueInfo_t queueInfo;

	GetQueueInfo(&queueInfo);

	// Clear Previous Header text and redraw
	ElementFillPrevRect(m_eidHeaderText, WHITE, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, fontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	sprintf(str, "UART PORT - RdIndex=%d, WrIndex=%d msgs:%d", queueInfo.NextReadIndex, queueInfo.NextWriteIndex, messagesPerScreen);
	SimpleTextDrawEle(m_eidHeaderText, 5, MenuTitleBarGetHeight() - 2 - fontSize + 100, str, BLACK, 100, LAYER_FRONT);

	ElementFillPrevRect(m_eidData, WHITE, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, fontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);

	uint8_t* buf = PeekMessage(PeekTail, 1, (uint8_t*)str, sizeof(str));
	SimpleTextDrawEle(m_eidData, (dataGap_X / 2), dataStart_Y + 100, str, BLACK, 100, LAYER_FRONT);
}

void ScreenDebugExit(void)
{
}

static void _Key1Release(void* userData)
{
	MMIScreenGoto(SCREENID_1);
}

