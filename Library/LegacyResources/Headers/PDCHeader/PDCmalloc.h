//==============================================================================================
//	FILENAME	:	PDCmalloc.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCmalloc_h
#define PDCmalloc_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <stdlib.h>

#ifndef WIN32
#ifndef __MANDRELL__
#include <alloca.h>
#define _alloca(x) alloca(x)
#endif
#endif

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCmalloc_h
