//==============================================================================================
//	FILENAME	:	AbsoluteTimeout.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2005-01-20
//	Copyright (C) MicroStrategy Incorporated 2005
//==============================================================================================
#ifndef MSynch_AbsoluteTimeout_h
#define MSynch_AbsoluteTimeout_h

#include "Base/Defines/BasicTypes.h"
#include "PDCtime.h"
#if defined(_NO_EXCEPTION)
#include <stdio.h>
#endif

#ifndef WIN32
namespace MSynch
{
	inline void GetAbsoluteTimeout(struct timespec& oAbsoluteTimeout, unsigned Int32 iMilliseconds)
	{
		struct timeval lTimeValue;
		const int lResult = ::gettimeofday(&lTimeValue, NULL);

		if (lResult != 0)
		{
#if !defined(_NO_EXCEPTION)
			throw ("::gettimeofday failed");
#else
			printf("%s\n","::gettimeofday failed");
			return;
#endif	
		}

		lTimeValue.tv_sec += iMilliseconds / 1000;
		lTimeValue.tv_usec += (iMilliseconds % 1000) * 1000; // convert milli-seconds to micro-seconds

		if (lTimeValue.tv_usec >= 1000 * 1000)
		{
			// fix the micro-second overflow
			lTimeValue.tv_sec += 1;
			lTimeValue.tv_usec -= 1000 * 1000;
		}

		_ASSERT(lTimeValue.tv_usec < 1000 * 1000);

		oAbsoluteTimeout.tv_sec = lTimeValue.tv_sec;
		oAbsoluteTimeout.tv_nsec = lTimeValue.tv_usec * 1000; // convert micro-seconds to nano-seconds
	}
}
#endif // WIN32

#endif // MSynch_AbsoluteTimeout_h
