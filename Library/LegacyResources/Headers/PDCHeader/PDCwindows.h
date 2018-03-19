//==============================================================================================
//	FILENAME	:	PDCwindows.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwindows_h
#define PDCwindows_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#ifndef STRICT
	#define STRICT // be type-safe
	#endif

//	#ifndef _WIN32_WINNT
//	#define _WIN32_WINNT 0x0500 // for Windows 2000+ APIs
//	#endif

	#include "ProtectedSource/DisableRareWin32API.h"
	#include <windows.h>
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	//==============================================================================================
	//	FILENAME	:	windows.h
	//	AUTHOR		:	Will Hurwood
	//	CREATION	:	7/10/2001
	//	Copyright (C) MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	// This header file stands in for the standard 'windows.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// We are using this file to hold symbols and declarations that seem to be part of
	// the windows environment.
	#ifndef __WINDOWS_H__
	#define __WINDOWS_H__

	#include "PDCrpc.h"
	#include "PDCwtypes.h"

	// Hash define away some VC++ / 16-bit Windows extensions
	#ifndef WIN32
	#define __stdcall
	#define _declspec(dllexport)
	#define __cdecl
	#endif

	#define WINAPI __stdcall
	#define APIENTRY        WINAPI
	
	#include "PDCwindef.h"
	#include "PDCwinnt.h"
	#include "PDCwinbase.h"

	// Enumeration constants used in DLL Main functions
	#define DLL_PROCESS_ATTACH 1
	#define DLL_PROCESS_DETACH 0

	// 2002-12-05 vovechkin
	// these are not supported on UNIX, so they should be used
	// on neither Windows nor UNIX to prevent subtle bugs
	//#define DLL_THREAD_ATTACH  2
	//#define DLL_THREAD_DETACH  3

	// API call made by thread on DLL startup
	// Tells WinAPI to stop informing DLL each time a thread attaches to it
	inline BOOL DisableThreadLibraryCalls(HMODULE /*hModule*/)
	{
		return 1;
	}

	/////////////////////////////////////
	// Dec 3, 2001 vovechkin:
	// Use MBase::GetLastError() instead, defined in Base/Base/LastError.h
	//DWORD GetLastError(void);

	void SetLastError(DWORD dwErrCode);

	// Memory Management APIs, which is in winbase.h
	#include "PDCstdlib.h"

	#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
	#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

	#ifndef THREAD_PRIORITY_NORMAL
	#define THREAD_PRIORITY_NORMAL          0
	#endif

	#endif /* __WINDOWS_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwindows_h
