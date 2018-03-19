//==============================================================================================
//	FILENAME	:	Base/Defines/BasicTypes.h
//  PROJECT		:	Base
//
//	AUTHOR		:	Will Hurwood
//	CREATION	:	2002-09-27
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

// This file contains definitions for the basic type macros
// These macros are used to ensure that we can reliably specify a 32 bit
// and a 64 bit integer accross many platforms
#ifndef MBase_BasicTypes_h
#define MBase_BasicTypes_h

// This file is include in both IDL and C++ projects
// So it should not contain anything other than macro definitions

// Int32 is a 32 bit integer - 'int' on all platforms
#define Int32		int

// Int64 is a 64 bit integer - 'long' or '__int64'
//INT64 is for 64 bit integer literals (UNIX and Windows have different suffixes)
//Use INT64(<your 64 bit integer literal>).
#ifdef WIN32
	#define Int64		__int64
	#define INT64(x)	x##i64
	// 8/3/2005(qxue): added definition of UINT64 (required by <sqltypes.h>)
	typedef unsigned __int64  UINT64;
	#define UInt64 UINT64
#elif defined(_MAC)
#import <MacTypes.h>
#define Int64 long long
#define INT64(x) x##LL
#else
	#define Int64		long long
	#define INT64(x)	x##LL
	#define UInt64		unsigned long long
#endif

#ifdef __cplusplus
// 2003-11-21 vovechkin
// ensure that the IntXX type sizes are correct
#include "StaticCheck.h"
STATIC_CHECK(sizeof(Int32) == 4);
STATIC_CHECK(sizeof(Int64) == 8);
STATIC_CHECK(sizeof(INT64(0)) == 8);
#endif // __cplusplus

#endif  // MBase_BasicTypes_h
