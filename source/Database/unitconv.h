//------------------------------------------------------------------------------
//  TITLE :-  unitconv.h
//  AUTHOR :         
//  DATE :           
//  DESCRIPTION :  Database declarations
//
//------------------------------------------------------------------------------
#ifndef UNITCONV_H
#define UNITCONV_H

#include "type.h"
#include "database.h"

#define CONV_DegC_TO_DegF(x)		((float)(x)	* (9.0f / 5.0f) + 32.0f)

#define CONV_Knots_TO_m_ph(x)		((float)(x)	* 1.15077945f)
#define CONV_Knots_TO_Km_ph(x)		((float)(x)	* 1.85200f)

#define CONV_NM_TO_m(x)				((float)(x)	* 1.1507f)
#define CONV_NM_TO_Km(x)			((float)(x)	* 1.8520f)

#define CONV_Kpa_TO_PSI(x)			((float)(x)	* 0.1450f)
#define CONV_Kpa_TO_Bar(x)			((float)(x)	* 0.01f)

#define CONV_L_ph_TO_Gal_ph(x)		((float)(x)	* 0.264172f)
#define CONV_L_ph_TO_Imp_Gal_ph(x)	((float)(x)	* 0.219969f)

#define CONV_L_TO_Gal(x)			((float)(x)	* 0.264172f)
#define CONV_L_TO_Imp_Gal(x)		((float)(x)	* 0.219969f)

#define CONV_M_TO_f(x)				((float)(x)	* 3.2808f)

#define CONV_Deg_TO_Rad(x)			((float)(x)	* 0.0175f)

#define CONV_sec_TO_hr(x)			((float)(x) / 3600.0f)

#define rounding(x) (signed long)(((float)x>=0.0) ? (float)x+0.5 : (float)x-0.5)

// Is called after "DataBase_Return_Element();" and converts the .Data into Current Units
void UnitConvert_To_Current_Units(DBELEMENT_T * p_DB_Element);

// Converts to the given units, where 0 is database units.
// need to make sure you pass in the RIGHT units for the RIGHT converstion.
// If the parameter is Pressure, make sure you pass in the PRESSURE units and not the Temperature units.
void UnitConvert_To_Desired_Units(DBELEMENT_T * p_DB_Element, uint8_t desiredUnits);

// converts from the specified units to the database units
float UnitConvertByTypeToDatabaseUnits(DBUNITS_T UnitType, uint32_t SourceUnits, float Data);

float UnitConvertGetMagneticVariationOffset(void);

const char * UnitGetStringDescriptor(DBUNITS_T UnitType);
const char * UnitGetStringCurrentUnits(DBUNITS_T UnitType);
const char * UnitGetStringSpecificUnits(DBUNITS_T UnitType, uint8_t desiredUnits);

uint8_t UnitGetMaxValid(DBUNITS_T UnitType);

//////////////////////////////////////////////////////////////////////////

#endif
