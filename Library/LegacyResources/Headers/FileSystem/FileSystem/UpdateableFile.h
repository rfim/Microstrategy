//==============================================================================================
//	FILENAME	:	UpdateableFile.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-04
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_UpdateableFile_h
#define MFileSystem_UpdateableFile_h

#include "ReadableFile.h"
#include "WriteableFile.h"

namespace MFileSystem
{
	/**
	 *
	 */
	class UpdateableFile:
		virtual public ReadableFile,
		virtual public WriteableFile
	{
	public:

		typedef MBase::StrongPtr<UpdateableFile> Ptr;
		typedef MBase::ReturnPtr<UpdateableFile> ReturnPtr;

		// virtual destructor is a must
		virtual ~UpdateableFile() throw()
		{
		}

		typedef ReadableFile::ReadSmartLock ReadSmartLock;
		typedef ReadableFile::ReadSmartLock_NoWait ReadSmartLock_NoWait;

		typedef WriteableFile::WriteSmartLock WriteSmartLock;
		typedef WriteableFile::WriteSmartLock_NoWait WriteSmartLock_NoWait;
	};
	
	DLL_FILE_SYSTEM_EXIM UpdateableFile::ReturnPtr OpenUpdateableFile(
		const char* ipPath,
		IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED iWhatToDoIfCommitAndCloseWasNotCalled);
}

#endif // MFileSystem_UpdateableFile_h
