//==============================================================================================
//	FILENAME	:	LockOrderViolationException.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	10/23/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_LockOrderViolationException_h
#define MSynch_LockOrderViolationException_h

#include "Base/Interfaces/BaseException.h"

namespace MSynch
{
	class LockOrderViolationException:
		public MBase::BaseException
	{
	};
}

#endif // MSynch_LockOrderViolationException_h
