//==============================================================================================
//	FILENAME	:	PDCobjidl.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCobjidl_h
#define PDCobjidl_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <objidl.h>
#else
	#include "MsiCOM/MsiCOM/Msi_objidl.h"
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCobjidl_h

