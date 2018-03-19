//==============================================================================================
//	FILENAME	:	PDCtchar.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCtchar_h
#define PDCtchar_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <TCHAR.H>
#else
	//==============================================================================================
	//	FILENAME	:	tchar.h
	//	AUTHOR		:	Yuling Ma
	//	CREATION	:	10/9/01
	//	Copyright (C) MicroStrategy Incorporated 2001
	//==============================================================================================

	// This header file stands in for the standard 'tchar.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// We are using this file to hold symbols and declarations that seem to be part of
	// the windows environment.

	#ifndef __TCHAR_H__
	#define __TCHAR_H__

	#include "PDCrpc.h"
	#include "PDCwtypes.h"
	#include "PDCwchar.h"
	#include "PDCstdio.h"
	#include "PDCassert.h"

#ifdef NO_WCHAR_SUPPORT
	#define __T(x) x
	typedef char     _TCHAR;  
	#define _itot      
	#define _ui64tot    
	
	#define _tcscmp     
#else
	#define __T(x)		L ## x
	typedef wchar_t     _TCHAR;
	#define _itot       _itow
	
	#define _ui64tot    _ui64tow
	
	#define _tcscmp     wcscmp
	
		// BEWARE: need to implement the following function
	inline wchar_t* _itow( Int32 value, wchar_t *string, Int32 radix )
	{
		// not full imp yet
		if (radix==10) {
			swprintf(string, 1000, L"%d", value);
			return string;
		} else if (radix==16) {
			swprintf(string, 1000, L"%x", value);
			return string;
		} else {
			assert(false);
		}
		return 0;
	}

	inline wchar_t* _ltow( long iVal, wchar_t* iStr, int iRadix )
	{
		// now only support 10
		assert(iRadix == 10);

		int lInt = (int)iVal;
		swprintf(iStr, 1000, L"%d", lInt);
		return iStr;
	}

	inline long _wtol( const wchar_t *string )
	{
		long lLongValue = 0;
		if(string == NULL) 
			return lLongValue;

		wchar_t* lpEnd = NULL;
		lLongValue = wcstol(string, &lpEnd, 10);
		return lLongValue;
	}
	
#endif

	#define _T(x)		__T(x)
	#define _TEXT(x)	__T(x)
	#define __TEXT(quote) L##quote 
	#define TEXT(quote) __TEXT(quote)   

	#ifdef WIN32
	#ifdef  _UNICODE

	#define _fgettc         fgetwc
	#define _fgettc_nolock  _fgetwc_nolock
	#define _fgettchar      _fgetwchar
	#define _fgetts         fgetws
	#define _fputtc         fputwc
	#define _fputtc_nolock  _fputwc_nolock
	#define _fputtchar      _fputwchar
	#define _fputts         fputws
	#define _cputts         _cputws
	#define _cgetts         _cgetws
	#define _cgetts_s       _cgetws_s
	#define _gettc          getwc
	#define _gettc_nolock   _getwc_nolock
	#define _gettch         _getwch
	#define _gettch_nolock  _getwch_nolock
	#define _gettche        _getwche
	#define _gettche_nolock _getwche_nolock
	#define _gettchar       getwchar
	#define _gettchar_nolock _getwchar_nolock
	#define _getts          _getws
	#define _getts_s        _getws_s
	#define _puttc          putwc
	#define _puttc_nolock   _putwc_nolock
	#define _puttchar       putwchar
	#define _puttchar_nolock _putwchar_nolock
	#define _puttch         _putwch
	#define _puttch_nolock  _putwch_nolock
	#define _putts          _putws
	#define _ungettc        ungetwc
	#define _ungettc_nolock _ungetwc_nolock
	#define _ungettch       _ungetwch
	#define _ungettch_nolock _ungetwch_nolock

	#else // !_UNICODE

	#define _fgettc         fgetc
	#define _fgettc_nolock  _fgetc_nolock
	#define _fgettchar      _fgetchar
	#define _fgetts         fgets
	#define _fputtc         fputc
	#define _fputtc_nolock  _fputc_nolock
	#define _fputtchar      _fputchar
	#define _fputts         fputs
	#define _cputts         _cputs
	#define _gettc          getc
	#define _gettc_nolock   _getc_nolock
	#define _gettch         _getch
	#define _gettch_nolock  _getch_nolock
	#define _gettche        _getche
	#define _gettche_nolock _getche_nolock
	#define _gettchar       getchar
	#define _gettchar_nolock _getchar_nolock
	#define _getts          gets
	#define _getts_s        gets_s
	#define _cgetts         _cgets
	#define _cgetts_s       _cgets_s
	#define _puttc          putc
	#define _puttc_nolock   _putc_nolock
	#define _puttchar       putchar
	#define _puttchar_nolock _putchar_nolock
	#define _puttch         _putch
	#define _puttch_nolock  _putch_nolock
	#define _putts          puts
	#define _ungettc        ungetc
	#define _ungettc_nolock _ungetc_nolock
	#define _ungettch       _ungetch
	#define _ungettch_nolock _ungetch_nolock

	#endif // _UNICODE
	#endif

	#endif /* __TCHAR_H__ */
#endif // WIN32 && !_USE_MSI_COM


// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCtchar_h
