//==============================================================================================
//	FILENAME	:	PDCtime.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCtime_h
#define PDCtime_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#ifdef WIN32
	#include <time.h>
#else
	#include <time.h>
	#include <sys/time.h>
#endif // WIN32

#ifndef WIN32
#include "PDCerrno.h"

namespace MPDCHeader
{
	// a signal-safe wrapper
	inline int nanosleep(const struct timespec* rqtp)
	{
		struct timespec lSleepTime = *rqtp;
		struct timespec lRemainder;

		for (;;)
		{
			const int lResult = ::nanosleep(&lSleepTime, &lRemainder);

			if (lResult == -1)
			{
				if (errno == EINTR)
				{
					// interrupted by signal, retry
					lSleepTime = lRemainder;
					continue;
				}

				// error
				return -1;
			}

			return lResult;
		}
	}
}

#endif // WIN32

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCtime_h
