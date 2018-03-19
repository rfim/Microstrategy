//==============================================================================================
//	FILENAME	:	FileSystem.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-08-27
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_FileSystem_h
#define MFileSystem_FileSystem_h

#define	DLL_FILE_SYSTEM_EXIM

#include "Base/Base/ReturnString.h"

/**
 * FileSystem is a constellation that puts together functions that manipulate
 * directories and files. It is not intended to replace or be used instead
 * of the standard stream libraries and such. But rather, it is intended to 
 * cover gaping holes in non-standard or OS-dependent functionality.
 */
namespace MFileSystem
{
	/**
	 * Returns the current working directory.
	 * Throws an exception if current working directory cannot be retrieved.
	 *
	 * @return absolute path.
	 *
	 * @throw MBase::FileSystemException()
	 */
	DLL_FILE_SYSTEM_EXIM MBase::ReturnString GetCurrentWorkingDirectory();
	
	/**
	 * Sets the current working directory.
	 *
	 * @param ipPath an absolute path.
	 *
	 * @throw MBase::FileSystemException()
	 */
	DLL_FILE_SYSTEM_EXIM void SetCurrentWorkingDirectory( const char* ipPath );
	
	/**
	 * Returns the path for temporary files.
	 */
	DLL_FILE_SYSTEM_EXIM MBase::ReturnString GetTemporaryPath();

	/**
	 * Creates a temporary file and returns its absolute path.
	 * It is caller's responsibility to remove the file when it's no longer needed.
	 */

	//DLL_FILE_SYSTEM_EXIM MBase::ReturnString CreateTemporaryFile( const char* ipFilenamePrefix );
}

#endif // MFileSystem_FileSystem_h
