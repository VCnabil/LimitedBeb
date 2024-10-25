//------------------------------------------------------------------------------
//  TITLE :           iouser.h
//------------------------------------------------------------------------------

#ifndef __IOUSER_H__
#define __IOUSER_H__

#include "project.h"

/// @defgroup ioboard IO Board Functions
/// Functions for controlling the IO Board
/// @{

/// Setup the IO
///
/// @return TRUE if succeeded, FALSE if failed.
BOOL IOUserInit(void);

/// @} // endgroup ioboard

#endif // #ifndef __IO_BOARD_USER_H__
