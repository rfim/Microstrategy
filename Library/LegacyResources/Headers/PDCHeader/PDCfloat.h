//==============================================================================================
//	FILENAME	:	PDCfloat.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCfloat_h
#define PDCfloat_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <float.h>

#ifdef WIN32
inline int isnan(double dsrc)
{
	return ::_isnan(dsrc);
}

inline int finite(double dsrc)
{
	return ::_finite(dsrc);
}
#endif // WIN32

#if defined(sun)
#include <ieeefp.h>
#endif // sun

// 2004-08-27 vovechkin
// TODO: _finite in Kernel/SourceCode/MsiGraph/PrivateSource/GraphGenImpl.cpp
#ifndef WIN32

#include <math.h>	// yluo: this is needed for finite().

#define _finite finite

#ifdef __hpux
	#define finite isfinite
#endif

#endif

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCfloat_h
