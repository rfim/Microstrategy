//==============================================================================================
//	FILENAME	:	PDCcrtdbg.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCcrtdbg_h
#define PDCcrtdbg_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#ifdef WIN32
#include <crtdbg.h>
#else

#define _CRTDBG_ALLOC_MEM_DF        0x01  /* Turn on debug allocation */
#define _CRTDBG_DELAY_FREE_MEM_DF   0x02  /* Don't actually free memory */
#define _CRTDBG_CHECK_ALWAYS_DF     0x04  /* Check heap every alloc/dealloc */
#define _CRTDBG_RESERVED_DF         0x08  /* Reserved - do not use */
#define _CRTDBG_CHECK_CRT_DF        0x10  /* Leak check/diff CRT blocks */
#define _CRTDBG_LEAK_CHECK_DF       0x20  /* Leak check at program exit */

#define _CRTDBG_REPORT_FLAG         -1    /* Query bitflag status */

#endif // WIN32

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCcrtdbg_h
