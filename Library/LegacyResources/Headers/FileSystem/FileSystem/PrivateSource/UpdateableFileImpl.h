//==============================================================================================
//	FILENAME	:	UpdateableFileImpl.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-04
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_UpdateableFileImpl_h
#define MFileSystem_UpdateableFileImpl_h

#include "Base/Base/String.h"

#include "../UpdateableFile.h"
#include "FileImpl.h"

#ifndef WIN32
#include "InprocessFileLocker.h"
#endif // WIN32

namespace MFileSystem
{
	class UpdateableFileImpl:
		virtual public UpdateableFile
	{
	public:

		UpdateableFileImpl(
			const char* ipPath,
			IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED iWhatToDoIfCommitAndCloseWasNotCalled);

		virtual ~UpdateableFileImpl() throw();

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
		bool mReadLocked;
		bool mWriteLocked;

		MBase::TotalTimer mReadTimer;
		MBase::TotalTimer mReadLockTimer;

		MBase::TotalTimer mWriteTimer;
		MBase::TotalTimer mWriteLockTimer;

#ifndef WIN32
		InprocessFileLocker::LockPtr mInprocessFileLockerPtr;
#endif // WIN32
	};
}

#endif // MFileSystem_UpdateableFileImpl_h
