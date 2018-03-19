//==============================================================================================
//	FILENAME	:	WriteableFile.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-09-28
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_WriteableFile_h
#define MFileSystem_WriteableFile_h

#include "Base/Base/Timer.h"

#include "FileSystem.h" // for FILE_SYSTEM_DLL_EXPORT

#include "GenericFile.h"

namespace MFileSystem
{
	enum IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED
	{
		/**
		 * NOTE: You must call CommitAndClose after the changes to the file.
		 */
		IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED_THEN_TRUNCATE_FILE,
		IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED_THEN_DELETE_FILE,
		IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED_THEN_PRESERVE_FILE
	};

	/**
	 *
	 */
	class WriteableFile:
		virtual public GenericFile
	{
	public:

		typedef MBase::StrongPtr<WriteableFile> Ptr;
		typedef MBase::ReturnPtr<WriteableFile> ReturnPtr;

		// virtual destructor is a must
		virtual ~WriteableFile() throw()
		{
		}

		/**
		 * Writes a null-terminated character string into a file stream.
		 * If iLength is 0, the function writes ::strlen() characters,
		 * otherwise, it uses the provided value of iLength.
		 *
		 * NOTE: it is required on most platforms to "flush" a buffered stream after
		 * writing and before reading/seeking. Otherwise, either SetPosition will fail,
		 * or the resulting file will be corrupt.
		 *
		 * @see MFileSystem::WriteableFile::Flush
		 * @see MFileSystem::GenericFile::SetPosition
		 */
		virtual void Write(const char* ipString, size_t iLength = 0) = 0;

		/**
		 * Writes raw data into a file stream.
		 */
		virtual void Write(const void* ipData, size_t iByteLength) = 0;

		/**
		 * Read a value stored in file as little-endian.
		 */
		virtual void WriteLittleEndian(const unsigned Int32& irValue) = 0;

		/**
		 * Flushes pending data to disk.
		 */
		virtual void Flush() = 0;

		/**
		 * Truncates a file after the current position (in other words, 
		 * changes file's size up to the current position).
		 */
		virtual void Truncate() = 0;

		/**
		 * Commit the changes and close the file.
		 *
		 * MUST BE CALLED as last operation on the file, before the object is released.
		 */
		virtual void CommitAndClose() = 0;

		/**
		 * Return the timer for writing operations: Write, Flush, Truncate, CommitAndClose.
		 */
		virtual const MBase::TotalTimer& GetWriteTimer() const = 0;

		/**
		 * Return the timer for write-lock operations.
		 */
		virtual const MBase::TotalTimer& GetWriteLockTimer() const = 0;

		/**
		 * Smart lock that waits forever.
		 */
		class DLL_FILE_SYSTEM_EXIM WriteSmartLock
		{
		public:
			WriteSmartLock(WriteableFile& irWriteableFile);
			~WriteSmartLock() throw();

			void Unlock() throw();

		protected:

			WriteSmartLock();

			WriteableFile* mpWriteableFile;
			bool mIsLocked;
		};

		/**
		 * Smart lock that does not wait for the lock.
		 *
		 * Example:
		 *
		 * MFileSystem::WriteableFile::WriteSmartLock_NoWait lFileLock(*lFilePtr);
		 *
		 * if (lFileLock.IsLocked())
		 * {
		 * 		// the file is locked -- do something with the file
		 * }
		 * else
		 * {
		 *		// the file is NOT locked -- do not access the file
		 * }
		 */
		class DLL_FILE_SYSTEM_EXIM WriteSmartLock_NoWait:
			public WriteSmartLock
		{
		public:

			WriteSmartLock_NoWait(WriteableFile& irWriteableFile);

			bool IsLocked() const throw()
			{
				return mIsLocked;
			}
		};

	protected:

		friend class WriteSmartLock;
		friend class WriteSmartLock_NoWait;

		virtual bool WriteLock(bool iNoWait) = 0;
		virtual void WriteUnlock() throw() = 0;
	};

	/**
	 * Opens (or creates if necessary) a file and returns the WriteableFile object.
	 *
	 * @throw MBase::BaseException if file cannot be opened/created (access denied, etc.)
	 */
	DLL_FILE_SYSTEM_EXIM WriteableFile::ReturnPtr OpenWriteableFile(
		const char* ipPath,
		IF_COMMIT_AND_CLOSE_WAS_NOT_CALLED iWhatToDoIfCommitAndCloseWasNotCalled);
}

#endif // MFileSystem_WriteableFile_h
