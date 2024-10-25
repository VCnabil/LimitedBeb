//------------------------------------------------------------------------------
//  TITLE :-  serialinterpolate.h
//  AUTHOR :  Colby Scarbrough
//  DATE :	  October 22 2024
//  DESCRIPTION :  Fault code declarations
//
//------------------------------------------------------------------------------
#ifndef SERIALINTERPOLATE_H
#define SERIALINTERPOLATE_H

#define FAULT_DESCRIPTION_MAX_LENGTH 50

typedef enum
{
    STA1 = 0,
    STA2,
    STA3,
    NFE,
    SFE,
    CFE,
    GAL
} FAULT_CODE;

typedef struct
{
    FAULT_CODE     FaultCode;
    int            Bit;
    char           Description[FAULT_DESCRIPTION_MAX_LENGTH];
} FAULTS_DB;

extern FAULTS_DB m_FAULTS_DB[];
#endif // SERIALINTERPOLATE_H

