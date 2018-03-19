//==============================================================================================
//	FILENAME	:	PDCstdlib.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCstdlib_h
#define PDCstdlib_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <stdlib.h>
#include "PDCerrno.h"

#ifndef WIN32
#define _gcvt gcvt
#endif

// for Windows
#if !defined(WIN32) && defined(WIN64)

/**
 * Security enhanced version of mbstowcs.
 */
inline errno_t mbstowcs_s(size_t *pConvertedChars, wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(mbstr!=NULL,EINVAL);
	size_t len=::mbstowcs(NULL,mbstr,count);
	if(wcstr!=NULL && len>=0)
	{
		_VALIDATE_RETURN_ERRCODE(sizeInWords>len,EINVAL);
		len=::mbstowcs(wcstr,mbstr,count);
	}
	if(pConvertedChars!=NULL)
		*pConvertedChars=len;
	return 0;
}

/**
 * Security enhanced version of wcstombs.
 */
inline errno_t wcstombs_s(size_t *pConvertedChars, char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(wcstr!=NULL,EINVAL);
	size_t len=::wcstombs(NULL,wcstr,count);
	if(mbstr!=NULL && len>=0)
	{
		_VALIDATE_RETURN_ERRCODE(sizeInBytes>len,EINVAL);
		len=::wcstombs(mbstr,wcstr,count);
	}
	if(pConvertedChars!=NULL)
		*pConvertedChars=len;
	return 0;
}

/**
 * Security enhanced version of wctomb.
 */
inline errno_t wctomb_s(int *pRetValue, char *mbchar, size_t sizeInBytes, wchar_t wchar)
{
	// todo: size check
	// need to find out a function in Unix/Linux that can determine the number of
	// MB characters available in the destination buffer.
	if(pRetValue==NULL)
		::wctomb(mbchar,wchar);
	else
		*pRetValue=::wctomb(mbchar,wchar);
	return 0;
}

#endif

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCstdlib_h
