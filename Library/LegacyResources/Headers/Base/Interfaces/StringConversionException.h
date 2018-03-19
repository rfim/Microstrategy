//==============================================================================================
//	FILENAME	:	StringConversionException.h
//	AUTHOR		:	vovechkin
//	CREATION	:	Nov 16, 2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StringConversionException_h
#define MBase_StringConversionException_h

#include "Base/Base/Base.h"
#include "Base/Interfaces/BaseException.h"

namespace MBase
{
	/**
	 * StringConversionException is the exception that would get thrown if a string conversion
	 * fails.
	 */
	class StringConversionException:
		public BaseException 
	{
	};
} 

#endif // MBase_StringConversionException_h
