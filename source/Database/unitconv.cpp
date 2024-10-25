//------------------------------------------------------------------------------
//  TITLE :- Main parameter database - database.c
//  AUTHOR :         
//  DATE :           
//
//  DESCRIPTION :- Main database for all the parameters used in the system.
//
//------------------------------------------------------------------------------

#include "syslib.h"
#include "Database/unitconv.h"
#include "NVSettings/settings.h"

#include <assert.h>

typedef enum
{
	PRESSUREUNITS_Kpa = 0,
	PRESSUREUNITS_PSI,
	PRESSUREUNITS_Bar,
	PRESSUREUNITS_Size,
}PRESSUREUNITS_T;

static unsigned char m_PressureText[PRESSUREUNITS_Size][50]=
{
	"kPa",
	"PSI",
	"Bar",
};

typedef enum
{
	TEMPERATUREUNITS_DegC = 0,
	TEMPERATUREUNITS_DegF,
	TEMPERATUREUNITS_Size,
}TEMPERATUREUNITS_T;

static unsigned char m_TemperatureText[TEMPERATUREUNITS_Size][50]=
{
	"\xC2\xB0\x43",
	"\xC2\xB0\x46",
};

typedef enum
{
	VOLUMEUNITS_L = 0,
	VOLUMEUNITS_Gal,
	VOLUMEUNITS_Imp_Gal,
	VOLUMEUNITS_Size,
}VOLUMEUNITS_T;

static unsigned char m_VolumeText[VOLUMEUNITS_Size][50]=
{
	"L",
	"Gal",
	"I Gal",
};

typedef enum
{
	DISTANCEUNITS_NM = 0,
	DISTANCEUNITS_Miles,
	DISTANCEUNITS_Km,
	DISTANCEUNITS_Size,
}DISTANCEUNITS_T;

static unsigned char m_DistanceText[DISTANCEUNITS_Size][50]=
{
	"NM",
	"M",
	"Km",
};

typedef enum
{
	PERCENTUNITS_Percent = 0,
	PERCENTUNITS_Size,
}PERCENTUNITS_T;

static unsigned char m_PercentageText[PERCENTUNITS_Size][50]=
{
	"%",
};

typedef enum
{
	FLOWRATEUNITS_L_Ph = 0,
	FLOWRATEUNITS_Gal_Ph,
	FLOWRATEUNITS_Imp_Gal_Ph,
	FLOWRATEUNITS_Size,
}FLOWRATEUNITS_T;

static unsigned char m_FlowRateText[FLOWRATEUNITS_Size][50]=
{
	"L/h",
	"Gal/h",
	"I Gal/h",
};

typedef enum
{
	TIMEUNITS_Seconds = 0,
	TIMEUNITS_Hours,
	TIMEUNITS_Size,
}TIMEUNITS_T;

static unsigned char m_TimeText[TIMEUNITS_Size][50]=
{
	"s",
	"Hrs",
};

typedef enum
{
	MASSUNITS_Kg = 0,
	MASSUNITS_Size,
}MASSUNITS_T;

static unsigned char m_MassText[MASSUNITS_Size][50]=
{
	"Kg",
};

typedef enum
{
	RPMUNITS_RPM = 0,
	RPMUNITS_Size,
}RPMUNITS_T;

static unsigned char m_RPMText[RPMUNITS_Size][50]=
{
	"RPM",
};

typedef enum
{
	SPEEDUNITS_Knots = 0,
	SPEEDUNITS_Miles_Ph,
	SPEEDUNITS_Km_Ph,
	SPEEDUNITS_Size,
}SPEEDUNITS_T;

static unsigned char m_SpeedText[SPEEDUNITS_Size][50]=
{
	"Kts",
	"M/h",
	"Km/h",
};

typedef enum
{
	ELECTRICALUNITS_Volts = 0,
	ELECTRICALUNITS_Size,
}ELECTRICALUNITS_T;

static unsigned char m_ElectricalText[ELECTRICALUNITS_Size][50]=
{
	"V",
};

typedef enum
{
	ECONOMYUNITS_NM_P_L = 0,
	ECONOMYUNITS_NM_P_GAL,
	ECONOMYUNITS_NM_P_IGAL,

	ECONOMYUNITS_Miles_P_L,
	ECONOMYUNITS_Miles_P_GAL,
	ECONOMYUNITS_Miles_P_IGAL,

	ECONOMYUNITS_km_P_L,
	ECONOMYUNITS_km_P_GAL,
	ECONOMYUNITS_km_P_IGAL,
	ECONOMYUNITS_Size,
}ECONOMYUNITS_T;

static unsigned char m_EconomyText[ECONOMYUNITS_Size][50]=
{
	"NM/L",
	"NM/Gal",
	"NM/I Gal",

	"M/L",
	"M/Gal",
	"M/I Gal",

	"Km/L",
	"Km/Gal",
	"Km/I Gal",
};

typedef enum
{
	DEPTHUNITS_Meters = 0,
	DEPTHUNITS_Feet,
	DEPTHUNITS_Size,
}DEPTHUNITS_T;

static unsigned char m_DepthText[DEPTHUNITS_Size][50]=
{
	"m",
	"ft",
};

typedef enum
{
	BEARINGUNITS_True = 0,
	BEARINGUNITS_Magnetic,
	BEARINGUNITS_Size,
}BEARINGUNITS_T;

static unsigned char m_BearingText[BEARINGUNITS_Size][50]=
{
	"True",
	"Mag",
};

typedef enum
{
	ANGLEUNITS_Degrees = 0,
	ANGLEUNITS_Radians,
	ANGLEUNITS_Size,
}ANGLEUNITS_T;

static unsigned char m_AngleText[ANGLEUNITS_Size][50]=
{
	"Deg",
	"Rad",
};

typedef enum
{
	COMMANDUNITS_Flag = 0,
	COMMANDUNITS_Size,
}COMMANDUNITS_T;

typedef enum
{
	CURRENTUNITS_Amps = 0,
	CURRENTUNITS_Size,
}CURRENTUNITS_T;

static unsigned char m_CurrentText[CURRENTUNITS_Size][50]=
{
	"A",
};

typedef enum
{
	FREQUENCYUNITS_Hertz = 0,
	FREQUENCYUNITS_Size,
}FREQUENCYUNITS_T;

static const unsigned char m_FrequencyText[FREQUENCYUNITS_Size][50]=
{
	"Hz",
};

typedef enum
{
	RESISTANCEUNITS_Ohms = 0,
	RESISTANCEUNITS_Size,
}RESISTANCEUNITS_T;

static const unsigned char m_ResistanceText[RESISTANCEUNITS_Size][50] =
{
	"Ohms",
};

static void _Convert_PRESSURE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Pressure_Bar(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Pressure_PSI(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_TEMPERATURE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Temperature_DegF(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_VOLUME(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Volume_Gal(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Volume_Imp_Gal(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_DISTANCE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Distance_m(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Distance_Km(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_FLOW_RATE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_FlowRate_Gal_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_FlowRate_Imp_Gal_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_SPEED(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Speed_m_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Speed_Km_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_PERCENT(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_TIME(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Time_hrs(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_MASS(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_RPM(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_ELECTRICAL(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Economy(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_COMMAND(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_CURRENT(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_FREQUENCY(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_DEPTH(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Depth_Feet(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_BEARING(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_Bearing_Magnetic(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

static void _Convert_Angle_Radians(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);
static void _Convert_BEARING(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits);

void _Convert_PRESSURE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case PRESSUREUNITS_Kpa:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case PRESSUREUNITS_PSI:
		_Convert_Pressure_PSI(p_DB_Element,CurrentUnits);
		break;
	case PRESSUREUNITS_Bar:
		_Convert_Pressure_Bar(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Pressure Units" && FALSE);
		break;
	}
}

void _Convert_Pressure_Bar(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_Kpa_TO_Bar((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_Kpa_TO_Bar((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_Kpa_TO_Bar((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_Kpa_TO_Bar((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_Kpa_TO_Bar(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_Pressure_PSI(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_Kpa_TO_PSI((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_Kpa_TO_PSI((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_Kpa_TO_PSI((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_Kpa_TO_PSI((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_Kpa_TO_PSI(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_TEMPERATURE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case TEMPERATUREUNITS_DegC:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case TEMPERATUREUNITS_DegF:
		_Convert_Temperature_DegF(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Temperature Units" && FALSE);
		break;
	}
}

void _Convert_Temperature_DegF(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_DegC_TO_DegF((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_DegC_TO_DegF((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_DegC_TO_DegF((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_DegC_TO_DegF((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_DegC_TO_DegF(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_VOLUME(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case VOLUMEUNITS_L:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case VOLUMEUNITS_Gal:
		_Convert_Volume_Gal(p_DB_Element,CurrentUnits);
		break;
	case VOLUMEUNITS_Imp_Gal:
		_Convert_Volume_Imp_Gal(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Volume Units" && FALSE);
		break;
	}
}

void _Convert_Volume_Gal(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_L_TO_Gal((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_L_TO_Gal((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_L_TO_Gal((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_L_TO_Gal((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_L_TO_Gal(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_Volume_Imp_Gal(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_L_ph_TO_Imp_Gal_ph(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_DISTANCE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case DISTANCEUNITS_NM:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case DISTANCEUNITS_Miles:
		_Convert_Distance_m(p_DB_Element,CurrentUnits);
		break;
	case DISTANCEUNITS_Km:
		_Convert_Distance_Km(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Volume Units" && FALSE);
		break;
	}
}

void _Convert_Distance_m(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_NM_TO_m((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_NM_TO_m((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_NM_TO_m((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_NM_TO_m((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_NM_TO_m(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_Distance_Km(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_NM_TO_Km((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_NM_TO_Km((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_NM_TO_Km((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_NM_TO_Km((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_NM_TO_Km(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_FLOW_RATE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case FLOWRATEUNITS_L_Ph:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case FLOWRATEUNITS_Gal_Ph:
		_Convert_FlowRate_Gal_ph(p_DB_Element,CurrentUnits);
		break;
	case FLOWRATEUNITS_Imp_Gal_Ph:
		_Convert_FlowRate_Imp_Gal_ph(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Volume Units" && FALSE);
		break;
	}
}

void _Convert_FlowRate_Gal_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_L_ph_TO_Gal_ph((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_L_ph_TO_Gal_ph((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_L_ph_TO_Gal_ph((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_L_ph_TO_Gal_ph((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_L_ph_TO_Gal_ph(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_FlowRate_Imp_Gal_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_L_ph_TO_Imp_Gal_ph((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_L_ph_TO_Imp_Gal_ph(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_SPEED(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case SPEEDUNITS_Knots:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case SPEEDUNITS_Miles_Ph:
		_Convert_Speed_m_ph(p_DB_Element,CurrentUnits);
		break;
	case SPEEDUNITS_Km_Ph:
		_Convert_Speed_Km_ph(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Speed Units" && FALSE);
		break;
	}
}

void _Convert_Speed_m_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_Knots_TO_m_ph((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_Knots_TO_m_ph((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_Knots_TO_m_ph((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_Knots_TO_m_ph((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_Knots_TO_m_ph(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_Speed_Km_ph(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_Knots_TO_Km_ph((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_Knots_TO_Km_ph((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_Knots_TO_Km_ph((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_Knots_TO_Km_ph((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_Knots_TO_Km_ph(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_Economy(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case ECONOMYUNITS_NM_P_L:
		// No Converstion is nessesary
		break;

	case ECONOMYUNITS_NM_P_GAL:
		_Convert_Volume_Gal(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_NM_P_IGAL:
		_Convert_Volume_Imp_Gal(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_Miles_P_L:
		_Convert_Distance_m(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_Miles_P_GAL:
		_Convert_Volume_Gal(p_DB_Element, CurrentUnits);
		_Convert_Distance_m(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_Miles_P_IGAL:
		_Convert_Volume_Imp_Gal(p_DB_Element, CurrentUnits);
		_Convert_Distance_m(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_km_P_L:
		_Convert_Distance_Km(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_km_P_GAL:
		_Convert_Volume_Gal(p_DB_Element, CurrentUnits);
		_Convert_Distance_Km(p_DB_Element, CurrentUnits);
		break;

	case ECONOMYUNITS_km_P_IGAL:
		_Convert_Volume_Imp_Gal(p_DB_Element, CurrentUnits);
		_Convert_Distance_Km(p_DB_Element, CurrentUnits);
		break;

	default:
		assert("Bad Economy Units" && FALSE);
		break;
	}
}

void _Convert_PERCENT(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_TIME(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch (CurrentUnits)
	{
	case TIMEUNITS_Seconds:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case TIMEUNITS_Hours:
		_Convert_Time_hrs(p_DB_Element, CurrentUnits);
		break;
	default:
		assert("Bad Time Units" && FALSE);
	}
}

void _Convert_Time_hrs(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch (p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_sec_TO_hr((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_sec_TO_hr((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_sec_TO_hr((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_sec_TO_hr((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_sec_TO_hr(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_MASS(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_RPM(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_ELECTRICAL(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_COMMAND(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_CURRENT(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_FREQUENCY(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_REVS(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_RESISTANCE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	// No Converstion is nessesary
	(void)p_DB_Element;
	(void)CurrentUnits;
}

void _Convert_ANGLE(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case ANGLEUNITS_Degrees:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case ANGLEUNITS_Radians:
		_Convert_Angle_Radians(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Bearing Units" && FALSE);
		break;
	}
}

void _Convert_Angle_Radians(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_Deg_TO_Rad((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_Deg_TO_Rad((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_Deg_TO_Rad((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_Deg_TO_Rad((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_Deg_TO_Rad(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_BEARING(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case BEARINGUNITS_True:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case BEARINGUNITS_Magnetic:
		_Convert_Bearing_Magnetic(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Bearing Units" && FALSE);
		break;
	}
}

void _Convert_Bearing_Magnetic(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	// Converting from True to Magnetic is more complicated, the Variance comes in over the NMEA2000. Assuming we have it. Add Westerly, Subtract Easterly
	// http://www.schoolofsailing.net/trueandmagnetic.html

	float Variation = UnitConvertGetMagneticVariationOffset();

	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)((float)p_DB_Element->Data.ui - Variation);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)((float)p_DB_Element->Data.i - Variation);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)((float)p_DB_Element->Data.ulg - Variation);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)((float)p_DB_Element->Data.lg - Variation);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = (float)(p_DB_Element->Data.flt - Variation);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void _Convert_DEPTH(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	switch(CurrentUnits)
	{
	case DEPTHUNITS_Meters:
		// Nothing needs doing here, as this is the "Database Units"
		break;
	case DEPTHUNITS_Feet:
		_Convert_Depth_Feet(p_DB_Element,CurrentUnits);
		break;
	default:
		assert("Bad Depth Units" && FALSE);
		break;
	}
}

void _Convert_Depth_Feet(DBELEMENT_T * p_DB_Element, uint8_t CurrentUnits)
{
	(void)CurrentUnits;
	switch(p_DB_Element->DataType)
	{
	case DBVARTYPE_UNSIGNED_INT:
		p_DB_Element->Data.ui = (uint32_t)CONV_M_TO_f((float)p_DB_Element->Data.ui);
		break;
	case DBVARTYPE_INT:
		p_DB_Element->Data.i = (int32_t)CONV_M_TO_f((float)p_DB_Element->Data.i);
		break;
	case DBVARTYPE_UNSIGNED_LONG:
		p_DB_Element->Data.ulg = (uint32_t)CONV_M_TO_f((float)p_DB_Element->Data.ulg);
		break;
	case DBVARTYPE_LONG:
		p_DB_Element->Data.lg = (int32_t)CONV_M_TO_f((float)p_DB_Element->Data.lg);
		break;
	case DBVARTYPE_FLOAT:
		p_DB_Element->Data.flt = CONV_M_TO_f(p_DB_Element->Data.flt);
		break;
		//////////////////////////////////////////////////////////////////////////
		// These are Bad types as far as unit conversion is concerned.
	case DBVARTYPE_UNSIGNED_CHAR_ARRAY_FOUR_ELEMENT:
	case DBVARTYPE_POINTER:
	default:
		assert("Bad Data Type" && FALSE);
		break;
	}
}

void UnitConvert_To_Current_Units(DBELEMENT_T * p_DB_Element)
{
	uint8_t CurrentUnits=0;

	switch(p_DB_Element->UnitType)
	{
		case DBUNITS_PRESSURE:
			CurrentUnits = SettingsUnitGetPressureUnits();
			_Convert_PRESSURE(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_TEMPERATURE:
			CurrentUnits = SettingsUnitGetTemperatureUnits();
			_Convert_TEMPERATURE(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_VOLUME:
			CurrentUnits = SettingsUnitGetVolumeUnits();
			_Convert_VOLUME(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_DISTANCE:
			CurrentUnits = SettingsUnitGetDistanceUnits();
			_Convert_DISTANCE(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_PERCENT:
			_Convert_PERCENT(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_FLOW_RATE:
			CurrentUnits = SettingsUnitGetFuelRateUnits();
			_Convert_FLOW_RATE(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_TIME:
			CurrentUnits = SettingsUnitGetTimeUnits();
			_Convert_TIME(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_MASS:
			_Convert_MASS(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_RPM:
			_Convert_RPM(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_SPEED:
			CurrentUnits = SettingsUnitGetSpeedUnits();
			_Convert_SPEED(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_ELECTRICAL:
			_Convert_ELECTRICAL(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_ECONOMY:
			 CurrentUnits = SettingsUnitGetEconomyUnits();
			_Convert_Economy(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_NONE:
		case DBUNITS_COMMAND:
			_Convert_COMMAND(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_DEPTH:
			CurrentUnits = SettingsUnitGetDepthUnits();
			_Convert_DEPTH(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_BEARING:
			CurrentUnits = SettingsUnitGetBearingUnits();
			_Convert_BEARING(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_ANGLE:
			CurrentUnits = SettingsUnitGetAngleUnits();
			_Convert_ANGLE(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_CURRENT:
			_Convert_CURRENT(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_FREQUENCY:
			_Convert_FREQUENCY(p_DB_Element,CurrentUnits);
			break;
		case DBUNITS_REVS:
			_Convert_REVS(p_DB_Element, CurrentUnits);
			break;
		case DBUNITS_RESISTANCE:
			_Convert_RESISTANCE(p_DB_Element, CurrentUnits);
			break;
		//////////////////////////////////////////////////////////////////////////
		// Both these are not valid.
		case DBUNITS_END:
		default:
			assert("Bad Unit Type" && FALSE);
			break;
	}
}

void UnitConvert_To_Desired_Units(DBELEMENT_T * p_DB_Element, uint8_t desiredUnits)
{
	switch(p_DB_Element->UnitType)
	{
	case DBUNITS_PRESSURE:
		_Convert_PRESSURE(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_TEMPERATURE:
		_Convert_TEMPERATURE(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_VOLUME:
		_Convert_VOLUME(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_DISTANCE:
		_Convert_DISTANCE(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_PERCENT:
		_Convert_PERCENT(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_FLOW_RATE:
		_Convert_FLOW_RATE(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_TIME:
		_Convert_TIME(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_MASS:
		_Convert_MASS(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_RPM:
		_Convert_RPM(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_SPEED:
		_Convert_SPEED(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_ELECTRICAL:
		_Convert_ELECTRICAL(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_ECONOMY:
		_Convert_Economy(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_COMMAND:
	case DBUNITS_NONE:
		_Convert_COMMAND(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_DEPTH:
		_Convert_DEPTH(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_BEARING:
		_Convert_BEARING(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_ANGLE:
		_Convert_ANGLE(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_CURRENT:
		_Convert_CURRENT(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_FREQUENCY:
		_Convert_FREQUENCY(p_DB_Element,desiredUnits);
		break;
	case DBUNITS_REVS:
		_Convert_REVS(p_DB_Element, desiredUnits);
		break;
	case DBUNITS_RESISTANCE:
		_Convert_RESISTANCE(p_DB_Element, desiredUnits);
		break;
		//////////////////////////////////////////////////////////////////////////
		// Both these are not valid.
	case DBUNITS_END:
	default:
		assert("Bad Unit Type" && FALSE);
		break;
	}
}

float UnitConvertByTypeToDatabaseUnits(DBUNITS_T UnitType, uint32_t SourceUnits, float Data)
{
	float returnValue = 0;
	
	if (SourceUnits == 0)
	{
		// Database units are 0, so we dont need to convert!
		returnValue = Data;
	}
	else
	{
		switch(UnitType)
		{
		case DBUNITS_PRESSURE:
			if (SourceUnits == 1)
			{
				returnValue = Data / 0.1450f;
			}
			else
			{
				returnValue = Data / 0.01f;
			}
			break;
		case DBUNITS_TEMPERATURE:
			returnValue = (Data - 32) * (5.0f / 9.0f);
			break;
		case DBUNITS_VOLUME:
			if (SourceUnits == 1)
			{
				returnValue = Data / 0.264172f;
			}
			else
			{
				returnValue = Data / 0.219969f;
			}
			break;
		case DBUNITS_DISTANCE:
			if (SourceUnits == 1)
			{
				returnValue = Data / 1.1507f;
			}
			else
			{
				returnValue = Data / 1.8520f;
			}
			break;
		case DBUNITS_FLOW_RATE:
			if (SourceUnits == 1)
			{
				returnValue = Data / 0.264172f;
			}
			else
			{
				returnValue = Data / 0.219969f;
			}
			break;
		case DBUNITS_SPEED:
			if (SourceUnits == 1)
			{
				returnValue = Data / 1.15077945f;
			}
			else
			{
				returnValue = Data / 1.85200f;
			}
			break;
		case DBUNITS_ECONOMY:
			if (SourceUnits == 1)
			{
				returnValue = Data / 0.264172f;
			}
			else
			{
				returnValue = Data / 0.2199f;
			}
			break;
		case DBUNITS_DEPTH:
			returnValue = Data / 3.2808f;
			break;
		case DBUNITS_BEARING:
			returnValue = Data + UnitConvertGetMagneticVariationOffset();
			break;
		case DBUNITS_ANGLE:
			returnValue = Data / 0.0175f;
			break;
		case DBUNITS_TIME:
			returnValue = Data * 3600.0f;

		case DBUNITS_PERCENT:
		case DBUNITS_MASS:
		case DBUNITS_RPM:
		case DBUNITS_ELECTRICAL:
		case DBUNITS_COMMAND:
		case DBUNITS_CURRENT:
		case DBUNITS_FREQUENCY:
		case DBUNITS_REVS:
		case DBUNITS_NONE:
		case DBUNITS_RESISTANCE:
			assert("Should not get here as these only have one type of unit, so should be in the top half of the if!" && FALSE);
			break;
		case DBUNITS_END:
		default:
			assert("Bad Unit Type" && FALSE);
			break;
		}
	}
	
	return returnValue;
}

float UnitConvertGetMagneticVariationOffset(void)
{
	float Variation = 0;
	uint32_t databaseIndex = db_MagneticVariation;
	uint32_t engineIndex = 0;
	DBELEMENT_T DB_Variation;

	for (engineIndex = 0; engineIndex < DB_INSTANCE_INVALID; engineIndex++)
	{
		if (DataBase_Get(&DB_Variation, databaseIndex, engineIndex))
		{
			if (DB_Variation.Timer)
			{
				// item exists and is Valid!
				Variation = DataBase_GetDataFromElementAsFloat(&DB_Variation);

				// we have a valid Variation so break the loop and return that!
				break;
			}
		}
	}

	return Variation;
}

const char * UnitGetStringDescriptor(DBUNITS_T UnitType)
{

	switch (UnitType)
	{
	case DBUNITS_PRESSURE:
		return (const char *)cucSystemWords[wUNITS_Pressure][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_TEMPERATURE:
		return (const char *)cucSystemWords[wUNITS_Temperature][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_VOLUME:
		return (const char *)cucSystemWords[wUNITS_Volume][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_DISTANCE:
		return (const char *)cucSystemWords[wUNITS_Distance][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_PERCENT:
		return (const char *)cucSystemWords[wUNITS_Percent][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_FLOW_RATE:
		return (const char *)cucSystemWords[wUNITS_FlowRate][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_TIME:
		return (const char *)cucSystemWords[wUNITS_Time][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_MASS:
		return (const char *)cucSystemWords[wUNITS_Mass][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_RPM:
		return (const char *)cucSystemWords[wUNITS_RPM][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_SPEED:
		return (const char *)cucSystemWords[wUNITS_Speed][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_ELECTRICAL:
		return (const char *)cucSystemWords[wUNITS_Electrical][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_ECONOMY:
		return (const char *)cucSystemWords[wUNITS_Economy][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_DEPTH:
		return (const char *)cucSystemWords[wUNITS_Depth][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_BEARING:
		return (const char *)cucSystemWords[wUNITS_Bearing][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_ANGLE:
		return (const char *)cucSystemWords[wUNITS_Angle][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_CURRENT:
		return (const char *)cucSystemWords[wUNITS_Current][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_FREQUENCY:
		return (const char *)cucSystemWords[wUNITS_Frequency][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_REVS:
		return (const char *)cucSystemWords[wUNITS_Revs][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_RESISTANCE:
		return (const char *)cucSystemWords[wUNITS_Resistance][SettingsGetLanguageIndex()];
		break;
	case DBUNITS_NONE:
	case DBUNITS_COMMAND:
		return "";
		break;
		//////////////////////////////////////////////////////////////////////////
		// Both these are not valid.
	case DBUNITS_END:
	default:
		assert("Bad Unit Type" && FALSE);
		return "";
		break;
	}
}

const char * UnitGetStringCurrentUnits(DBUNITS_T UnitType)
{
	return UnitGetStringSpecificUnits(UnitType, SettingsUnitGetUnits(UnitType));
}

const char * UnitGetStringSpecificUnits(DBUNITS_T UnitType, uint8_t desiredUnits)
{
	if (desiredUnits <= UnitGetMaxValid(UnitType))
	{
		switch(UnitType)
		{
		case DBUNITS_PRESSURE:
			return (const char *)m_PressureText[desiredUnits];
			break;
		case DBUNITS_TEMPERATURE:
			return (const char *)m_TemperatureText[desiredUnits];
			break;
		case DBUNITS_VOLUME:
			return (const char *)m_VolumeText[desiredUnits];
			break;
		case DBUNITS_DISTANCE:
			return (const char *)m_DistanceText[desiredUnits];
			break;
		case DBUNITS_PERCENT:
			return (const char *)m_PercentageText[desiredUnits];
			break;
		case DBUNITS_FLOW_RATE:
			return (const char *)m_FlowRateText[desiredUnits];
			break;
		case DBUNITS_TIME:
			return (const char *)m_TimeText[desiredUnits];
			break;
		case DBUNITS_MASS:
			return (const char *)m_MassText[desiredUnits];
			break;
		case DBUNITS_RPM:
			return (const char *)m_RPMText[desiredUnits];
			break;
		case DBUNITS_SPEED:
			return (const char *)m_SpeedText[desiredUnits];
			break;
		case DBUNITS_ELECTRICAL:
			return (const char *)m_ElectricalText[desiredUnits];
			break;
		case DBUNITS_ECONOMY:
			return (const char *)m_EconomyText[desiredUnits];
			break;
		case DBUNITS_DEPTH:
			return (const char *)m_DepthText[desiredUnits];
			break;
		case DBUNITS_BEARING:
			return (const char *)m_BearingText[desiredUnits];
			break;
		case DBUNITS_ANGLE:
			return (const char *)m_AngleText[desiredUnits];
			break;
		case DBUNITS_CURRENT:
			return (const char *)m_CurrentText[desiredUnits];
			break;
		case DBUNITS_FREQUENCY:
			return (const char *)m_FrequencyText[desiredUnits];
			break;
		case DBUNITS_RESISTANCE:
			return (const char *)m_ResistanceText[desiredUnits];
			break;
		case DBUNITS_NONE:
		case DBUNITS_COMMAND:
			return "";
			break;
		case DBUNITS_REVS:
			return "Revs";
			break;
			//////////////////////////////////////////////////////////////////////////
			// Both these are not valid.
		case DBUNITS_END:
		default:
			assert("Bad Unit Type" && FALSE);
			return "";
			break;
		}
	}
	else
	{
		return "";
	}
}

uint8_t UnitGetMaxValid(DBUNITS_T UnitType)
{
	uint8_t returnValue = 0;

	switch(UnitType)
	{
	case DBUNITS_PRESSURE:
		returnValue = PRESSUREUNITS_Size - 1;
		break;
	case DBUNITS_TEMPERATURE:
		returnValue = TEMPERATUREUNITS_Size - 1;
		break;
	case DBUNITS_VOLUME:
		returnValue = VOLUMEUNITS_Size - 1;
		break;
	case DBUNITS_DISTANCE:
		returnValue = DISTANCEUNITS_Size - 1;
		break;
	case DBUNITS_PERCENT:
		returnValue = PERCENTUNITS_Size - 1;
		break;
	case DBUNITS_FLOW_RATE:
		returnValue = FLOWRATEUNITS_Size - 1;
		break;
	case DBUNITS_TIME:
		returnValue = TIMEUNITS_Size - 1;
		break;
	case DBUNITS_MASS:
		returnValue = MASSUNITS_Size - 1;
		break;
	case DBUNITS_RPM:
		returnValue = RPMUNITS_Size - 1;
		break;
	case DBUNITS_SPEED:
		returnValue = SPEEDUNITS_Size - 1;
		break;
	case DBUNITS_ELECTRICAL:
		returnValue = ELECTRICALUNITS_Size - 1;
		break;
	case DBUNITS_ECONOMY:
		returnValue = ECONOMYUNITS_Size - 1;
		break;
	case DBUNITS_COMMAND:
		returnValue = COMMANDUNITS_Size - 1;
		break;
	case DBUNITS_DEPTH:
		returnValue = DEPTHUNITS_Size - 1;
		break;
	case DBUNITS_BEARING:
		returnValue = BEARINGUNITS_Size - 1;
		break;
	case DBUNITS_ANGLE:
		returnValue = ANGLEUNITS_Size - 1;
		break;
	case DBUNITS_CURRENT:
		returnValue = CURRENTUNITS_Size - 1;
		break;
	case DBUNITS_FREQUENCY:
		returnValue = FREQUENCYUNITS_Size - 1;
		break;
	case DBUNITS_RESISTANCE:
	case DBUNITS_NONE:
	case DBUNITS_REVS:
		returnValue = 0;
		break;
		//////////////////////////////////////////////////////////////////////////
		// Both these are not valid.
	case DBUNITS_END:
	default:
		assert("Bad Unit Type" && FALSE);
		break;
	}

	return returnValue;
}