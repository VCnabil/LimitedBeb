//------------------------------------------------------------------------------
//  TITLE :          buttonbar.c
//------------------------------------------------------------------------------

#include "project.h"

//------------------------------------------------------------------------------
// TYPES AND CONSTANTS
//------------------------------------------------------------------------------

// Maximum length of button text
#define BUTTONBAR_TEXT_MAXLEN			255

// The state of the keys
typedef enum
{
	KEYSTATE_UP = 0,
	KEYSTATE_DOWN,
	KEYSTATE_HOLD
} KEYSTATE_T;

// Keypad keys
typedef struct
{
	// The current state of the key
	KEYSTATE_T state;

	// The time in milliseconds when the state last changed
	uint32_t stateChangeTimeMs;

	// The time in milliseconds for a key press to trigger a hold
	uint32_t keyHoldTimeMs;

	// The time in milliseconds for a key hold to trigger a repeat
	uint32_t keyRepeatTimeMs;

	// Key release callback function
	BUTTONBAR_CALLBACK_T pKeyReleaseFunc;
	void* keyReleaseFuncUserData;

	// Key press callback function
	BUTTONBAR_CALLBACK_T pKeyPressFunc;
	void* keyPressFuncUserData;

	// Key hold/repeat callback function
	BUTTONBAR_CALLBACK_T pKeyHoldFunc;
	void* keyHoldFuncUserData;
} KEYPADKEYS_T;

// Key masks
typedef struct
{
	// The key mask. A combination of KEYMASK_N bits
	uint8_t mask;

	// The hold time for the mask
	uint32_t holdTimeMs;

	// The function to call when the key mask matches
	BUTTONBAR_CALLBACK_T pKeyMaskFunc;
	void* keyMaskFuncUserData;

	// Has the hold callback been triggered?
	BOOL hasHoldTriggered;
} KEYPADMASK_T;

// The state of the button bar
typedef enum
{
	BUTTONBARSTATE_DISABLED = 0,
	BUTTONBARSTATE_ACTIVE,
	BUTTONBARSTATE_HIDDEN,
	BUTTONBARSTATE_SHOWING,
	BUTTONBARSTATE_HIDING
} BUTTONBARSTATE_T;

// Button bar information
typedef struct
{
	// The state of the button bar
	BUTTONBARSTATE_T state;

	// The mode of the button bar
	BUTTONBARMODE_T mode;

	// Does the button bar need redrawing?
	BOOL isInvalidated;

	BUTTONBARANIMATION_T animation;

	// The height of the button bar
	uint32_t height;

	// The background colour of the button bar
	uint16_t backgroundColour;

	// the background bitmap for the button bar
	BITMAPA* pBackgroundBitmap;

	// Key information
	struct
	{
		// Up image for the key
		BITMAPA* pKeyUpImage;

		// Down image for the key
		BITMAPA* pKeyDownImage;

		// The font to use
		FONT_INDEX_T fontIndex;

		// The size of the font
		uint32_t fontSize;

		// The colour of the text
		uint16_t fontColour;

		// Text line 1
		char textLine1[BUTTONBAR_TEXT_MAXLEN];

		// Text line 2
		char textLine2[BUTTONBAR_TEXT_MAXLEN];
	} keys[KEYPAD_MAX_KEYS];
} BUTTONBAR_T;

//------------------------------------------------------------------------------
// PRIVATE PROTOTYPES
//------------------------------------------------------------------------------

static void _ButtonBarProcess(void* pUserData = nullptr);
static void _ButtonBarCheckKeys(uint8_t keypadCode, uint32_t currTimeMs);
static void _ButtonBarDoKeyCallback(BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData);
static void _ButtonBarCheckKeyMasks(uint8_t keypadCode, uint32_t currTimeMs);
static void _ButtonBarDraw(uint8_t keypadCode);
static void _ButtonBarDrawBackground(void);
static void _ButtonBarDrawKey(KEYINDEX_T keyIndex, KEYSTATE_T keyState);
static void _ButtonBarAnimate(uint32_t currTimeMs);
static void _ButtonBarKeyCallbackDisable(void* pUserData = nullptr);

//------------------------------------------------------------------------------
// MODULE GLOBALS
//-----------------------------------------------------------------------------

// Are we inhibited?
static BOOL m_isKeyCallbacksDisabled = FALSE;

// The last keypad code
static uint8_t m_prevKeypadCode = 0;
// the last time the keypad code changed
static uint32_t m_prevKeypadCodeTimeMs = 0;

// Settings for each of the keys
static KEYPADKEYS_T m_keypadKeys[KEYPAD_MAX_KEYS];

// Settings for the keypad masks
static KEYPADMASK_T m_keypadMasks[BUTTONBAR_MAX_KEYMASK_HOOKS];

// Button bar graphic information
static BUTTONBAR_T m_buttonBar;

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//-----------------------------------------------------------------------------

// Initialise the button bar module
void ButtonBarInit(void)
{
	// Not inhibited
	m_isKeyCallbacksDisabled = FALSE;

	// Clear keycode and time
	m_prevKeypadCode = 0;
	m_prevKeypadCodeTimeMs = get_uptime_milliseconds();

	// Clear the rest of our state
	ButtonBarReset();

	// Register our Keypad Scan function with the MMI so it gets called at the start of the MMIProcess() function
	MMIRegisterCallback(MMICALLBACKTYPE_PREPROCESS, _ButtonBarProcess, nullptr);

	// Register our Reset function with the MMI so button bar is reset to defaults before a screens create is called
	MMIRegisterCallback(MMICALLBACKTYPE_PRECREATE, ButtonBarReset, nullptr);

	// Register our callback that disables key callbacks with the MMI 
	MMIRegisterCallback(MMICALLBACKTYPE_ENTER, _ButtonBarKeyCallbackDisable, nullptr);
}

// Reset the button bar to its default state.
// This can be called at any time and is automatically called BEFORE a screens create function is called (via an MMI Pre Create Hook)
void ButtonBarReset(void* pUserData)
{
	(void)pUserData;

	uint32_t i = 0;

	// Clear they mappings and settings for the keys
	for (i = 0; i < KEYPAD_MAX_KEYS; i++)
	{
		// Clear all hooks for this key
		m_keypadKeys[i].keyHoldTimeMs = 0;
		m_keypadKeys[i].keyRepeatTimeMs = 0;
		m_keypadKeys[i].pKeyReleaseFunc = 0;
		m_keypadKeys[i].pKeyPressFunc = 0;
		m_keypadKeys[i].pKeyHoldFunc = 0;
	}

	// Clear the key mask mappings
	for (i = 0; i < BUTTONBAR_MAX_KEYMASK_HOOKS; i++)
	{
		// Clear mask
		m_keypadMasks[i].mask = 0;
		m_keypadMasks[i].holdTimeMs = 0;
		m_keypadMasks[i].pKeyMaskFunc = 0;
	}

	// Reset defaults for the button bar settings
	m_buttonBar.state = BUTTONBARSTATE_DISABLED;
	m_buttonBar.mode = BUTTONBARMODE_DISABLED;
	m_buttonBar.isInvalidated = TRUE;
	m_buttonBar.animation = BUTTONBARANIMATION_NONE;
	m_buttonBar.height = 0;
	m_buttonBar.backgroundColour = ALPHA_COLOR;
	m_buttonBar.pBackgroundBitmap = 0;
	for (i = 0; i < KEYPAD_MAX_KEYS; i++)
	{
		// Clear images
		m_buttonBar.keys[i].pKeyUpImage = 0;
		m_buttonBar.keys[i].pKeyDownImage = 0;

		// Clear font and text
		m_buttonBar.keys[i].fontIndex = FONT_INDEX_TTMAIN;
		m_buttonBar.keys[i].fontSize = 10;
		m_buttonBar.keys[i].fontColour = WHITE_TEXT;
		m_buttonBar.keys[i].textLine1[0] = '\0';
		m_buttonBar.keys[i].textLine2[0] = '\0';
	}
}

// Set the mode of the button bar
void ButtonBarSetMode(BUTTONBARMODE_T mode)
{
	m_buttonBar.mode = mode;

	if (m_buttonBar.mode == BUTTONBARMODE_DISABLED)
	{
		m_buttonBar.state = BUTTONBARSTATE_DISABLED;
	}
	else
	{
		m_buttonBar.state = BUTTONBARSTATE_ACTIVE;
	}

	m_buttonBar.isInvalidated = TRUE;
}

// Set the height of the button bar
void ButtonBarSetHeight(uint32_t height)
{
	m_buttonBar.height = height;

	m_buttonBar.isInvalidated = TRUE;
}

// Set the animation of the button bar when hiding/displaying a menu
void ButtonBarSetAnimation(BUTTONBARANIMATION_T animation)
{
	m_buttonBar.animation = animation;
}

// Set the background colour of the button bar. This can be ALPHA_COLOUR for a transparent background
void ButtonBarSetBackgroundColour(uint16_t colour)
{
	m_buttonBar.backgroundColour = colour;

	m_buttonBar.isInvalidated = TRUE;
}

// Set the background image for the button bar.
void ButtonBarSetBackgroundImage(const void* pBitmap)
{
	m_buttonBar.pBackgroundBitmap = (BITMAPA*)pBitmap;

	m_buttonBar.isInvalidated = TRUE;
}

// Set the image for a key
void ButtonBarSetKeyImages(KEYINDEX_T keyIndex,
	const void* pUpBitmap,
	const void* pDownBitmap)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_buttonBar.keys[keyIndex].pKeyUpImage = (BITMAPA*)pUpBitmap;
		m_buttonBar.keys[keyIndex].pKeyDownImage = (BITMAPA*)pDownBitmap;

		m_buttonBar.isInvalidated = TRUE;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// Set the line1 and line 2 text for the key
void ButtonBarSetKeyText(KEYINDEX_T keyIndex,
	FONT_INDEX_T fontIndex,
	uint32_t fontSize,
	uint16_t fontColour,
	const char* pTextLine1,
	const char* pTextLine2)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_buttonBar.keys[keyIndex].fontIndex = fontIndex;
		m_buttonBar.keys[keyIndex].fontSize = fontSize;
		m_buttonBar.keys[keyIndex].fontColour = fontColour;
		if (pTextLine1 != 0)
		{
			strncpy(m_buttonBar.keys[keyIndex].textLine1, pTextLine1, BUTTONBAR_TEXT_MAXLEN);
		}
		if (pTextLine2 != 0)
		{
			strncpy(m_buttonBar.keys[keyIndex].textLine2, pTextLine2, BUTTONBAR_TEXT_MAXLEN);
		}

		m_buttonBar.isInvalidated = TRUE;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// Test if a key is pressed or not
BOOL ButtonBarIsKeyPressed(KEYINDEX_T keyIndex)
{
	BOOL isPressed = FALSE;

	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		isPressed = (m_keypadKeys[keyIndex].state != KEYSTATE_UP);
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}

	return isPressed;
}

// Register for key release events
void ButtonBarRegisterKeyReleaseCallback(KEYINDEX_T keyIndex,
	BUTTONBAR_CALLBACK_T pCallbackFunc,
	void* userData)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyReleaseFunc = pCallbackFunc;
		m_keypadKeys[keyIndex].keyReleaseFuncUserData = userData;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// De-register for key release events
void ButtonBarDeregisterKeyReleaseCallback(KEYINDEX_T keyIndex)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyReleaseFunc = 0;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// Register for key press events
void ButtonBarRegisterKeyPressCallback(KEYINDEX_T keyIndex,
	BUTTONBAR_CALLBACK_T pCallbackFunc,
	void* userData)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyPressFunc = pCallbackFunc;
		m_keypadKeys[keyIndex].keyPressFuncUserData = userData;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// De-register for key press events
void ButtonBarDeregisterKeyPressCallback(KEYINDEX_T keyIndex)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyPressFunc = 0;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// Register for key hold/repeat events
void ButtonBarRegisterKeyHoldCallback(KEYINDEX_T keyIndex,
	BUTTONBAR_CALLBACK_T pCallbackFunc,
	void* userData,
	uint32_t keyHoldTimeMs,
	uint32_t keyRepeatTimeMs)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyHoldFunc = pCallbackFunc;
		m_keypadKeys[keyIndex].keyHoldFuncUserData = userData;
		m_keypadKeys[keyIndex].keyHoldTimeMs = keyHoldTimeMs;
		m_keypadKeys[keyIndex].keyRepeatTimeMs = keyRepeatTimeMs;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// De-register for key hold/repeat events
void ButtonBarDeregisterKeyHoldCallback(KEYINDEX_T keyIndex)
{
	if (keyIndex < KEYPAD_MAX_KEYS)
	{
		m_keypadKeys[keyIndex].pKeyHoldFunc = 0;
		m_keypadKeys[keyIndex].keyHoldTimeMs = 0;
		m_keypadKeys[keyIndex].keyRepeatTimeMs = 0;
	}
	else
	{
		assert("keyIndex is not valid!" && FALSE);
	}
}

// Register for key-mask (multiple key) events
BOOL ButtonBarRegisterKeyMaskCallback(uint8_t keyMask,
	uint32_t holdTimeMs,
	BUTTONBAR_CALLBACK_T pCallbackFunc,
	void* userData)
{
	BOOL bRet = FALSE;
	uint32_t i;

	for (i = 0; i < BUTTONBAR_MAX_KEYMASK_HOOKS; i++)
	{
		if (m_keypadMasks[i].mask == 0)
		{
			m_keypadMasks[i].mask = keyMask;
			m_keypadMasks[i].holdTimeMs = holdTimeMs;
			m_keypadMasks[i].pKeyMaskFunc = pCallbackFunc;
			m_keypadMasks[i].keyMaskFuncUserData = userData;
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

// De-register for key-mask (multiple key) events
BOOL ButtonBarDeregisterKeyMaskCallback(BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData)
{
	BOOL bRet = FALSE;
	uint32_t i;

	for (i = 0; i < BUTTONBAR_MAX_KEYMASK_HOOKS; i++)
	{
		if (m_keypadMasks[i].pKeyMaskFunc == pCallbackFunc && m_keypadMasks[i].keyMaskFuncUserData == userData)
		{
			m_keypadMasks[i].mask = 0;
			m_keypadMasks[i].holdTimeMs = 0;
			m_keypadMasks[i].pKeyMaskFunc = 0;
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

//------------------------------------------------------------------------------
// PRIVATE FUNCTIONS
//-----------------------------------------------------------------------------

// Button Bar Processing
static void _ButtonBarProcess(void* pUserData)
{
	(void)pUserData;

	// Get the current time in milliseconds
	uint32_t currTimeMs = get_uptime_milliseconds();

	// Read the keypad
	uint8_t keypadCode = keypad_read();

	_ButtonBarCheckKeys(keypadCode, currTimeMs);

	_ButtonBarCheckKeyMasks(keypadCode, currTimeMs);

	_ButtonBarDraw(keypadCode);

	_ButtonBarAnimate(currTimeMs);

	// Are key callbacks disabled?
	if (m_isKeyCallbacksDisabled)
	{
		// They are. These get automatically re-enabled once no keys are pressed
		if (keypadCode == 0)
		{
			m_isKeyCallbacksDisabled = FALSE;
		}
	}

	if (m_prevKeypadCode != keypadCode)
	{
		// Save the current keypad code as it has changed
		m_prevKeypadCode = keypadCode;
		// and save the time of the change
		m_prevKeypadCodeTimeMs = currTimeMs;
	}
}

static void _ButtonBarCheckKeys(uint8_t keypadCode, uint32_t currTimeMs)
{
	if (m_buttonBar.state != BUTTONBARSTATE_DISABLED)
	{
		uint32_t keyIndex = 0;

		// Check for single key matches
		for (keyIndex = 0; keyIndex < KEYPAD_MAX_KEYS; keyIndex++)
		{
			// Create the mask for this key
			uint8_t keyMask = 1 << keyIndex;

			// Is the key down?
			if (keyMask & keypadCode)
			{
				// Yes, key is down. Handle change in state
				switch (m_keypadKeys[keyIndex].state)
				{
				default:
				case KEYSTATE_UP:
					// Change in state.
					m_keypadKeys[keyIndex].state = KEYSTATE_DOWN;
					// Save time of change
					m_keypadKeys[keyIndex].stateChangeTimeMs = currTimeMs;

					// Make a short beep of 100ms
					buzzer_out_timed(100);

					// Do the callback
					_ButtonBarDoKeyCallback(m_keypadKeys[keyIndex].pKeyPressFunc, m_keypadKeys[keyIndex].keyPressFuncUserData);
					break;

				case KEYSTATE_DOWN:
					// Is there a hold time specified for this key?
					if (m_keypadKeys[keyIndex].keyHoldTimeMs != 0)
					{
						// Has the hold time been reached?
						if ((currTimeMs - m_keypadKeys[keyIndex].stateChangeTimeMs) >= m_keypadKeys[keyIndex].keyHoldTimeMs)
						{
							// Change in state.
							m_keypadKeys[keyIndex].state = KEYSTATE_HOLD;
							// Save time of change
							m_keypadKeys[keyIndex].stateChangeTimeMs = currTimeMs;

							_ButtonBarDoKeyCallback(m_keypadKeys[keyIndex].pKeyHoldFunc, m_keypadKeys[keyIndex].keyHoldFuncUserData);
						}
					}
					break;

				case KEYSTATE_HOLD:
					// Is there a repeat time specified for this key?
					if (m_keypadKeys[keyIndex].keyRepeatTimeMs != 0)
					{
						// Has the repeat time been reached?
						if ((currTimeMs - m_keypadKeys[keyIndex].stateChangeTimeMs) >= m_keypadKeys[keyIndex].keyRepeatTimeMs)
						{
							// Save time of change
							m_keypadKeys[keyIndex].stateChangeTimeMs = currTimeMs;

							_ButtonBarDoKeyCallback(m_keypadKeys[keyIndex].pKeyHoldFunc, m_keypadKeys[keyIndex].keyHoldFuncUserData);
						}
					}
					break;
				}
			}
			else
			{
				// No, key is up. Handle change in state
				switch (m_keypadKeys[keyIndex].state)
				{
				default:
				case KEYSTATE_UP:
					// No change...
					break;

				case KEYSTATE_DOWN:
				case KEYSTATE_HOLD:
					// Change in state.
					m_keypadKeys[keyIndex].state = KEYSTATE_UP;
					// Save time of change
					m_keypadKeys[keyIndex].stateChangeTimeMs = currTimeMs;

					_ButtonBarDoKeyCallback(m_keypadKeys[keyIndex].pKeyReleaseFunc, m_keypadKeys[keyIndex].keyReleaseFuncUserData);
					break;
				}
			}
		}
	}
}

static void _ButtonBarDoKeyCallback(BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData)
{
	if ((pCallbackFunc != nullptr)
		&& (!m_isKeyCallbacksDisabled)
		&& (m_buttonBar.state == BUTTONBARSTATE_ACTIVE))
	{
		// Only do callback if we have a valid callback, we are not inhibited and we are in ACTIVE state
		pCallbackFunc(userData);
	}
}

static void _ButtonBarCheckKeyMasks(uint8_t keypadCode, uint32_t currTimeMs)
{
	// Key masks are always processed if we are not in DISABLED state
	if (m_buttonBar.state != BUTTONBARSTATE_DISABLED)
	{
		// Is any key pressed? 
		if (keypadCode != 0)
		{
			uint32_t i;

			// Has the keypad code changed?
			if (keypadCode != m_prevKeypadCode)
			{
				// Yes, scan all the masks
				for (i = 0; i < BUTTONBAR_MAX_KEYMASK_HOOKS; i++)
				{
					// Hold time set for this key mask?
					if (m_keypadMasks[i].holdTimeMs == 0)
					{
						// This mask does not have a hold time
						// Check to see if it is a match
						if (m_keypadMasks[i].mask == keypadCode)
						{
							// Matched. Call the function
							m_keypadMasks[i].pKeyMaskFunc(m_keypadMasks[i].keyMaskFuncUserData);
						}
					}
					else
					{
						// This mask has a hold time. 
						// Clear the triggered flag as the keypad code has changed
						m_keypadMasks[i].hasHoldTriggered = FALSE;
					}
				}
			}
			else
			{
				// Keypad code has not changed since last check
				// Scan all the masks
				for (i = 0; i < BUTTONBAR_MAX_KEYMASK_HOOKS; i++)
				{
					// Does this mask have a hold time set?
					// and has the hold already triggered?
					// and does the keypad code match?
					if ((m_keypadMasks[i].holdTimeMs > 0)
						&& (!m_keypadMasks[i].hasHoldTriggered)
						&& (m_keypadMasks[i].mask == keypadCode))
					{
						// Has the hold time been reached?
						if ((currTimeMs - m_prevKeypadCodeTimeMs) >= m_keypadMasks[i].holdTimeMs)
						{
							// Yes. Do the callback
							m_keypadMasks[i].pKeyMaskFunc(m_keypadMasks[i].keyMaskFuncUserData);
							// and set the flag to say it has triggered
							m_keypadMasks[i].hasHoldTriggered = TRUE;
						}
					}
				}
			}
		}
	}
}

static void _ButtonBarDraw(uint8_t keypadCode)
{
	// We don't draw anything if we are in DISABLED state or we are in INVISIBLE mode
	if ((m_buttonBar.state != BUTTONBARSTATE_DISABLED) && (m_buttonBar.mode != BUTTONBARMODE_INVISIBLE))
	{
		// Save invalidated flag locally
		BOOL isInvalidated = m_buttonBar.isInvalidated;
		// Reset invalidated flag
		m_buttonBar.isInvalidated = FALSE;

		// Has the key code changed since the last check?
		if (keypadCode != m_prevKeypadCode)
		{
			// Yes. Do a complete redraw then
			isInvalidated = TRUE;
		}

		// Does button bar need redrawing?
		if (isInvalidated)
		{
			uint32_t keyIndex = 0;

			// Redraw background
			_ButtonBarDrawBackground();

			// Redraw all keys
			for (keyIndex = 0; keyIndex < KEYPAD_MAX_KEYS; keyIndex++)
			{
				_ButtonBarDrawKey((KEYINDEX_T)keyIndex, (keypadCode & (1 << keyIndex)) ? KEYSTATE_DOWN : KEYSTATE_UP);
			}
		}
	}
}

static void _ButtonBarDrawBackground(void)
{
	// Fill the height of the menu with the background colour
	blockfillEx(0, lcd_get_height(), lcd_get_width() - 1, lcd_get_height() + m_buttonBar.height, m_buttonBar.backgroundColour, 100, LAYER_FRONT);

	// Is there a bitmap?
	if (m_buttonBar.pBackgroundBitmap != 0)
	{
		// Yes. Draw this as well then
		outimageEx(0, lcd_get_height(), m_buttonBar.pBackgroundBitmap, 100, ALPHA_INVARIANT, LAYER_FRONT);
	}
}

static void _ButtonBarDrawKey(KEYINDEX_T keyIndex,
	KEYSTATE_T keyState)
{
	// Calculate X position of image
	int32_t xPos = (lcd_get_width() / KEYPAD_MAX_KEYS) * keyIndex;
	// Select the correct image
	const void* pBitmap = m_buttonBar.keys[keyIndex].pKeyUpImage;
	if ((keyState == KEYSTATE_DOWN) && (m_buttonBar.keys[keyIndex].pKeyDownImage != 0))
	{
		pBitmap = m_buttonBar.keys[keyIndex].pKeyDownImage;
	}

	// Draw the image in the position for the key
	outimageEx(xPos, lcd_get_height(), pBitmap, 100, ALPHA_INVARIANT, LAYER_FRONT);

	// Is there any text?
	if (strlen(m_buttonBar.keys[keyIndex].textLine1) > 0)
	{
		// Calculate the centre X position of the button
		xPos += (lcd_get_width() / KEYPAD_MAX_KEYS) / 2;

		// Setup the font
		SimpleTextSetupFontEx(m_buttonBar.keys[keyIndex].fontIndex, m_buttonBar.keys[keyIndex].fontSize, HORIZONTAL_ALIGNMENT_CENTRE, VERTICAL_ALIGNMENT_TOP, 1);

		// Are there 2 lines of text?
		if (strlen(m_buttonBar.keys[keyIndex].textLine2) == 0)
		{
			// No, only 1 line of text
			uint32_t yPos = lcd_get_height() + (m_buttonBar.height / 2) - (m_buttonBar.keys[keyIndex].fontSize / 2);
			SimpleTextDraw(xPos, yPos, m_buttonBar.keys[keyIndex].textLine1, m_buttonBar.keys[keyIndex].fontColour, 100, LAYER_FRONT);
		}
		else
		{
			// Yes, 2 lines of text
			uint32_t yPos = lcd_get_height() + (m_buttonBar.height / 2);
			SimpleTextDraw(xPos, yPos - m_buttonBar.keys[keyIndex].fontSize, m_buttonBar.keys[keyIndex].textLine1, m_buttonBar.keys[keyIndex].fontColour, 100, LAYER_FRONT);
			SimpleTextDraw(xPos, yPos, m_buttonBar.keys[keyIndex].textLine2, m_buttonBar.keys[keyIndex].fontColour, 100, LAYER_FRONT);
		}
	}
}

static void _ButtonBarAnimate(uint32_t currTimeMs)
{
	// We don't draw anything if we are in DISABLED state or we are in INVISIBLE mode
	if ((m_buttonBar.state != BUTTONBARSTATE_DISABLED) && (m_buttonBar.mode != BUTTONBARMODE_INVISIBLE))
	{
		if (m_buttonBar.mode == BUTTONBARMODE_VISIBLE_ALWAYS)
		{
			// We are always showing
			SetButtonOverlayHeight(m_buttonBar.height);
		}
		else if (m_buttonBar.mode == BUTTONBARMODE_VISIBLE_WITH_TIMEOUT)
		{
			// Set the height of the displayed button bar (overlay)
			switch (m_buttonBar.state)
			{
			default:
			case BUTTONBARSTATE_ACTIVE:
				// We are fully visible in ACTIVE state
				SetButtonOverlayHeight(m_buttonBar.height);

				// Has a key been pressed within the timeout period?
				if ((currTimeMs - m_prevKeypadCodeTimeMs) >= BUTTONBAR_TIMEOUT_MS)
				{
					// No. Change to HIDING state
					m_buttonBar.state = BUTTONBARSTATE_HIDING;
				}
				break;


			case BUTTONBARSTATE_HIDDEN:
				// We are in HIDDEN state
				SetButtonOverlayHeight(0);

				// Has a key been pressed within the timeout period?
				if ((currTimeMs - m_prevKeypadCodeTimeMs) < BUTTONBAR_TIMEOUT_MS)
				{
					// Yes. Change to SHOWING state
					m_buttonBar.state = BUTTONBARSTATE_SHOWING;
				}
				break;

			case BUTTONBARSTATE_SHOWING:
				// We are showing. What animation are we using?
				switch (m_buttonBar.animation)
				{
				default:
				case BUTTONBARANIMATION_NONE:
					// We are now ACTIVE
					SetButtonOverlayHeight(m_buttonBar.height);
					m_buttonBar.state = BUTTONBARSTATE_ACTIVE;
					break;

				case BUTTONBARANIMATION_SLIDE:
				{
					// Increment height
					int32_t overlayHeight = GetButtonOverlayHeight();
					overlayHeight += BUTTONBAR_SLIDE_ANIMATION_INCREMENT;
					if (overlayHeight < (int32_t)m_buttonBar.height)
					{
						SetButtonOverlayHeight(overlayHeight);
					}
					else
					{
						SetButtonOverlayHeight(m_buttonBar.height);
						// Done showing. We are now ACTIVE
						m_buttonBar.state = BUTTONBARSTATE_ACTIVE;
					}
				}
				}
				break;

			case BUTTONBARSTATE_HIDING:
			{
				// We are hiding. What animation are we using?
				switch (m_buttonBar.animation)
				{
				default:
				case BUTTONBARANIMATION_NONE:
					// We are now HIDDEN
					SetButtonOverlayHeight(0);
					m_buttonBar.state = BUTTONBARSTATE_HIDDEN;
					break;

				case BUTTONBARANIMATION_SLIDE:
				{
					// Decrement height
					int32_t overlayHeight = GetButtonOverlayHeight();
					overlayHeight -= BUTTONBAR_SLIDE_ANIMATION_INCREMENT;
					if (overlayHeight > 0)
					{
						SetButtonOverlayHeight(overlayHeight);
					}
					else
					{
						SetButtonOverlayHeight(0);
						// Done hiding. We are now HIDDEN
						m_buttonBar.state = BUTTONBARSTATE_HIDDEN;
					}
				}
				}
			}
			break;
			}
		}
	}
	else
	{
		// Make sure that the button bar is always hidden in DISABLED state or INVISIBLE mode
		SetButtonOverlayHeight(0);
	}
}

static void _ButtonBarKeyCallbackDisable(void* pUserData)
{
	(void)pUserData;

	// This is called by the MMI before a screens enter function
	// We want to inhibit the callbacks until the keys are all released!
	m_isKeyCallbacksDisabled = TRUE;
}
