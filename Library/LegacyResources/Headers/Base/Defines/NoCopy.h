//==============================================================================================
//	FILENAME	:	NoCopy.h
//	AUTHOR		:	Andres Murillo
//	CREATION	:	9/26/1999
//	Copyright (C) MicroStrategy Incorporated 1999
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_NoCopy_h
#define MBase_NoCopy_h

#include "../Base/Base.h"

namespace MBase 
{
	/**
	 * Inherit from this class disable the copy constructor and the 
	 * assignment operaton in your class. You may need to do this because
	 * the compiler will provide default implementations of this
	 * methods and this tends to be dangerous.
	 */
	class DLL_BASE_EXIM NoCopy
	{
		protected:
			// Only derived classes can call the constructor
			NoCopy()
			{
			}

		private:	
			// Declare copy constructor and asignment operator as private
			NoCopy(const NoCopy&);
			const NoCopy& operator=(const NoCopy&);
	};
}

#endif // MBase_NoCopy_h
