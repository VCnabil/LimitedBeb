#include "keyboard.h"

#include <vector>
#include <string>

// creation functions
static void _KeyboardCreate(void* userData);
static void _CreateKeys(void);

// update functions
static void _KeyboardUpdate(void* userData);
static void _DrawInputText(void);
static void _DrawSelectedChar(void);

// key press functions
// button 1
static void _DoShift(void*); // go to next page. If current page is a-z then go to A-Z for a single char
static void _DoPermShift(void*); // go to next page
// button 2
static void _GoPrevChar(void*); // go to the previous char in the list
// button 3
static void _SelectChar(void*); // add the current char to the input string
static void _DoBackspace(void*); // remove the last char from the input string
// button 4
static void _GoNextChar(void*); // go to the next char in the list
static void _AddSpaceChar(void*); // Add a space character to the input string
// button 5
static void _ConfirmInput(void*); // Confirm the current input string and exit

class KeyboardData
{
public:
	KeyboardData(
		const std::vector<std::string>& pageNames,
		const std::vector<std::string>& pageChars);

	// get the name of the age at the index
	const std::string& getPageName(const uint32_t page) const;

	// get the character for the page at the index
	char getChar(const uint32_t page, const uint32_t index) const;
	char getNextChar(const uint32_t page, const uint32_t index) const;
	char getPrevChar(const uint32_t page, const uint32_t index) const;

	// get the number of pages in this data set
	uint32_t getPageCount() const;
	// get the number of character in the page of this data set
	uint32_t getPageCharCount(const uint32_t page) const;

private:
	std::vector<std::string> _PageNames;

	// _Data must be a set of strings with even numbers of characters
	// each character pair represents 
	std::vector<std::string> _PageChars;

	// check that the page is in range
	bool isPageInRange(const uint32_t page) const;

	// are the given page and index indicies in range
	bool areIndicesInRange(const uint32_t page, const uint32_t index) const;
};

KeyboardData::KeyboardData(
	const std::vector<std::string>& pageNames,
	const std::vector<std::string>& pageChars)
{
	// validate the data
	_PageNames = pageNames;
	_PageChars = pageChars;
}

const std::string& KeyboardData::getPageName(const uint32_t page) const
{
	return _PageNames[page]; // ...
}

char KeyboardData::getChar(const uint32_t page, const uint32_t index) const
{
	char output = 0;
	if (areIndicesInRange(page, index))
	{
		output = _PageChars[page][index];
	}
	return output;
}

char KeyboardData::getNextChar(const uint32_t page, const uint32_t index) const
{
	char output = 0;
	if (areIndicesInRange(page, index))
	{
		const uint32_t nextIndex = (index + 1U) % getPageCharCount(page);
		output = _PageChars[page][nextIndex];
	}
	return output;
}

char KeyboardData::getPrevChar(const uint32_t page, const uint32_t index) const
{
	char output = 0;
	if (areIndicesInRange(page, index))
	{
		uint32_t prevIndex = index;
		if (prevIndex == 0U)
		{
			prevIndex = getPageCharCount(page) - 1U;
		}
		else
		{
			--prevIndex;
		}
		output = _PageChars[page][prevIndex];
	}
	return output;
}

uint32_t KeyboardData::getPageCount() const
{
	return _PageChars.size();
}

uint32_t KeyboardData::getPageCharCount(const uint32_t page) const
{
	uint32_t output = 0U;
	if (isPageInRange(page))
	{
		output = _PageChars[page].size();
	}
	return output;
}

bool KeyboardData::isPageInRange(const uint32_t page) const
{
	return page < getPageCount();
}

bool KeyboardData::areIndicesInRange(const uint32_t page, const uint32_t index) const
{
	bool output = false;
	if (isPageInRange(page))
	{
		output = (index < _PageChars[page].size());
	}
	return output;
}

// add one for each keboard layout
static const KeyboardData _KeyboardData = {
	{ "a-z", "A-Z", "0-9", "!%#~" },
	{ 
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"0123456789",
		".,?!;:-_=+\"'%&*@#~()[]<>{}$" // this could do with some rearrangement
	}
};

// page indicies
static const uint32_t PageIndex_LowerAlpha = 0U;
static const uint32_t PageIndex_UpperAlpha = 1U;
static const uint32_t PageIndex_Numeric = 2U;
static const uint32_t PageIndex_Symbols = 3U;
static const uint32_t PageIndex_Max = 4U;

// the current page of character that we are on
static uint32_t _PageIndex = PageIndex_LowerAlpha;
// are we staying on this page for a single char - switches back to lower alpha from upper alpha after 1 char
static bool _SingleCharMode = false;

// the index of the selected char for the current page
static uint32_t _CharIndex = 0U;

// the current string that has been input
static std::string _InputString = "";

// elementID for drawing the currently input text
static uint32_t _TextElementID = ELEMENTID_INVALID;
static uint32_t _CharElementID = ELEMENTID_INVALID;

static bool _SuppressShiftRelease = false;
static bool _SuppressSelectRelease = false;
static bool _SuppressNextRelease = false;

// confirm callback
// the user data output by the confirm callback will
// be a pointer to the _InputString cast to void*
static void(*_ConfirmCallback)(void*) = nullptr;

// The minimum and maximum length of the input string
static uint32_t _MinLength = 3U;
static uint32_t _MaxLength = 16U;

void KeyboardInit(void)
{

}

void KeyboardEnter(void)
{
	// Register us with the MMI as a POST-CREATE and POST-UPDATE callback
	MMIRegisterCallback(MMICALLBACKTYPE_POSTCREATE, _KeyboardCreate, nullptr);
	MMIRegisterCallback(MMICALLBACKTYPE_POSTUPDATE, _KeyboardUpdate, nullptr);

	// Force a full screen redraw. This will force our _KeyboardCreate() and _KeyboardUpdate() to be called
	MMIInvalidate();
}

void KeyboardSetInputString(const std::string& inputString)
{
	_InputString = inputString;
}

const std::string& KeyboardGetInputString(void)
{
	return _InputString;
}

void KeyboardSetConfirmCallback(void(*callback)(void*))
{
	_ConfirmCallback = callback;
}

static void _KeyboardCreate(void* userData)
{
	(void)userData;

	vLcdBlankerEx(BLACK, BLACK);

	_CreateKeys();
}

void KeyboardSetLengthLimits(const uint32_t minLength, const uint32_t maxLength)
{
	_MinLength = minLength;
	_MaxLength = maxLength;

	// trim any addtional length from the input string
	if (_InputString.size() > maxLength)
	{
		_InputString.erase(_InputString.begin() + maxLength, _InputString.end());
	}
}

static void _CreateKeys(void)
{
	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(24);
	for (uint32_t i = KEYINDEX_1; i <= KEYINDEX_5; ++i)
	{
		ButtonBarSetKeyImages(static_cast<KEYINDEX_T>(i), &blank64wide, &blank64wide_s);
	}

	const uint32_t fontSize = 12;

	// button 1
	ButtonBarSetKeyText(KEYINDEX_1, FONT_INDEX_TTMAIN, fontSize, BLACK, _KeyboardData.getPageName(_PageIndex).c_str(), nullptr); // but needs to be the title text for the "page"
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, &_DoShift, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_1, &_DoPermShift, nullptr, 1000U, 0U);
	// button 2
	ButtonBarSetKeyText(KEYINDEX_2, FONT_INDEX_TTMAIN, fontSize, BLACK, "PREV", nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, &_GoPrevChar, nullptr);
	// button 3
	ButtonBarSetKeyText(KEYINDEX_3, FONT_INDEX_TTMAIN, fontSize, BLACK, "SEL /", "BSP  ");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, &_SelectChar, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_3, &_DoBackspace, nullptr, 1000U, 0U);
	// button 4
	ButtonBarSetKeyText(KEYINDEX_4, FONT_INDEX_TTMAIN, fontSize, BLACK, "NEXT /", "SPACE");
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, &_GoNextChar, nullptr);
	ButtonBarRegisterKeyHoldCallback(KEYINDEX_4, &_AddSpaceChar, nullptr, 1000U, 0U);
	// button 5 
	ButtonBarSetKeyText(KEYINDEX_5, FONT_INDEX_TTMAIN, fontSize, BLACK, "CONFIRM", nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, &_ConfirmInput, nullptr);
}

static void _KeyboardUpdate(void* userData)
{
	(void)userData;

	_DrawInputText(); // refresh the input text
	_DrawSelectedChar(); // refresh the selected char
}

static void _DrawInputText(void)
{
	if (_TextElementID == ELEMENTID_INVALID)
	{
		// create element
		_TextElementID = ElementGetNewId();
	}

	const int32_t textX = lcd_get_width() / 2;
	const int32_t textY = lcd_get_height() / 3;

	ElementFillPrevRect(_TextElementID, BLACK, LAYER_FRONT);

	SimpleTextSetupFont(FONT_INDEX_TTMAIN, 40);
	SimpleTextSetupFontAlignment(HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_CENTRE);

	SimpleTextDrawEle(_TextElementID, textX, textY, _InputString.c_str(), WHITE_TEXT, 100, LAYER_FRONT);
}

static void _DrawSelectedChar(void)
{
	if (_CharElementID == ELEMENTID_INVALID)
	{
		// create element
		_CharElementID = ElementGetNewId();
	}

	const int32_t textX = lcd_get_width() / 2;
	const int32_t textY = (2 * lcd_get_height()) / 3;

	ElementFillPrevRect(_CharElementID, BLACK, LAYER_FRONT);

	SimpleTextSetupFont(FONT_INDEX_TTMAIN, 40);
	SimpleTextSetupFontAlignment(HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_CENTRE);

	const char text[] = { _KeyboardData.getChar(_PageIndex, _CharIndex), '\0' };
	SimpleTextDrawEle(_CharElementID, textX, textY, text, WHITE_TEXT, 100, LAYER_FRONT);

	// also draw the next char in either direction
	const char next[] = { _KeyboardData.getNextChar(_PageIndex, _CharIndex), '\0' };
	const char prev[] = { _KeyboardData.getPrevChar(_PageIndex, _CharIndex), '\0' };

	SimpleTextSetupFont(FONT_INDEX_TTMAIN, 20);
	SimpleTextSetupFontAlignment(HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_CENTRE);

	SimpleTextDrawEle(_CharElementID, textX + 100, textY, next, WHITE_TEXT, 100, LAYER_FRONT);
	SimpleTextDrawEle(_CharElementID, textX - 100, textY, prev, WHITE_TEXT, 100, LAYER_FRONT);
}

static void _DoShift(void*)
{
	if (!_SuppressShiftRelease)
	{
		if (_PageIndex == PageIndex_LowerAlpha)
		{
			// shift to the next upper alpha for 1 char
			_PageIndex = PageIndex_UpperAlpha;
			_SingleCharMode = true;
			// we don't need to change the index as a-z and A-Z should have the same number of chars
		}
		else
		{
			// go to the next page
			_PageIndex = (_PageIndex + 1U) % PageIndex_Max;

			// reset the character index
			_CharIndex = 0U;

			// if on upper alpha and on single char then we still go to the next page
			_SingleCharMode = false;
		}

		MMIInvalidate();
	}
	_SuppressShiftRelease = false;
}

static void _DoPermShift(void*)
{
	if (_PageIndex == PageIndex_LowerAlpha)
	{
		// go to upper alpha but don't enter single char mode
		_PageIndex = PageIndex_UpperAlpha;
		_SingleCharMode = false;
		MMIInvalidate();
	}
	_SuppressShiftRelease = true;
}

static void _GoPrevChar(void*)
{
	// decrease the current index
	if (_CharIndex == 0U)
	{
		_CharIndex = _KeyboardData.getPageCharCount(_PageIndex) - 1U;
	}
	else
	{
		--_CharIndex;
	}
}

static void _SelectChar(void*)
{
	if (!_SuppressSelectRelease)
	{
		if(_InputString.size() < _MaxLength)
		{
			_InputString.push_back(_KeyboardData.getChar(_PageIndex, _CharIndex));
			if (_PageIndex == PageIndex_UpperAlpha && _SingleCharMode)
			{
				_PageIndex = PageIndex_LowerAlpha;
			}
		}
	}
	_SuppressSelectRelease = false;
}

static void _DoBackspace(void*)
{
	if (!_InputString.empty())
	{
		_InputString.pop_back();
	}
	_SuppressSelectRelease = true;
}

static void _GoNextChar(void*)
{
	if (!_SuppressNextRelease)
	{
		// increase the current index
		_CharIndex = (_CharIndex + 1U) % _KeyboardData.getPageCharCount(_PageIndex);
	}
	_SuppressNextRelease = false;
}

static void _AddSpaceChar(void*)
{
	// insert a space
	if(_InputString.size() < _MaxLength)
	{
		_InputString.push_back(' ');
	}
	_SuppressNextRelease = true;
}

static void _ConfirmInput(void*)
{
	if (_ConfirmCallback)
	{
		_ConfirmCallback((void*)&_InputString);
		_ConfirmCallback = nullptr; // unset the callback before we exit
	}

	// we are done
	MMIScreenGotoPrev();

	MMIDeregisterCallback(MMICALLBACKTYPE_POSTCREATE, _KeyboardCreate, nullptr);
	MMIDeregisterCallback(MMICALLBACKTYPE_POSTUPDATE, _KeyboardUpdate, nullptr);
}

