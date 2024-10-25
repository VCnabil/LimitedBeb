//------------------------------------------------------------------------------
//  TITLE :-  systemoptions.h
//  AUTHOR :  Colby Scarbrough
//  DATE :	  October 22 2024
//  DESCRIPTION :  System  options declarations
//
//------------------------------------------------------------------------------

#ifndef SYSTEMOPTIONS_H
#define SYSTEMOPTIONS_H

typedef struct
{
    const char*     name;
    const char*     on;
    const char*     off;
    bool            status;
} SYSTEMOPTIONS_DB;

extern SYSTEMOPTIONS_DB m_SYSTEMOPTIONS_DB[];

extern const int system_options_count;
#endif // SYSTEMOPTIONS_H