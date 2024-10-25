
#include <math.h>
#include "syslib.h"
#include "Database/database.h"
#include "Database/unitconv.h"

#include "UI/buttonbar.h"

#include "UI/screens.h"

static void _DBViewerBack(void* userData);

static void _DBViewerTitleRow(void);
static void _DBViewerDrawDescription(void);

static void _DBViewer_DrawData(void);
static void _DBViewer_DrawUnits(void);

static void _DBViewerPageDown(void* userData);
static void _DBViewerPageUp(void* userData);

static void _DBViewerChangeInstance(void* userData);

static uint32_t m_DescriptionLeft = 10;
static uint32_t m_ValueRight = 250 - 3;
static uint32_t m_UnitsLeft = m_ValueRight + 6;

static uint32_t m_itemsOnScreen = 9;

static uint32_t m_yPosDelta;
static uint32_t m_StartingPositionY;
static uint32_t m_dataFontSize;
static uint32_t m_unitsFontSize;
static uint32_t m_descriptionFontSize;

static uint32_t m_titleRowFontSize = 14;

static uint32_t m_FirstItem = 0;

static uint32_t m_EleID_Data = ELEMENTID_INVALID;

static uint32_t m_Instance = 0;

void ScreenDBViewerEnter(void)
{
	m_StartingPositionY = MenuTitleBarGetHeight() + 3;
	m_yPosDelta = (lcd_get_height() - m_StartingPositionY - 3 - 28) / m_itemsOnScreen;
	m_dataFontSize = m_yPosDelta - 3;
	m_unitsFontSize = m_dataFontSize;
	m_descriptionFontSize = m_dataFontSize;
}

void ScreenDBViewerCreate(void)
{
	uint32_t currentScreen = 0;
	uint32_t NumberOfScreens = 0;
	char str[15];
	char titleString[50];

	sprintf(titleString, "%s (%d)", "DB Viewer", m_Instance + 1);

	vLcdBlankerEx(BLACK, ALPHA_COLOR);

	m_EleID_Data = ElementGetNewId();

	MenuDrawTitleBar(titleString);

	// Page X of X
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, 12, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	NumberOfScreens = ((DATABASEINDEX_MAX - 1) / m_itemsOnScreen) + 1;
	currentScreen = (m_FirstItem / m_itemsOnScreen) + 1;
	sprintf(str, "%s %u %s %u", "Page", currentScreen, "of", NumberOfScreens);
	SimpleTextDraw(5, 5, str, WHITE_TEXT, 100, LAYER_BACK);

	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetAnimation(BUTTONBARANIMATION_NONE);
	ButtonBarSetHeight(24);

	ButtonBarSetKeyImages(KEYINDEX_1, &blank, &blank_s);
	ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 14, BLACK, "Up", "");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _DBViewerPageUp, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_1, _DBViewerPageUp, nullptr, 2000, 500);

	ButtonBarSetKeyImages(KEYINDEX_2, &blank, &blank_s);
	ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 14, BLACK, "Down", "");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _DBViewerPageDown, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_2, _DBViewerPageDown, nullptr, 2000, 500);

	ButtonBarSetKeyImages(KEYINDEX_4, &blank, &blank_s);
	ButtonBarSetKeyText(KEYINDEX_4, FONT_INDEX_TTMAIN, 14, BLACK, "Device", "");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _DBViewerChangeInstance, nullptr);

	ButtonBarSetKeyImages(KEYINDEX_5, &blank, &blank_s);
	ButtonBarSetKeyText(KEYINDEX_5, FONT_INDEX_TTMAIN, 14, BLACK, "Back", "");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _DBViewerBack, nullptr);
	
	_DBViewerDrawDescription();
}

void ScreenDBViewerUpdate(void)
{
	ElementFillPrevRect(m_EleID_Data, WHITE, LAYER_FRONT);

	_DBViewer_DrawData();

	_DBViewer_DrawUnits();
}

static void _DBViewerBack(void* userData)
{
	MMIScreenGotoPrev();
}

static void _DBViewerTitleRow(void)
{
	uint32_t Height = MenuTitleBarGetHeight() - m_titleRowFontSize - 2;

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_titleRowFontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(m_DescriptionLeft, Height, "Description (SPN)", WHITE_TEXT, 100, LAYER_BACK);
	
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_titleRowFontSize, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(m_ValueRight, Height, "Value", WHITE_TEXT, 100, LAYER_BACK);

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_titleRowFontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	SimpleTextDraw(m_UnitsLeft, Height, "Units", WHITE_TEXT, 100, LAYER_BACK);
}

static void _DBViewerDrawDescription(void)
{
	uint32_t DescriptionIndex = 0;
	uint32_t YPos = m_StartingPositionY + 10;
	char descriptionString[50];

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_descriptionFontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	for (DescriptionIndex = m_FirstItem; DescriptionIndex < (m_FirstItem + m_itemsOnScreen); DescriptionIndex++)
	{
		if (DescriptionIndex < DATABASEINDEX_MAX)
		{
			Database_GetItemDescription(descriptionString, sizeof(descriptionString), DescriptionIndex);
			SimpleTextDraw(m_DescriptionLeft, YPos, descriptionString, WHITE_TEXT, 100, LAYER_BACK);
			YPos += m_yPosDelta;
		}
	}
}

static void _DBViewer_DrawData(void)
{
	uint32_t DataIndex = 0;
	uint32_t YPos = m_StartingPositionY + 10;

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_dataFontSize, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
	for (DataIndex = m_FirstItem; DataIndex < (m_FirstItem + m_itemsOnScreen); DataIndex++)
	{
		if (DataIndex < DATABASEINDEX_MAX)
		{
			DBELEMENT_T DB_Element;
			char Str[255];

			if (DataBase_Get(&DB_Element, DataIndex, m_Instance))
			{
				UnitConvert_To_Current_Units(&DB_Element);
				Database_snprintf(Str, sizeof(Str), DatabaseGetPrecisionString(DataIndex), &DB_Element);
			}
			else
			{
				sprintf((char*)Str, "%s", "N/A");
			}
			SimpleTextDrawEle(m_EleID_Data, m_ValueRight, YPos, Str, WHITE_TEXT, 100, LAYER_FRONT);
			YPos += m_yPosDelta;
		}
	}
}

static void _DBViewer_DrawUnits(void)
{
	uint32_t UnitIndex = 0;
	uint32_t YPos = m_StartingPositionY + 10;

	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, m_unitsFontSize, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
	for (UnitIndex = m_FirstItem; UnitIndex < (m_FirstItem + m_itemsOnScreen); UnitIndex++)
	{
		if (UnitIndex < DATABASEINDEX_MAX)
		{
			DBELEMENT_T DB_Element;
			char Str[255];

			if (DataBase_Get(&DB_Element, UnitIndex, m_Instance))
			{
				Database_GetUnitString(Str, sizeof(Str), &DB_Element);
			}
			else
			{
				sprintf((char*)Str, "%s", "");
			}
			SimpleTextDrawEle(m_EleID_Data, m_UnitsLeft, YPos, Str, WHITE_TEXT, 100, LAYER_FRONT);
			YPos += m_yPosDelta;
		}
	}
}

static void _DBViewerPageDown(void* userData)
{
	if (m_FirstItem + m_itemsOnScreen < DATABASEINDEX_MAX)
	{
		m_FirstItem += m_itemsOnScreen;
	}
	else
	{
		m_FirstItem = 0;
	}
	MMIInvalidate();
}

static void _DBViewerPageUp(void* userData)
{
	uint32_t ResetValue = 0;
	uint32_t NumberOfScreens = 0;

	// (Its a uint32_t so will wrap
	if ((m_FirstItem - m_itemsOnScreen) > DATABASEINDEX_MAX)
	{
		NumberOfScreens = ((DATABASEINDEX_MAX - 1) / m_itemsOnScreen) + 1;
		ResetValue = ((NumberOfScreens - 1) * m_itemsOnScreen);
		m_FirstItem = ResetValue;
	}
	else
	{
		m_FirstItem -= m_itemsOnScreen;
	}
	MMIInvalidate();
}

static void _DBViewerChangeInstance(void* userData)
{
	if (m_Instance >= (DB_INSTANCE_INVALID - 1))
	{
		m_Instance = 0;
	}
	else
	{
		m_Instance++;
	}
	MMIInvalidate();
}
