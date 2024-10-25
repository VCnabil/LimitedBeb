//------------------------------------------------------------------------------
//  TITLE :          buttonbar.h
//  DESCRIPTION :    Handling the buttons
//------------------------------------------------------------------------------
#ifndef __BUTTONBAR_H__
#define __BUTTONBAR_H__

#include "project.h"

/// @defgroup buttonbar Button Bar Functions
/// Functions for controlling the Button Bar creation
/// @{

//------------------------------------------------------------------------------
// TYPES AND CONSTANTS
//------------------------------------------------------------------------------

/// Maximum number of key mask hooks that can be registered
#define BUTTONBAR_MAX_KEYMASK_HOOKS				5

/// The timeout (in milliseconds) for the button bar when in BUTTONBARMODE_VISIBLE_WITH_TIMEOUT mode
#define BUTTONBAR_TIMEOUT_MS					5000

/// The increment (in pixels) when doing a slide animation
#define BUTTONBAR_SLIDE_ANIMATION_INCREMENT		2

/// Modes of the button bar
typedef enum
{
	// The button bar is completely disabled with no keys
	BUTTONBARMODE_DISABLED = 0,
	// The keys will generate callbacks but the button bar will never be shown
	BUTTONBARMODE_INVISIBLE,
	// The button bar pops up when a key is pressed and disappears after the timeout period
	BUTTONBARMODE_VISIBLE_WITH_TIMEOUT,
	// The button bar is always visible
	BUTTONBARMODE_VISIBLE_ALWAYS
} BUTTONBARMODE_T;

/// Animations for the button bar (only used when BUTTONBARMODE_VISIBLE_WITH_TIMEOUT is used!) 
typedef enum
{
	/// No animation (default)
	BUTTONBARANIMATION_NONE = 0,
	/// Slide in/out animation
	BUTTONBARANIMATION_SLIDE
} BUTTONBARANIMATION_T;

/// Indices of the keys
typedef enum
{
	KEYINDEX_1 = 0,
	KEYINDEX_2,
	KEYINDEX_3,
	KEYINDEX_4,
	KEYINDEX_5
} KEYINDEX_T;

/// Masks for the keys. These should be ORed together to create a mask for the KeypadRegisterKeyMaskHook() function
#define KEYMASK_1						0x01
#define KEYMASK_2						0x02
#define KEYMASK_3						0x04
#define KEYMASK_4						0x08
#define KEYMASK_5						0x10

/// Callback function type
typedef void (*BUTTONBAR_CALLBACK_T)(void* userData);

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

/// Initialise the button bar module. This should be called once at startup.
void ButtonBarInit(void);

/// Reset the button bar to its default (disabled) state. It clears all the settings and callbacks. This can be called at any time and is automatically called BEFORE a screens create function is called (via an MMI Pre Create Hook).
void ButtonBarReset(void* pUserData = nullptr);

/// Set the mode of the button bar.
/// @param mode - Specifies the mode used by the button bar: Disabled, Invisible, Visible with Timeout, Always Visible
void ButtonBarSetMode(BUTTONBARMODE_T mode);

/// Set the animation mode of the button bar when hiding/displaying a menu.
/// @param animation - Specifies the animation type: None, Slide
void ButtonBarSetAnimation(BUTTONBARANIMATION_T animation);

/// Set the height of the button bar.
/// @param height - Specifies the height of the button bar. Default 48px
void ButtonBarSetHeight(uint32_t height);

/// Set the background colour of the button bar. This can be ALPHA_COLOUR for a transparent background.
/// @param colour - Specifies the background color value
void ButtonBarSetBackgroundColour(uint16_t colour);

/// Set the background image for the button bar.
/// @param pBitmap - Pointer to Bitmap
void ButtonBarSetBackgroundImage(const void* pBitmap);

/// Set the image for a key.
/// @param keyIndex - Specifies the button index
/// @param pUpBitmap - Pointer to the image that will be displayed when the button is not pressed
/// @param pDownBitmap - Pointer to the image that will be displayed when the button is pressed 
void ButtonBarSetKeyImages(KEYINDEX_T keyIndex, const void* pUpBitmap, const void* pDownBitmap);

/// Set the line 1 and line 2 text for the key.
/// @param keyIndex - Specifies the button index
/// @param fontIndex - Specifies the index of the font to be used
/// @param fontSize - Specifies the font size
/// @param fontColour - Specifies the colour used for drawing
/// @param pTextLine1 - Pointer of type char to the first line of text
/// @param pTextLine2 - Pointer of type char to the second line of text
void ButtonBarSetKeyText(KEYINDEX_T keyIndex, FONT_INDEX_T fontIndex, uint32_t fontSize, uint16_t fontColour, const char* pTextLine1, const char* pTextLine2);

/// Test if a key is pressed or not.
/// @param keyIndex - Specifies the button index
/// @returns TRUE if key is pressed, otherwise FALSE
BOOL ButtonBarIsKeyPressed(KEYINDEX_T keyIndex);

/// Register for key release events.
/// @param keyIndex - Specifies the button index
/// @param pCallbackFunc - Pointer to the callback function 
void ButtonBarRegisterKeyReleaseCallback(KEYINDEX_T keyIndex, BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData);

/// De-register for key release events.
/// @param keyIndex - Specifies the button index
void ButtonBarDeregisterKeyReleaseCallback(KEYINDEX_T keyIndex);

/// Register for key press events.
/// @param keyIndex - Specifies the button index
/// @param pCallbackFunc - Pointer to the callback function
void ButtonBarRegisterKeyPressCallback(KEYINDEX_T keyIndex, BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData);

/// De-register for key press events
/// @param keyIndex - Specifies the button index
void ButtonBarDeregisterKeyPressCallback(KEYINDEX_T keyIndex);

/// Register for key hold/repeat events.
/// @param keyIndex - Specifies the button index
/// @param pCallbackFunc - Pointer to the callback function 
/// @param keyHoldTimeMs - Value (in milliseconds) required for callback function to be triggered
/// @param keyRepeatTimeMs - Value (in milliseconds) after which the callback function is re-triggered
void ButtonBarRegisterKeyHoldCallback(KEYINDEX_T keyIndex, BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData, uint32_t keyHoldTimeMs, uint32_t keyRepeatTimeMs);

/// De-register for key hold/repeat events.
/// @param keyIndex - Specifies the button index
void ButtonBarDeregisterKeyHoldCallback(KEYINDEX_T keyIndex);

/// Register for key-mask (multiple key) events.
/// @param keyMask - Specifies the Key Mask
/// @param holdTimeMs - Value (in milliseconds) required for callback function to be triggered
/// @param pCallbackFunc - Pointer to the callback function 
/// @returns TRUE on success, otherwise FALSE
BOOL ButtonBarRegisterKeyMaskCallback(uint8_t keyMask, uint32_t holdTimeMs, BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData);

/// De-register for key-mask (multiple key) events.
/// @param pCallbackFunc - Pointer to the callback function 
/// @returns TRUE on success, otherwise FALSE
BOOL ButtonBarDeregisterKeyMaskCallback(BUTTONBAR_CALLBACK_T pCallbackFunc, void* userData);

/// @} // endgroup buttonbar

#endif // #ifndef __BUTTONBAR_H__
