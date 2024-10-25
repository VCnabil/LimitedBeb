/*
*
* File to set up system options
*
* Colby Scarbrough
* Oct 22 2024
*
*/

#include "systemoptions.h"

extern SYSTEMOPTIONS_DB m_SYSTEMOPTIONS_DB[] = {
	{ "DatMode", "232", "GPSI", false}, //true = 232, false = GPSI
	{ "232 Xmit", "ON", "OFF", true},
	{ "NozMap", "FLIP", "NORM", false}, //true = FLIP, false = NORM
	{ "BktMap", "FLIP", "NORM", false},
	{ "IntMap", "FLIP", "NORM", false},
	{ "IntSteer", "ON", "OFF", true},
	{ "StaType", "MAIN", "WING", true}, //true = MAIN, false = WING
	{ "Comm Mode", "4", "5", false} //true = 4, false = 5
};

extern const int system_options_count = sizeof(m_SYSTEMOPTIONS_DB) / sizeof(m_SYSTEMOPTIONS_DB[0]);