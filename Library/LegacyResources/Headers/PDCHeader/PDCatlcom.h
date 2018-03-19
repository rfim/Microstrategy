//==============================================================================================
//	FILENAME	:	PDCatlcom.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCatlcom_h
#define PDCatlcom_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	//#include <atlcom.h>
	#include "ATL/DssAtlcom.h"
#else
	#include "MsiCOM/MsiCOM/Msi_atlcom.h"
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCatlcom_h
