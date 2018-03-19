//==============================================================================================
//	FILENAME	:	Atomic.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-22
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Atomic_h
#define MBase_Atomic_h

#include "Base.h"
#include "Synch/Synch/PrivateSource/AtomicLongData.h"


namespace MBase
{
	/**
	 * Atomically assigns iNewValue and returns a previous value.
	 */
	inline long AtomicExchange(long& irVariable, long iNewValue)
	{
		MSynch::AtomicLongData newData(irVariable);
		return newData.Exchange(iNewValue);
	}
}

#endif // MBase_Atomic_h
