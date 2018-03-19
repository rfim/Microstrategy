//==============================================================================================
//	FILENAME	:	PDCatlbase.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCatlbase_h
#define PDCatlbase_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	//#include <atlbase.h>
	#include "ATL/DssAtlbase.h"
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	#include "MsiCOM/MsiCOM/Msi_atlbase.h"
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCatlbase_h
