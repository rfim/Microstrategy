//==============================================================================================
//	FILENAME	:	Asserte.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	10/19/2001
//  Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Asserte_h
#define MBase_Asserte_h

// Sanity check:
//
// Diagnostic code frequently relies upon either
// _DEBUG or NDEBUG symbols defined -- in order
// to distinguish Debug/Release configurations.
//
// These symbols should be defined in the imake
// configuration files. Here, we only check that
// we didn't forget to define the proper symbol.
//
#if defined( _DEBUG ) && defined( NDEBUG )
    #error Cannot have both _DEBUG and NDEBUG defined.
#endif

#if !defined( _DEBUG ) && !defined( NDEBUG )
    #error Please, update imake configuration to define either _DEBUG or NDEBUG.
#endif

#ifdef WIN32
	#include <crtdbg.h>
#else
	#include <assert.h>

	#define _ASSERT(x)	_ASSERTE(x)

	#ifdef NDEBUG
		#define _ASSERTE(x)	((void)0)
	#else
		//#define _ASSERTE(x)	((void)((x) || (MBase::InteractiveAssert(#x, __FILE__, __LINE__), false)))
		#define _ASSERTE(x) assert(x)
	#endif


#endif // WIN32

#endif // MBase_Asserte_h
