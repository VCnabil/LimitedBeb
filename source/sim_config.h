//------------------------------------------------------------------------------
//  TITLE :          sim_config.h
//  DESCRIPTION :    Configure the simulator.
//------------------------------------------------------------------------------
#ifndef __SIM_CONFIG_H__
#define __SIM_CONFIG_H__

/// @defgroup simconfig PC Simulator Parameters
/// Parameters for controlling the PC Simulator
/// @{

#include "platform_sysinfo.h"
#include "AppVersion.h"

/// SIM_PRODUCTID needs to be defined with the platform to simulate. Valid values are one of the following:
///     PLATFORM_CANVU355;
///     PLATFORM_CANVU570;
///     PLATFORM_CANVU700;
#define SIM_PRODUCTID	PLATFORM_CANVU355

/// @} // endgroup simconfig

#endif // #ifndef __SIM_CONFIG_H__
