
#include "project.h"

static void _UARTViewerExit(void* userData);

static uint32_t m_eidHeaderText = ELEMENTID_INVALID;
static uint32_t m_eidData = ELEMENTID_INVALID;

void UARTViewerCreate(void)
{
	// Get our element ids
	m_eidHeaderText = ElementGetNewId();
	m_eidData = ElementGetNewId();

	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	MenuDrawTitleBar("");

	//Setup Keys
	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(48);
	ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _UARTViewerExit, nullptr);
}

void UARTViewerUpdate(void)
{
	const uint32_t dataStart_Y = MenuTitleBarGetHeight() + 5;
	const uint32_t dataGap_X = 40;
	const uint32_t dataGap_Y = 11;
	const uint32_t fontSize = 9;
	const uint32_t messagesPerScreen = (uint32_t)((lcd_get_height() - dataStart_Y - back.y) / dataGap_Y);

	char str[255];
	uint32_t screenPos;
	MessageQueueInfo_t queueInfo;

	GetQueueInfo(&queueInfo);

	// Clear Previous Header text and redraw
	ElementFillPrevRect(m_eidHeaderText, WHITE, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, fontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	sprintf(str, "UART PORT - RdIndex=%d, WrIndex=%d", queueInfo.NextReadIndex, queueInfo.NextWriteIndex);
	SimpleTextDrawEle(m_eidHeaderText, 5, MenuTitleBarGetHeight() - 2 - fontSize, str, WHITE_TEXT, 100, LAYER_FRONT);

	ElementFillPrevRect(m_eidData, WHITE, LAYER_FRONT);
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, fontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);

	for (screenPos = 0; screenPos < messagesPerScreen; screenPos++)
	{
		uint8_t* buf = PeekMessage(PeekTail, messagesPerScreen - screenPos - 1, (uint8_t*)str, sizeof(str));

		if (buf != nullptr)
		{
			SimpleTextDrawEle(m_eidData, (dataGap_X / 2), dataStart_Y + (dataGap_Y * screenPos), str, WHITE_TEXT, 100, LAYER_FRONT);
		}
	}
}

static void _UARTViewerExit(void* userData)
{
	MMIScreenGotoPrev();
}