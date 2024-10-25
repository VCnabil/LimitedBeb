
#include "project.h"

void J1939_PGN_FEE5_HOURS(CAN_PORTS_T canPort, CAN_MSG_T* pMsg)
{
		uint32_t Data_32 = 0;
		float fData;
		DBVAR_T	DBData;

		// Total Hours of Operation
		//================================================
		if ((pMsg->msg_content[3] != DATA_NOT_AVAILABLE) && (pMsg->msg_content[3] != DATA_ERROR_INDICATOR))
		{
			/* engine hours is tranmitted as 0.05 hours/bit */
			/* get the data from bytes 1 to 4               */
			Data_32 = (uint32_t)(((uint32_t)(pMsg->msg_content[3]) << 24)
				| ((uint32_t)(pMsg->msg_content[2]) << 16)
				| ((uint32_t)(pMsg->msg_content[1]) << 8)
				| ((uint32_t)(pMsg->msg_content[0])));

			// Because of the way Floats are stored, there is the potential for this to be the "wrong" number for very large numbers.
			fData = (float)Data_32 * 0.05f * 60 * 60;	//	 stored in seconds
			DBData.flt = fData;
			Database_Set_CAN(db_TotalHoursOfOperation_247, &DBData, DBVARTYPE_FLOAT, canPort, 0, CANIDEXT_GETSOURCEADDR(pMsg->id));
		}

		// Total Revolutions
		//================================================
		if ((pMsg->msg_content[7] != DATA_NOT_AVAILABLE) && (pMsg->msg_content[7] != DATA_ERROR_INDICATOR))
		{
			/* Total Revolutions is 1000 Revs Per Bit */
			Data_32 = (uint32_t)(((uint32_t)(pMsg->msg_content[7]) << 24)
				| ((uint32_t)(pMsg->msg_content[6]) << 16)
				| ((uint32_t)(pMsg->msg_content[5]) << 8)
				| ((uint32_t)(pMsg->msg_content[4])));

			fData = (float)Data_32 * 1000;
			DBData.flt = fData;
			Database_Set_CAN(db_TotalRevolutions_249, &DBData, DBVARTYPE_FLOAT, canPort, 0, CANIDEXT_GETSOURCEADDR(pMsg->id));
		}
}