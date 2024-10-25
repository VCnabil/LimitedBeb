
//------------------------------------------------------------------------------
//  TITLE :          config.c
//  AUTHOR :
//
//  DATE :
//
//  DESCRIPTION :
//
//
//  UPDATES :-
//------------------------------------------------------------------------------

#include "project.h"

// The number of pixels to leave as a border
#define MENU_X_BORDER	80
#define MENU_Y_BORDER	40    

/// The size of the menu stack
#define MENU_STACK_SIZE	5

/// The maximum number of menu button rows
#define MENU_MAX_ROWS					20

/// The height of the menu title bar
#define MENU_TITLE_HEIGHT				26

/// The text size of the menu title
#define MENU_TITLE_TEXT_SIZE				20

/// The maximum number of rows that fit in the display
#define MENU_DISPLAY_ROWS				6

/// Size of the menu under text
#define MENU_TEXT_SIZE	18

/// Line height
#define LINE_HEIGHT  27

/// Maximum length of row text
#define MENU_ROW_TEXT_MAXLEN			100

/// Spacing for the left text
#define MENU_TEXT_LEFT_SPACE			25

/// Spacing for the right text
#define MENU_TEXT_RIGHT_SPACE			25

/// The colour of the menu background
#define MENU_BARBACKGROUND_COLOUR		MAKERGB565(0,84,96)

/// The colour of the menu title
#define MENU_TITLE_COLOUR				MAKERGB565(88,100,96)

/// The colour of the menu title text
#define MENU_TITLE_TEXT_COLOUR			MAKERGB565(250,250,250)

/// Non selected item colours
#define MENU_ITEM_COLOUR				MAKERGB565(4,7,249)

/// The colour of the left text non selected items
#define MENU_ITEM_LEFT_TEXT_COLOUR		MAKERGB565(254,254,254)

/// The colour of the right text non selected items
#define MENU_ITEM_RIGHT_TEXT_COLOUR		MAKERGB565(255,255,0)

/// Selected item colour
#define MENU_ITEM_SELECTED_COLOUR		MAKERGB565(223,231,58)

/// Selected item text colour
#define MENU_ITEM_SELECTED_TEXT_COLOUR	MAKERGB565(1,1,1)

/// Opacity default is fully opaque
#define MENU_ITEM_OPACITY				100

//Used in storing the left and right text
typedef struct
{
	//Callback function for the currently selected menu item
	MENUCALLBACKFUNC_T pButton3FuncSelected;
	void* button3UserData;
	char button3Str[MENU_ROW_TEXT_MAXLEN];

	MENUCALLBACKFUNC_T pButton4FuncSelected;
	void* button4UserData;
	char button4Str[MENU_ROW_TEXT_MAXLEN];

	// Left text of the current menu row
	char labelStr[MENU_ROW_TEXT_MAXLEN];
	// Right text of the current menu row
	char valueStr[MENU_ROW_TEXT_MAXLEN];
} MENUENTRY_T;

typedef struct
{
	// Number of rowns
	uint32_t rows;
	// Menu title.
	char pTitleStr[MENU_ROW_TEXT_MAXLEN];
	// Total number of Item Entries in the current Menu
	MENUENTRY_T text[MENU_MAX_ROWS];
	// Index of the current selected row
	uint32_t rowIndexSelection;
	//Current row from where the rendering starts
	uint32_t drawRowIndex;
	//Current selected Row for drawing
	uint32_t selectionBoxIndex;
	// The create function for the menu
	MENUCREATEFUNC_T pCreateFunc;
	// The create user data for the menu
	void* createFuncUserData;
} MENU_T;

static void _MenuSetup(MENU_T* pMenu);
static void _MenuCalcLayout(MENU_T* pEntry);
static void _MenuDrawItems(MENU_T* pMenu);
static void _MenuUp(void* userData);
static void _MenuDown(void* userData);

// The menu stack
static MENU_T m_menuStack[MENU_STACK_SIZE];
static int32_t m_menuStackIndex = -1;

// Menu layout
// Y starting position used for drawing
static int32_t m_startY = 0;

void MenuInit(void)
{

}

void MenuScreenEnter(void)
{
	// 	Is the stack empty?
	if (m_menuStackIndex == -1)
	{
		// Yes. We are not being called to come back from a menu item so go to the top level home menu
		MenuHomeEnter();
	}
	else
	{
		// Re-setup the menu
		_MenuSetup(&m_menuStack[m_menuStackIndex]);
	}
}

void MenuScreenCreate(void)
{
	if (m_menuStackIndex >= 0)
	{
		// Clear the screen
		vLcdBlankerEx(MENU_BARBACKGROUND_COLOUR, ALPHA_COLOR);

		// Create title bar
		MenuDrawTitleBar(m_menuStack[m_menuStackIndex].pTitleStr);

		// Draw the menu items
		_MenuDrawItems(&m_menuStack[m_menuStackIndex]);
	}
	else
	{
		MenuQuit();
	}
}

void MenuScreenExit(void)
{
	// Save any settings that have changed
	SettingsSave();
}

void MenuCreateNew(MENUCREATEFUNC_T pCreateFunc, void* userData)
{
	// Enough space in stack?
	if (m_menuStackIndex < (MENU_STACK_SIZE - 1))
	{
		// Add info to stack
		++m_menuStackIndex;
		memset(&m_menuStack[m_menuStackIndex], 0, sizeof(m_menuStack[m_menuStackIndex]));
		m_menuStack[m_menuStackIndex].pCreateFunc = pCreateFunc;
		m_menuStack[m_menuStackIndex].createFuncUserData = userData;

		// Setup the menu
		_MenuSetup(&m_menuStack[m_menuStackIndex]);
	}
}

void MenuSetTitle(const char* pTitleStr)
{
	if (pTitleStr != 0)
	{
		snprintf(m_menuStack[m_menuStackIndex].pTitleStr, MENU_ROW_TEXT_MAXLEN, "%s", pTitleStr);
	}
}

void MenuDrawTitleBar(const char * titleString)
{
	// Title background
	blockfillEx(0, 0, lcd_get_width() - 1, MENU_TITLE_HEIGHT, MENU_TITLE_COLOUR, 100, LAYER_BACK);
	blockfillEx(0, MENU_TITLE_HEIGHT, lcd_get_width() - 1, MENU_TITLE_HEIGHT + 2, DARK_GRAY, 100, LAYER_BACK);

	// Title
	SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, MENU_TITLE_TEXT_SIZE, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 2);
	SimpleTextDraw(lcd_get_width() / 2, 4, titleString, MENU_TITLE_TEXT_COLOUR, 100, LAYER_BACK);
}

uint32_t MenuTitleBarGetHeight(void)
{
	return MENU_TITLE_HEIGHT;
}

void MenuSetItem(uint32_t itemNumber, const char* pLableText, const char* pValueText, MENUCALLBACKFUNC_T pSelectedCB, void* userData)
{
	if (itemNumber < MENU_MAX_ROWS)
	{
		if (itemNumber >= m_menuStack[m_menuStackIndex].rows)
		{
			m_menuStack[m_menuStackIndex].rows = itemNumber + 1;
		}

		// If we don't specify any Left Text
		if (pLableText != 0)
		{
			snprintf(m_menuStack[m_menuStackIndex].text[itemNumber].labelStr, MENU_ROW_TEXT_MAXLEN, "%s", pLableText);
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].labelStr[0] = '\0';
		}

		// If we don't specify any Right Text
		if (pValueText != 0)
		{
			snprintf(m_menuStack[m_menuStackIndex].text[itemNumber].valueStr, MENU_ROW_TEXT_MAXLEN, "%s", pValueText);
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].valueStr[0] = '\0';
		}

		if (pSelectedCB != nullptr)
		{
			// Store the function callback
			strncpy(m_menuStack[m_menuStackIndex].text[itemNumber].button4Str, "OK", MENU_ROW_TEXT_MAXLEN);
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton4FuncSelected = pSelectedCB;
			m_menuStack[m_menuStackIndex].text[itemNumber].button4UserData = userData;
		}
		else
		{
			strncpy(m_menuStack[m_menuStackIndex].text[itemNumber].button4Str, "", MENU_ROW_TEXT_MAXLEN);
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton4FuncSelected = nullptr;
		}

		// Force re-draw of the screen
		MMIInvalidate();
	}
}

void MenuSetupButtons(uint32_t itemNumber, const char* button3Text, MENUCALLBACKFUNC_T pbutton3CB, void* button3UserData, const char* button4Text, MENUCALLBACKFUNC_T pbutton4CB, void* button4UserData)
{
	if (itemNumber < MENU_MAX_ROWS)
	{
		if (itemNumber >= m_menuStack[m_menuStackIndex].rows)
		{
			m_menuStack[m_menuStackIndex].rows = itemNumber + 1;
		}

		// Was any text specified for button 3?
		if (button3Text != nullptr)
		{
			snprintf(m_menuStack[m_menuStackIndex].text[itemNumber].button3Str, MENU_ROW_TEXT_MAXLEN, "%s", button3Text);
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].button3Str[0] = '\0';
		}

		// Was a callback specified for button 3?
		if (pbutton3CB != nullptr)
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton3FuncSelected = pbutton3CB;
			m_menuStack[m_menuStackIndex].text[itemNumber].button3UserData = button3UserData;
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton3FuncSelected = nullptr;
		}

		// Was any text specified for button 4?
		if (button4Text != nullptr)
		{
			snprintf(m_menuStack[m_menuStackIndex].text[itemNumber].button4Str, MENU_ROW_TEXT_MAXLEN, "%s", button4Text);
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].button4Str[0] = '\0';
		}

		// Was a callback specified for button 4?
		if (pbutton4CB != nullptr)
		{
			// Store the function callback
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton4FuncSelected = pbutton4CB;
			m_menuStack[m_menuStackIndex].text[itemNumber].button4UserData = button4UserData;
		}
		else
		{
			m_menuStack[m_menuStackIndex].text[itemNumber].pButton4FuncSelected = nullptr;
		}

		// Force re-draw of the screen
		MMIInvalidate();
	}
}

void MenuQuit(void)
{
	// Reset the stack for next time
	m_menuStackIndex = -1;

	// Go to the screen from which we came from
	MMIScreenGotoPrev();
}

void MenuBack(void* userData)
{
	// Decrement stack
	--m_menuStackIndex;
	if (m_menuStackIndex >= 0)
	{
		// Still a menu to display.
		_MenuSetup(&m_menuStack[m_menuStackIndex]);

		// Force full redraw
		MMIInvalidate();
	}
	else
	{ // We exit
		MenuQuit();
	}
}

static void _MenuSetup(MENU_T* pMenu)
{
	// Call the create function for the menu
	if (pMenu->pCreateFunc != 0)
	{
		pMenu->pCreateFunc(pMenu->createFuncUserData);
	}

	// Calculate the layout of the menu
	_MenuCalcLayout(pMenu);

	// Force redraw of the menu
	MMIInvalidate();
}

static void _MenuDrawItems(MENU_T* pMenu)
{
	// Index used for rendering MENU_DISPLAY_ROWS menu rows
	uint32_t row;
	uint32_t drawRowIndex = m_menuStack[m_menuStackIndex].drawRowIndex;
	uint32_t rowIndexSelection = m_menuStack[m_menuStackIndex].rowIndexSelection;
	uint32_t selectionBoxIndex = m_menuStack[m_menuStackIndex].selectionBoxIndex;
	uint32_t maxNumberOfRows;

	for (row = 0; row < pMenu->rows; row++)
	{
		// Setup the button bar
		ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
		ButtonBarSetHeight(24);
		ButtonBarSetKeyImages(KEYINDEX_1, &blank64wide, &blank64wide_s);
		ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, 20, BLACK, cucSystemWords[wMENU_Up][SettingsGetLanguageIndex()], "");
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _MenuUp, nullptr);

		ButtonBarSetKeyImages(KEYINDEX_2, &blank64wide, &blank64wide_s);
		ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, 20, BLACK, cucSystemWords[wMENU_Down][SettingsGetLanguageIndex()], "");
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _MenuDown, nullptr);

		ButtonBarSetKeyImages(KEYINDEX_3, &blank64wide, (m_menuStack[m_menuStackIndex].text[rowIndexSelection].pButton3FuncSelected == nullptr) ? (void*)&blank64wide : (void*)&blank64wide_s);
		ButtonBarSetKeyText(KEYINDEX_3, FONT_INDEX_TTMAIN, 20, BLACK, m_menuStack[m_menuStackIndex].text[rowIndexSelection].button3Str, "");
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, m_menuStack[m_menuStackIndex].text[rowIndexSelection].pButton3FuncSelected, m_menuStack[m_menuStackIndex].text[rowIndexSelection].button3UserData);

		ButtonBarSetKeyImages(KEYINDEX_4, &blank64wide, (m_menuStack[m_menuStackIndex].text[rowIndexSelection].pButton4FuncSelected == nullptr) ? (void*)&blank64wide : (void*)&blank64wide_s);
		ButtonBarSetKeyText(KEYINDEX_4, FONT_INDEX_TTMAIN, 20, BLACK, m_menuStack[m_menuStackIndex].text[rowIndexSelection].button4Str, "");
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, m_menuStack[m_menuStackIndex].text[rowIndexSelection].pButton4FuncSelected, m_menuStack[m_menuStackIndex].text[rowIndexSelection].button4UserData);

		ButtonBarSetKeyImages(KEYINDEX_5, &blank64wide, &blank64wide_s);
		ButtonBarSetKeyText(KEYINDEX_5, FONT_INDEX_TTMAIN, 20, BLACK, cucSystemWords[wMENU_Back][SettingsGetLanguageIndex()], "");
		ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, MenuBack, nullptr);
	}
	// Render only the maximum amount of visible elements.
	if (m_menuStack[m_menuStackIndex].rows < MENU_DISPLAY_ROWS)
	{
		maxNumberOfRows = m_menuStack[m_menuStackIndex].rows;
	}
	else
	{
		maxNumberOfRows = MENU_DISPLAY_ROWS;
	}
	// Do the actual rendering
	for (row = 0; row < maxNumberOfRows; row++)
	{
		// Select the MENU_DISPLAY_ROWS text to display
		uint32_t rowIndexRender = drawRowIndex + row;

		// Used in font drawing. X,Y position
		uint32_t fontStartX = 0;
		uint32_t fontStartY = 0;

		// Used in drawing the blocks
		uint32_t blockXStart = 0;
		uint32_t blockYStart = 0;
		uint32_t blockXEnd = 0;
		uint32_t blockYEnd = 0;

		// Display a White line above the text
		blockXStart = 0;
		blockYStart = ((m_startY - 9) + (row - 1)*LINE_HEIGHT) - 2;
		blockXEnd = lcd_get_width() - 1;
		blockYEnd = ((m_startY - 9) + (row - 1)*LINE_HEIGHT);
		//
		blockfillEx(blockXStart, blockYStart, blockXEnd, blockYEnd, WHITE_TEXT, MENU_ITEM_OPACITY, LAYER_BACK);

		// Display a White line below the text	
		blockXStart = 0;
		blockYStart = ((m_startY - 9) + (row - 1)*LINE_HEIGHT) + LINE_HEIGHT - 1;
		blockXEnd = lcd_get_width() - 1;
		blockYEnd = ((m_startY - 9) + (row - 1)*LINE_HEIGHT) + LINE_HEIGHT - 2;
		//
		blockfillEx(blockXStart, blockYStart, blockXEnd, blockYEnd, WHITE_TEXT, MENU_ITEM_OPACITY, LAYER_BACK);

		//Draw the actual line block
		blockXStart = 0;
		blockYStart = ((m_startY - 9) + (row - 1)*LINE_HEIGHT);
		blockXEnd = lcd_get_width() - 1;
		blockYEnd = ((m_startY - 9) + (row - 1)*LINE_HEIGHT) + LINE_HEIGHT - 3;
		//
		blockfillEx(blockXStart, blockYStart, blockXEnd, blockYEnd, MENU_ITEM_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);

		// Set the Font & Color (Left Alignment)
		fontStartX = MENU_TEXT_LEFT_SPACE;
		fontStartY = (m_startY + (row - 1)*LINE_HEIGHT) - 5;
		//
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, MENU_TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
		SimpleTextDraw(fontStartX, fontStartY, m_menuStack[m_menuStackIndex].text[rowIndexRender].labelStr, MENU_ITEM_LEFT_TEXT_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);

		// Set the Font & Color (Right Alignment)
		fontStartX = lcd_get_width() - MENU_TEXT_RIGHT_SPACE;
		fontStartY = (m_startY + (row - 1)*LINE_HEIGHT) - 5;
		//
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, MENU_TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
		SimpleTextDraw(fontStartX, fontStartY, m_menuStack[m_menuStackIndex].text[rowIndexRender].valueStr, MENU_ITEM_RIGHT_TEXT_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);

		//Set the selection block
		blockXStart = 0;
		blockYStart = ((m_startY - 9) + (selectionBoxIndex - 1)*LINE_HEIGHT);
		blockXEnd = lcd_get_width() - 1;
		blockYEnd = ((m_startY - 12) + (selectionBoxIndex - 1)*LINE_HEIGHT) + LINE_HEIGHT;
		//
		blockfillEx(blockXStart, blockYStart, blockXEnd, blockYEnd, MENU_ITEM_SELECTED_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);

		// Set the Font & Color (Left Alignment)
		fontStartX = MENU_TEXT_LEFT_SPACE;
		fontStartY = (m_startY + (selectionBoxIndex - 1)*LINE_HEIGHT) - 5;
		//
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, MENU_TEXT_SIZE, HORIZONTAL_ALIGNMENT_LEFT, VERTICAL_ALIGNMENT_TOP, 1);
		SimpleTextDraw(fontStartX, fontStartY, m_menuStack[m_menuStackIndex].text[rowIndexSelection].labelStr, MENU_ITEM_SELECTED_TEXT_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);

		// Set the Font & Color (Right Alignment)
		fontStartX = lcd_get_width() - MENU_TEXT_RIGHT_SPACE;
		fontStartY = (m_startY + (m_menuStack[m_menuStackIndex].selectionBoxIndex - 1)*LINE_HEIGHT) - 5;
		//
		SimpleTextSetupFontEx(FONT_INDEX_TTMAIN, MENU_TEXT_SIZE, HORIZONTAL_ALIGNMENT_RIGHT, VERTICAL_ALIGNMENT_TOP, 1);
		SimpleTextDraw(fontStartX, fontStartY, m_menuStack[m_menuStackIndex].text[rowIndexSelection].valueStr, MENU_ITEM_SELECTED_TEXT_COLOUR, MENU_ITEM_OPACITY, LAYER_BACK);
	}
}

static void _MenuCalcLayout(MENU_T* pEntry)
{
	// Stop divide by zero!
	if (pEntry->rows > 0)
	{
		// If we have fewer rows than the maximum number, we center them
		if (pEntry->rows < MENU_DISPLAY_ROWS)
		{
			m_startY = ((lcd_get_height() / 2 + MENU_TITLE_HEIGHT) - (pEntry->rows)*(LINE_HEIGHT / 2));
		}
		else
		{
			// We have the maximum number of rows
			// Y starting position of first icon
			m_startY = MENU_TITLE_HEIGHT + MENU_Y_BORDER;
		}
	}
	else
	{
		m_startY = 0;
	}
}

static void _MenuUp(void* userData)
{
	if ((m_menuStack[m_menuStackIndex].rowIndexSelection <= m_menuStack[m_menuStackIndex].rows) && (m_menuStack[m_menuStackIndex].rowIndexSelection > 0))
	{
		// Select the previous row
		m_menuStack[m_menuStackIndex].rowIndexSelection--;

		// if the current selected row is on the first position
		if (m_menuStack[m_menuStackIndex].selectionBoxIndex == 0)
		{
			// Decrement the row from where the rendering starts
			m_menuStack[m_menuStackIndex].drawRowIndex--;
		}
		else
		{
			// Decrement the selection row
			m_menuStack[m_menuStackIndex].selectionBoxIndex--;
		}
	}
	else
	{
		// Roll round to last item

		//The selection should be the last item
		m_menuStack[m_menuStackIndex].rowIndexSelection = m_menuStack[m_menuStackIndex].rows - 1;

		// If we have more items then we can display 
		if (m_menuStack[m_menuStackIndex].rowIndexSelection >= MENU_DISPLAY_ROWS)
		{
			// We need to update the row from where we start drawing the elements
			m_menuStack[m_menuStackIndex].drawRowIndex = m_menuStack[m_menuStackIndex].rows - MENU_DISPLAY_ROWS;
			// The selection box needs to be set on the last item
			m_menuStack[m_menuStackIndex].selectionBoxIndex = MENU_DISPLAY_ROWS - 1;
		}
		else
		{
			// All elements fit inside the screen so draw the selection box on the selected item
			m_menuStack[m_menuStackIndex].selectionBoxIndex = m_menuStack[m_menuStackIndex].rowIndexSelection;
		}
	}

	// Redraw
	MMIInvalidate();
}

static void _MenuDown(void* userData)
{
	// If the selected row is smaller than the total number of rows and higher than 0 (first row)
	if ((m_menuStack[m_menuStackIndex].rowIndexSelection < m_menuStack[m_menuStackIndex].rows - 1) && (m_menuStack[m_menuStackIndex].rowIndexSelection >= 0))
	{
		// Select the next Text
		m_menuStack[m_menuStackIndex].rowIndexSelection++;

		// if the current selected row is on the MENU_DISPLAY_ROWS-1 position
		if (m_menuStack[m_menuStackIndex].selectionBoxIndex == MENU_DISPLAY_ROWS - 1)
		{
			//Increment the row from where the rendering starts
			m_menuStack[m_menuStackIndex].drawRowIndex++;
		}
		else
		{
			// Increment the selection row
			m_menuStack[m_menuStackIndex].selectionBoxIndex++;
		}
	}
	else
	{
		// Roll round to first item
		m_menuStack[m_menuStackIndex].rowIndexSelection = 0;
		m_menuStack[m_menuStackIndex].drawRowIndex = 0;
		m_menuStack[m_menuStackIndex].selectionBoxIndex = 0;
	}

	// Redraw
	MMIInvalidate();
}
