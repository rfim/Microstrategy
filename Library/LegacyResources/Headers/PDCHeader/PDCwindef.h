//==============================================================================================
//	FILENAME	:	PDCwindef.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwindef_h
#define PDCwindef_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <windef.h>
#else
	//==============================================================================================
	//	FILENAME	:	windef.h
	//	AUTHOR		:	Yuling Ma
	//	CREATION	:	8/15/02
	//	Copyright (C) MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	#ifndef __WINDEF_H__
	#define __WINDEF_H__

	#include "PDCwtypes.h"

	typedef struct tagRECT
	{
		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	} RECT;

	typedef struct tagPOINT
	{
		LONG  x;
		LONG  y;
	} POINT, *PPOINT;

	typedef struct tagPOINT __RPC_FAR *LPPOINT;
#ifdef WIN32		//kxiao add for security subsytem  on MSICOM Windows
typedef DWORD near          *PDWORD;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
DECLARE_HANDLE(HKEY);
typedef HKEY *PHKEY;



/* Types use for passing & returning polymorphic values */
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;


#endif //win32


	#endif /* __WINDEF_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwindef_h
