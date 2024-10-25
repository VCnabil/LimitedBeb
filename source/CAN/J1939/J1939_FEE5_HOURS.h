//------------------------------------------------------------------------------
//  TITLE :          PGN FEE5 Decode
//  DESCRIPTION :    CAN Decode
//------------------------------------------------------------------------------
#ifndef __PGN_FEE5_H__
#define __PGN_FEE5_H__

#define PGN_FEE5_HOURS_Engine_Hours_Revolutions 65253

/// Handles the decode of PGN FEE5
void J1939_PGN_FEE5_HOURS(CAN_PORTS_T canPort, CAN_MSG_T* pMsg);

#endif // #ifndef __PGN_FEE5_H__
