//------------------------------------------------------------------------------
//  TITLE :         iouser.c
//------------------------------------------------------------------------------

#include "project.h"

static void _IOUserInitAnalogueChannels(void);

static void _IOUserReadInputs(void* pUserData = nullptr);

BOOL IOUserInit(void)
{
	// Check I/O Board is OK
	if (io_open() != IO_STATUS_SUCCESS)	
	{
		return FALSE;	// no point in setting up if I/O board off-line so exit 
	}

	// Sets up the analogue Modes (Resistance, Low Voltage (High Gain), High Voltage (Low Gain))
	_IOUserInitAnalogueChannels();

	// Register with the MMI system the Function used to read all the data from the io board
	MMIRegisterCallback(MMICALLBACKTYPE_PREPROCESS, _IOUserReadInputs, nullptr);

	return TRUE;
}

static void _IOUserInitAnalogueChannels(void)
{
	for (uint32_t channelIndex = 0; channelIndex < io_get_number_analog_inputs(); channelIndex++)
	{
		ANALOG_IN_CHANNEL_T channel = (ANALOG_IN_CHANNEL_T)(channelIndex + 1);
		io_set_analog_input_mode(channel, RESISTANCE); // +1 because the IO Board Driver starts at 1 not 0

		// You can also change the Filter Rate
		// A value of 1.0 is no filtering. The lower the value the more
		// filtering. The minimum value accepted is 0.01 which approximates
		// to a filter time constant of around 10 seconds. The default setting
		// is 0.1
		io_set_analog_input_filter_rate(channel, 0.5f);
	}
}

static void _IOUserReadInputs(void* pUserData)
{
	(void)pUserData;
	
	DBVAR_T dbData;
	for (uint32_t channelIndex = 0; channelIndex < io_get_number_analog_inputs(); channelIndex++)
	{
		if (io_get_analog_input_data(channelIndex + 1, &dbData.flt) == IO_STATUS_SUCCESS) // +1 because the IO Board Driver starts at 1 not 0
		{
			INPUT_MODE_T inputMode;
			if (io_get_analog_input_mode(channelIndex + 1, &inputMode) == IO_STATUS_SUCCESS) // +1 because the IO Board Driver starts at 1 not 0
			{
				if (inputMode == RESISTANCE)
				{
					Database_Set_IO(db_AnalogueInput_01_Resistance + channelIndex, &dbData, DBVARTYPE_FLOAT, 0);
				}
				else
				{
					Database_Set_IO(db_AnalogueInput_01_Voltage + channelIndex, &dbData, DBVARTYPE_FLOAT, 0);
				}
			}
		}
	}

	if (io_get_supply_voltage(&dbData.flt) == IO_STATUS_SUCCESS)
	{
		Database_Set_IO(db_SupplyVoltage, &dbData, DBVARTYPE_FLOAT, 0);
	}

	if (io_get_frequency_input_data_frequency(FREQ_IN_CHANNEL_1, &dbData.flt) == IO_STATUS_SUCCESS)
	{
		Database_Set_IO(db_FrequencyInput, &dbData, DBVARTYPE_FLOAT, 0);
	}

	for (uint32_t switchIndex = 0; switchIndex < io_get_number_digital_inputs(); switchIndex++)
	{
		BOOL switchActive = FALSE;
		uint16_t switchCount = 0;

		if (io_get_digital_input_data(switchIndex + 1, &switchActive, &switchCount) == IO_STATUS_SUCCESS) // +1 because the IO Board Driver starts at 1 not 0
		{
			dbData.flt = (float)switchActive;
			Database_Set_IO(db_DigitalInput_01_State + switchIndex, &dbData, DBVARTYPE_FLOAT, 0);
			dbData.flt = switchCount;
			Database_Set_IO(db_DigitalInput_01_Count + switchIndex, &dbData, DBVARTYPE_FLOAT, 0);
		}
	}

	for (uint32_t outputIndex = 0; outputIndex < io_get_number_digital_outputs(); outputIndex++)
	{
		if (io_get_digital_output_voltage(outputIndex + 1, &dbData.flt) == IO_STATUS_SUCCESS) // +1 because the IO Board Driver starts at 1 not 0
		{
			Database_Set_IO(db_DigitalOutputSenseVoltage_01 + outputIndex, &dbData, DBVARTYPE_FLOAT, 0);
		}
	}
}