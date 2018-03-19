//==============================================================================================
//	FILENAME	:	PDCstdio.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCstdio_h
#define PDCstdio_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <stdio.h>
#include <string.h>

#include "PDCwchar.h"
#include "PDCstdarg.h"

#if !defined(WIN32) && defined(WIN64)
//////////////////////////////////////////////////////////////
// 2003-11-04 vovechkin
// Implemented swprintf() with additional count parameter,
// just like on UNIX.
//
inline int swprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...)
{
	va_list argptr;

	// variable arguments start with last defined argument - format
	va_start(argptr, format);

	// build message based on format, and remaining arguments
	const int lResult = ::_vsnwprintf(buffer, count, format, argptr);

	va_end(argptr);

	return lResult;
}
#endif // WIN32

// this is a version of sprintf with extra count parameter
inline int sprintf(char* buffer, size_t count, const char* format, ...)
{
	va_list argptr;

	// variable arguments start with last defined argument - format
	va_start(argptr, format);

	// build message based on format, and remaining arguments
#ifdef WIN32
	const int lResult = ::_vsnprintf_s(buffer, count, count, format, argptr);
#else
	const int lResult = ::vsnprintf(buffer, count, format, argptr);
#endif

	va_end(argptr);

	return lResult;
}

#if !defined(WIN32) && defined(WIN64)
/**
 * Security enhanced version of sprintf and swprintf for non-Windows platform.
 * @author Qiang Xue
 * @version $Revision: $  $Date: $
 */

/**
 * Security enhanced version of sprintf.
 * 
 * Formats and stores a series of characters and values in buffer.
 * 
 * The main difference between sprintf_s and sprintf is that sprintf_s checks 
 * the format string for valid formatting characters, whereas sprintf only checks 
 * if the format string or buffer are NULL pointers. If either check fails, 
 * an assertion will be raised when _DEBUG is set. If execution is allowed to continue,
 * the function returns -1 and sets errno to EINVAL.
 *
 * @param char* Storage location for output
 * @param size_t Maximum number of characters to store
 * @param const char* Format-control string
 * @return int number of characters written, -1 if any error occurs.
 */
inline int sprintf_s(char *buffer, size_t sizeOfBuffer, const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format); // variable arguments start with last defined argument - format
	const int lResult = ::vsnprintf(buffer, sizeOfBuffer, format, argptr);
	va_end(argptr);

	return lResult;
}

/**
 * Security enhanced version of swprintf.
 * 
 * Formats and stores a series of characters and values in buffer.
 * 
 * swprintf_s is a wide-character version of sprintf_s.
 *
 * @param wchar_t* Storage location for output
 * @param size_t Maximum number of characters to store
 * @param const wchar_t* Format-control string
 * @return int number of characters written, -1 if any error occurs.
 */
inline int swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t* format, ...)
{
	va_list argptr;
	va_start(argptr, format); // variable arguments start with last defined argument - format
	const int lResult = ::_vsnwprintf(buffer, sizeOfBuffer, format, argptr);
	va_end(argptr);

	return lResult;
}

#endif

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCstdio_h
