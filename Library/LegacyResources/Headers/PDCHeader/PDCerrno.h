//==============================================================================================
//	FILENAME	:	PDCerrno.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCerrno_h
#define PDCerrno_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <errno.h>

/**
 * Defines a macro of parameter validation for security
 * enhanced string and memory related functions.
 * @author Qiang Xue
 */
#ifndef _VALIDATE_RETURN_ERRCODE
#define _VALIDATE_RETURN_ERRCODE( expr, errorcode ) \
{                                                   \
	_ASSERTE( ( expr ) );                           \
	if ( !( expr ) )                                \
	{                                               \
		errno = errorcode;                          \
		return ( errorcode );                       \
	}                                               \
}
#endif

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCerrno_h
