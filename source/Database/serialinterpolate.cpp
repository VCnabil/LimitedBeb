/*
*
* File to set up serial string interpreation from motherboard
* 
* Colby Scarbrough
* Oct 22 2024
* 
*/

#include "serialinterpolate.h"

extern FAULTS_DB m_FAULTS_DB[] = {

    { STA1, 0, "Idle Knob" },
    { STA1, 1, "Helm / Tiller" },
    { STA1, 2, "Roll command" },
    { STA1, 3, "Trim command" },
    { STA1, 4, "Joystick Y-axis" },
    { STA1, 5, "Joystick X-axis" },

    { STA2, 0, "Idle Knob" },
    { STA2, 1, "Helm / Tiller" },
    { STA2, 2, "Roll command" },
    { STA2, 3, "Trim command" },
    { STA2, 4, "Joystick Y-axis" },
    { STA2, 5, "Joystick X-axis" },

    { STA3, 0, "Idle Knob" },
    { STA3, 1, "Helm / Tiller" },
    { STA3, 2, "Roll command" },
    { STA3, 3, "Trim command" },
    { STA3, 4, "Joystick Y-axis" },
    { STA3, 5, "Joystick X-axis" },

    { NFE, 0, "Stbd Bucket NFU" },
    { NFE, 1, "Port Bucket NFU" },
    { NFE, 2, "Stbd Nozzle NFU" },
    { NFE, 3, "Port Nozzle NFU" },
    { NFE, 4, "Stbd Trim Tab / INT NFU" },
    { NFE, 5, "Port Trim Tab / INT NFU" },

    { SFE, 0, "Stbd Bucket feedback" },
    { SFE, 1, "Port Bucket feedback" },
    { SFE, 2, "Stbd Nozzle feedback" },
    { SFE, 3, "Port Nozzle feedback" },
    { SFE, 4, "Stbd Trim Tab / INT feedback" },
    { SFE, 5, "Port Trim Tab / INT feedback" },

    { CFE, 0, "Calibration Fault" },
    { GAL, 0, "General Alarm" }
};
