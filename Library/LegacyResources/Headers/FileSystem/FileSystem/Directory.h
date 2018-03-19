//==============================================================================================
//	FILENAME	:	Directory.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-09-19
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_Directory_h
#define MFileSystem_Directory_h

#include "FileSystem.h"

namespace MFileSystem
{
	/**
	 * Directory contains functions operating upon a directory.
	 */
	class DLL_FILE_SYSTEM_EXIM Directory
	{
	public:
		/**
		 * Creates a new directory.
		 *
		 * @throw MFileSystem::Exception()
		 */
		static void Create( const char *ipPath, bool iCreateParentDirectoriesIfNecessary = true );

		/**
		 * Deletes an empty directory.
		 *
		 * Note: this method does not throw MBase::Message, but may throw other low-level exceptions.
		 *
		 * @return true if succeeded, false if failed
		 */
		static bool DeleteEmpty(const char* ipPath);

		/**
		 * Deletes a directory recursively -- BE CAREFUL!!!
		 *
		 * Note: this method does not throw MBase::Message, but may throw other low-level exceptions.
		 *
		 * @return true if succeeded, false if failed or partially succeeded
		 */
		static bool DeleteRecursively(const char* ipPath);
	};
}

#endif // MFileSystem_Directory_h
