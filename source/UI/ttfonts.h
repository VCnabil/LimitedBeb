//------------------------------------------------------------------------------
//  TITLE :          ttfonts.h
//  DESCRIPTION :    Truetype Font Support
//------------------------------------------------------------------------------
#ifndef __TTFONTS_H__
#define __TTFONTS_H__

/// @defgroup ttfonts True Type Fonts Functions
/// Functions for controlling the True Type Fonts
/// @{

typedef enum _enumFONT_INDEX
{
	FONT_INDEX_TTMAIN = 0,
	FONT_INDEX_TTMONOSPACE,
	// Count of entries
	FONT_INDEX_END
} FONT_INDEX_T;

/// Loades the fonts into the library.
void FontInit(void);

/// @} // endgroup ttfonts

#endif // #ifndef __TTFONTS_H__
