//==============================================================================================
//	FILENAME	:	ReadableFileImpl.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-09-28
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_ReadableFileImpl_h
#define MFileSystem_ReadableFileImpl_h

#include "Base/Base/String.h"

#include "../ReadableFile.h"
#include "FileImpl.h"

#ifndef WIN32
#include "InprocessFileLocker.h"
#endif // WIN32

namespace MFileSystem
{
	class ReadableFileImpl:
		virtual public ReadableFile
	{
	public:

		ReadableFileImpl(const char* ipPath, bool bInProcessLock_NoWait);

		virtual ~ReadableFileImpl() throw();

	protected:

		/// @see MFileSystem::File::GetPath
		virtual const char* GetPath() const;

		/// @see MFileSystem::File::IsAccessibleViaPath
		virtual bool IsAccessibleViaPath() const;

		/// @see MFileSystem::File::GetSize
		virtual Int64 GetSize() const;

		/// @see MFileSystem::File::GetPosition
		virtual Int64 GetPosition() const;

		/// @see MFileSystem::File::MovePosition
		virtual void MovePosition(Int64 iOffset);

		/// @see MFileSystem::File::SetPosition
		virtual void SetPosition(Int64 iOffset);

		/// @see MFileSystem::File::SetPositionToEnd
		virtual Int64 SetPositionToEnd();

	protected:

		/// @see MFileSystem::ReadableFile::Read
		virtual void Read(void* ipBuffer, size_t iBufferSize);

		/// @see MFileSystem::ReadableFile::Read
		virtual void Read(MDataType::Binary& irBinary, size_t iMaxSize);

		/// @see MFileSystem::ReadableFile::ReadLittleEndian
		virtual void ReadLittleEndian(unsigned Int32& orValue);

		/// @see MFileSystem::ReadableFile::ReadLine
		virtual void ReadLine(MDataType::Binary& irBinary);
		
		/// @see MFileSystem::ReadableFile::GetReadTimer
		virtual const MBase::TotalTimer& GetReadTimer() const;

		/// @see MFileSystem::ReadableFile::GetReadLockTimer
		virtual const MBase::TotalTimer& GetReadLockTimer() const;

		/// @see MFileSystem::ReadableFile::ReadLock
		virtual bool ReadLock(bool iNoWait);
		
		/// @see MFileSystem::ReadableFile::ReadUnlock
		virtual void ReadUnlock() throw();

	private:

		const std::string mPath;

		MFileSystem::FileHandle mFileHandle;

		// flags
		bool mPositionUndefined;
		bool mReadLocked;

		MBase::TotalTimer mReadTimer;
		MBase::TotalTimer mReadLockTimer;

#ifndef WIN32
		InprocessFileLocker::LockPtr mInprocessFileLockerPtr;
#endif // WIN32
	};
}

#endif // MFileSystem_ReadableFileImpl_h
