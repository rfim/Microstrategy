//==============================================================================================
//	FILENAME	:	Initializer.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-09-30
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================
#ifndef MSynch_Initializer_h
#define MSynch_Initializer_h

#include "Synch.h"

namespace MSynch
{
	/**
	 * Instantiate this class to initialize the Synch library.
	 */
	class DLL_SYNCH_EXIM Initializer
	{
		public:
			Initializer();
			~Initializer() throw();
	};
}

#endif // MSynch_Initializer_h
