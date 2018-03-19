//==============================================================================================
//	FILENAME	:	Initializer.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-12-01
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_Initializer_h
#define MFileSystem_Initializer_h

#include "FileSystem.h"

namespace MFileSystem
{
	/**
	 * Instantiate this class to initialize the FileSystem library.
	 */
	class DLL_FILE_SYSTEM_EXIM Initializer
	{
		public:
			Initializer();
			~Initializer() throw();
	};
}

#endif // MFileSystem_Initializer_h
