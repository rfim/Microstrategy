//==============================================================================================
//	FILENAME	:	Synch.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_Synch_h
#define MSynch_Synch_h

#ifdef WIN32
	#ifdef SYNCH_DLL_EXPORT
		#define	DLL_SYNCH_EXIM	_declspec(dllexport)
	#else
		#define	DLL_SYNCH_EXIM	_declspec(dllimport)
	#endif
#else
	#define	DLL_SYNCH_EXIM
#endif

#include "Logger.h"

namespace MSynch
{
	// SetLogger must not be called simultaneously by different threads!
	DLL_SYNCH_EXIM void SetLogger( Logger* ipLogger );
}

#endif // MSynch_Synch_h
