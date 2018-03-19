//==============================================================================================
//	FILENAME	:	WriteableFileImpl.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-09-28
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_WriteableFileImpl_h
#define MFileSystem_WriteableFileImpl_h


#include "../WriteableFile.h"
#include "FileImpl.h"


#ifndef WIN32
#include "InprocessFileLocker.h"
#endif // WIN32

namespace MFileSystem
{
	class WriteableFileImpl:
		virtual public WriteableFile
	{
	public:

		WriteableFileImpl(
			const char* ipPath,
			IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED iWhatToDoIfCommitAndCloseWasNotCalled);

		virtual ~WriteableFileImpl() throw();

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

		/// @see MFileSystem::WriteableFile::Write
		virtual void Write(const char* ipString, size_t iLength);

		/// @see MFileSystem::WriteableFile::Write
		virtual void Write(const void* ipData, size_t iByteLength);

		/// @see MFileSystem::WriteableFile::WriteLittleEndian
		virtual void WriteLittleEndian(const unsigned Int32& irValue);

		/// @see MFileSystem::WriteableFile::Flush
		virtual void Flush();

		/// @see MFileSystem::WriteableFile::Truncate
		virtual void Truncate();

		/// @see MFileSystem::WriteableFile::CommitAndClose
		virtual void CommitAndClose();

		/// @see MFileSystem::WriteableFile::GetWriteTimer
		virtual const MBase::TotalTimer& GetWriteTimer() const;

		/// @see MFileSystem::WriteableFile::GetWriteLockTimer
		virtual const MBase::TotalTimer& GetWriteLockTimer() const;

		/// @see MFileSystem::WriteableFile::WriteLock
		virtual bool WriteLock(bool iNoWait);
		
		/// @see MFileSystem::WriteableFile::WriteUnlock
		virtual void WriteUnlock() throw();

	private:

		const IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED mWhatToDoIfCommitAndCloseWasNotCalled;

		const std::string mPath;

		MFileSystem::FileHandle mFileHandle;

		// flags
		bool mPendingFlush;
		bool mPositionUndefined;
		bool mWriteLocked;

		MBase::TotalTimer mWriteTimer;
		MBase::TotalTimer mWriteLockTimer;

#ifndef WIN32
		InprocessFileLocker::LockPtr mInprocessFileLockerPtr;
#endif // WIN32
	};
}

#endif // MFileSystem_WriteableFileImpl_h
