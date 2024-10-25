
#include "project.h"

// LCD backlight levels (0 - 10000)
static const uint32_t m_BacklightLevels[] =
{
	320, 450, 640, 900, 1280, 1800, 2560, 3620, 5120, 7160, 10000
};
static const uint32_t m_numberOfm_BacklightLevels = (sizeof(m_BacklightLevels) / sizeof(m_BacklightLevels[0]));

#define LIGHTING_OPACITY		100
static uint32_t m_currentLightIndex = 0;
static uint32_t m_currentKeypadLightIndex = 0;

static void _LightingCreate(void* pUserData = nullptr);
static void _LightingUpdate(void* pUserData = nullptr);
static void _LightingExit(void* pUserData = nullptr);
static void _LightingPlusCB(void* pUserData = nullptr);
static void _LightingMinusCB(void* pUserData = nullptr);
static void _KeyLightingPlusCB(void* pUserData = nullptr);
static void _KeyLightingMinusCB(void* pUserData = nullptr);

void LightingInit(void)
{
	uint32_t actualLCDLevel = lcd_get_saved_backlight_level();

	BOOL wasMatchFound = FALSE;

	for (uint32_t index = 0; index < m_numberOfm_BacklightLevels; index++)
	{
		if (actualLCDLevel == m_BacklightLevels[index])
		{
			m_currentLightIndex = (uint32_t)index;
			wasMatchFound = TRUE;
			break;
		}
	}

	if (wasMatchFound == FALSE)
	{
		// We never found a match with our table of Values, so we need to make sure the index has a "safe/sensible" value.
		m_currentLightIndex = m_numberOfm_BacklightLevels - 1;
	}

	uint32_t actualKeypadLevel = keypad_get_saved_backlight_level();

	wasMatchFound = FALSE;

	for (uint32_t index = 0; index < m_numberOfm_BacklightLevels; index++)
	{
		if (actualKeypadLevel == m_BacklightLevels[index])
		{
			m_currentKeypadLightIndex = (uint32_t)index;
			wasMatchFound = TRUE;
			break;
		}
	}

	if (wasMatchFound == FALSE)
	{
		// We never found a match with our table of Values, so we need to make sure the index has a "safe/sensible" value.
		m_currentKeypadLightIndex = m_numberOfm_BacklightLevels - 1;
	}
}

void LightingEnter(void)
{
	// Register us with the MMI as a POST-CREATE and POST-UPDATE callback
	MMIRegisterCallback(MMICALLBACKTYPE_POSTCREATE, _LightingCreate, nullptr);
	MMIRegisterCallback(MMICALLBACKTYPE_POSTUPDATE, _LightingUpdate, nullptr);

	// Force a full redraw (this will also redraw us!)
	MMIInvalidate();
}

static void _LightingCreate(void* pUserData)
{
	(void)pUserData;

	ButtonBarReset();

	ButtonBarSetMode(BUTTONBARMODE_VISIBLE_ALWAYS);
	ButtonBarSetHeight(48);
	
	ButtonBarSetKeyImages(KEYINDEX_1, &lcd_light_minus, &lcd_light_minus_s);
	ButtonBarSetKeyImages(KEYINDEX_2, &lcd_light_plus, &lcd_light_plus_s);
	ButtonBarSetKeyImages(KEYINDEX_3, &key_light_minus, &key_light_minus_s);
	ButtonBarSetKeyImages(KEYINDEX_4, &key_light_plus, &key_light_plus_s);
	ButtonBarSetKeyImages(KEYINDEX_5, &back, &back_s);
	
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_1, _LightingMinusCB, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_2, _LightingPlusCB, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_3, _KeyLightingMinusCB, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_4, _KeyLightingPlusCB, nullptr);
	ButtonBarRegisterKeyReleaseCallback(KEYINDEX_5, _LightingExit, nullptr);
}
//---------------------------------------------------------------------------->

static void _LightingUpdate(void* pUserData)
{
	(void)pUserData;

	// Draw our bitmap
	int32_t bitmapLeft = 10;
	int32_t bitmapTop = lcd_get_height()/2 - 60;
	outimageEx(bitmapLeft, bitmapTop, &lighting_bar, LIGHTING_OPACITY, ALPHA_INVARIANT, LAYER_FRONT);

	// Draw the LCD bar
	uint16_t barColour = MAKERGB565(0, 190, 0);
	int32_t left = (lcd_get_width() / 2) - 97;
	int32_t top = (lcd_get_height() / 2) - 50;
	int32_t right = (lcd_get_width() / 2) - 125 + 30 + ((230 * m_currentLightIndex) / (m_numberOfm_BacklightLevels - 1));
	int32_t bottom = (lcd_get_height() / 2) - 21;
	blockfillEx(left, top, right, bottom, barColour, LIGHTING_OPACITY, LAYER_FRONT);

	// Draw the key bar
	barColour = MAKERGB565(0, 190, 0);
	left = (lcd_get_width() / 2) - 97;
	top = (lcd_get_height() / 2) - 9;
	right = (lcd_get_width() / 2) - 125 + 30 + ((230 * m_currentKeypadLightIndex) / (m_numberOfm_BacklightLevels - 1));
	bottom = (lcd_get_height() / 2) + 19;
	blockfillEx(left, top, right, bottom, barColour, LIGHTING_OPACITY, LAYER_FRONT);
 }

static void _LightingExit(void* pUserData)
{
	// De-register our MMI callbacks
	MMIDeregisterCallback(MMICALLBACKTYPE_POSTUPDATE, _LightingUpdate, nullptr);
	MMIDeregisterCallback(MMICALLBACKTYPE_POSTCREATE, _LightingCreate, nullptr);

	// Make sure the Backlight level is saved.
	lcd_set_saved_backlight_level(m_BacklightLevels[m_currentLightIndex]);
	keypad_set_saved_backlight_level(m_BacklightLevels[m_currentKeypadLightIndex]);

	// Force a full screen redraw
	MMIInvalidate();
}

static void _LightingPlusCB(void* pUserData)
{
	if (m_currentLightIndex < (m_numberOfm_BacklightLevels - 1))
	{
		m_currentLightIndex++;
		lcd_set_current_backlight_level(m_BacklightLevels[m_currentLightIndex]);
	}
}

static void _LightingMinusCB(void* pUserData)
{
	if (m_currentLightIndex > 0)
	{
		m_currentLightIndex--;
		lcd_set_current_backlight_level(m_BacklightLevels[m_currentLightIndex]);
	}
}

static void _KeyLightingPlusCB(void* pUserData)
{
	if (m_currentKeypadLightIndex < (m_numberOfm_BacklightLevels - 1))
	{
		m_currentKeypadLightIndex++;
		keypad_set_current_backlight_level(m_BacklightLevels[m_currentKeypadLightIndex]);
	}
}

static void _KeyLightingMinusCB(void* pUserData)
{
	if (m_currentKeypadLightIndex > 0)
	{
		m_currentKeypadLightIndex--;
		keypad_set_current_backlight_level(m_BacklightLevels[m_currentKeypadLightIndex]);
	}
}