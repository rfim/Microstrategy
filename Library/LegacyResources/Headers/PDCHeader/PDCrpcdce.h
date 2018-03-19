//==============================================================================================
//	FILENAME	:	PDCrpcdce.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCrpcdce_h
#define PDCrpcdce_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <rpcdce.h>
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	//==============================================================================================
	//	FILENAME	:	rpcdce.h
	//	AUTHOR		:	Liqun Jin
	//	CREATION	:	8/27/2001
	//	Copyright (C) 2001 MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	// This header file stands in for the standard 'rpcdce.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	#ifndef __RPCDCE_H__
	#define __RPCDCE_H__

	#ifndef GUID_DEFINED
	#define GUID_DEFINED
	typedef struct _GUID
	{
		unsigned int Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[8];
	} GUID;
	#endif // GUID_DEFINED 

	#ifndef UUID_DEFINED
	#define UUID_DEFINED
	typedef GUID UUID;
	#ifndef uuid_t
	#define uuid_t UUID
	#endif
	#endif

	#ifdef WIN32
	#define IN
	#define OUT
	#define OPTIONAL
	#endif

	#endif // __RPCDCE_H__
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCrpcdce_h
