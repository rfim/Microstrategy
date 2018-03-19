//==============================================================================================
//	FILENAME	:	ReadableFile.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-09-28
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_ReadableFile_h
#define MFileSystem_ReadableFile_h

#include "Base/Base/Timer.h"

#include "FileSystem.h" // for FILE_SYSTEM_DLL_EXPORT

#include "GenericFile.h"

namespace MDataType
{
	// forward declarations
	class Binary;
}

namespace MFileSystem
{
	/**
	 *
	 */
	class ReadableFile:
		virtual public GenericFile
	{
	public:

		typedef MBase::StrongPtr<ReadableFile> Ptr;
		typedef MBase::ReturnPtr<ReadableFile> ReturnPtr;

		// virtual destructor is a must
		virtual ~ReadableFile() throw()
		{
		}

		/**
		 * Read raw data from a file stream.
		 */
		virtual void Read(void* ipBuffer, size_t iBufferSize) = 0;

		/**
		 * Read raw data from a file stream. GetSize method of irBinary returns the 
		 * number of bytes read.
		 *
		 * @param iMaxSize how many bytes to read, if 0 maximum capacity of irBinary is read.
		 */
		virtual void Read(MDataType::Binary& irBinary, size_t iMaxSize = 0) = 0;

		/**
		 * Read a value stored in file as little-endian.
		 */
		virtual void ReadLittleEndian(unsigned Int32& orValue) = 0;

		/**
		 * Read new-line delimited data from a file stream.
		 * The new-line character is preserved.
		 * If the last returned character is not '\n', then there is more data to be read.
		 */
		virtual void ReadLine(MDataType::Binary& irBinary) = 0;

		/**
		 * Return the timer for reading operations: Read, ReadLine.
		 */
		virtual const MBase::TotalTimer& GetReadTimer() const = 0;

		/**
		 * Return the timer for read-lock operations.
		 */
		virtual const MBase::TotalTimer& GetReadLockTimer() const = 0;

		/**
		 * Smart lock that waits forever.
		 */
		class DLL_FILE_SYSTEM_EXIM ReadSmartLock
		{
		public:

			ReadSmartLock(ReadableFile& irReadableFile);
			~ReadSmartLock() throw();

			void Unlock() throw();

		protected:

			ReadSmartLock();

			ReadableFile* mpReadableFile;
			bool mIsLocked;
		};

		/**
		 * Smart lock that does not wait for the lock.
		 *
		 * Example:
		 *
		 * MFileSystem::ReadableFile::ReadSmartLock_NoWait lFileLock(*lFilePtr);
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
		class DLL_FILE_SYSTEM_EXIM ReadSmartLock_NoWait:
			public ReadSmartLock
		{
		public:

			ReadSmartLock_NoWait(ReadableFile& irReadableFile);

			bool IsLocked() const throw()
			{
				return mIsLocked;
			}
		};

	protected:

		friend class ReadSmartLock;
		friend class ReadSmartLock_NoWait;

		virtual bool ReadLock(bool iNoWait) = 0;
		virtual void ReadUnlock() throw() = 0;
	};
	
	/**
	 * Opens a file and returns the ReadableFile object.
	 *
	 * @throw MBase::BaseException if file cannot be opened (file does not exist, access denied, etc.)
	 */
	DLL_FILE_SYSTEM_EXIM ReadableFile::ReturnPtr OpenReadableFile(const char* ipPath);

	/**
	 * Opens a file and returns the ReadableFile object.
	 *
	 * @throw MBase::BaseException if file cannot be opened (file does not exist or is already locked by writing)
	 */
	DLL_FILE_SYSTEM_EXIM ReadableFile::ReturnPtr OpenReadableFile_InProcessLockNoWait(const char* ipPath);
}

#endif // MFileSystem_ReadableFile_h
