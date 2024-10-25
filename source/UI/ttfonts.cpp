//------------------------------------------------------------------------------
//  TITLE :          ttfonts.c
//  DESCRIPTION :    Truetype Font Support
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDE
//------------------------------------------------------------------------------

#include "project.h"

void FontInit(void)
{
	FTFAddFont(TTFONT_MAIN, &TTFONT_MAIN_SIZE);
	FTFAddFont(TTFONT_MONOSPACE, &TTFONT_MONOSPACE_SIZE);
}