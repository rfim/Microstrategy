//==============================================================================================
//	FILENAME	:	FileImpl.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-11
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_FileImpl_h
#define MFileSystem_FileImpl_h

#include "Base/Defines/BasicTypes.h"

namespace MDataType
{
	// forward declarations
	class Binary;
}

namespace MFileSystem
{
	typedef int FileHandle;
	#define INVALID_HANDLE_VALUE (-1)

	/**
	 * File contains functions operating upon a file.
	 */
	class FileImpl
	{
	public:

		/**
		 * Returns size of the specified file.
		 */
		static Int64 GetSize(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Writes a null-terminated character string into a file stream.
		 * If iLength is 0, the function writes ::strlen() characters,
		 * otherwise, it uses the provided value of iLength.
		 */
		static void Write(const char* ipPath, FileHandle iFileHandle, const char* ipString, size_t iLength = 0);

		/**
		 * Writes raw data into a file stream.
		 */
		static void Write(const char* ipPath, FileHandle iFileHandle, const void* ipBuffer, size_t iBufferSize);

		/**
		 * Read raw data from a file stream.
		 */
		static void Read(const char* ipPath, FileHandle iFileHandle, void* ipBuffer, size_t iBufferSize);

		/**
		 * Read raw data from a file stream. GetSize method of irBinary returns the 
		 * number of bytes read.
		 *
		 * @param iMaxSize how many bytes to read, if 0 maximum capacity of irBinary is read.
		 */
		static void Read(const char* ipPath, FileHandle iFileHandle, MDataType::Binary& irBinary, size_t iMaxSize = 0);

		/**
		 * Read new-line delimited data from a file stream.
		 * The new-line character is preserved.
		 * If the last returned character is not '\n', then there is more data to be read.
		 */
		static void ReadLine(const char* ipPath, FileHandle iFileHandle, MDataType::Binary& irBinary);

		/**
		 * Flushes a file descriptor/stream.
		 */
		static void Flush(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Truncates a file (changes file's size) to specified size.
		 */
		static void Truncate(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Returns the curent file position.
		 */
		static Int64 GetPosition(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Moves the file pointer relative to the current position.
		 */
		static Int64 MovePosition(const char* ipPath, FileHandle iFileHandle, Int64 iOffset);

		/**
		 * Moves the file pointer to the absolute position.
		 */
		static void SetPosition(const char* ipPath, FileHandle iFileHandle, Int64 iOffset);

		/**
		 * Moves the file pointer to the end of the file.
		 */
		static Int64 SetPositionToEnd(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Closes the file handle.
		 */
		static void CloseAndGroundHandle(const char* ipPath, FileHandle& irFileHandle);

		/**
		 * Closes the file handle.
		 */
		static void CloseAndGroundHandle_NoThrow(FileHandle& irFileHandle) throw();

		/**
		 * Truncates the file and then closes the file handle. Errors are ignored.
		 */
		static void TruncateFile_ThenCloseAndGroundHandle_NoThrow(FileHandle& irFileHandle) throw();

		/**
		 * Whether the file handle matches the path.
		 */
		static bool DoesPathMatchFileHandle(const char* ipPath, FileHandle iFileHandle);

		/**
		 * Locks a file for exclusive access.
		 */
		static bool WriteLock(const char* ipPath, FileHandle iFileHandle, bool iNoWait = false);
		
		/**
		 * Locks a file for shared access.
		 */
		static bool ReadLock(const char* ipPath, FileHandle iFileHandle, bool iNoWait = false);

		/**
		 * Unlocks a previously locked file.
		 *
		 * @return true if unlocking succeeded.
		 */
		static void Unlock(FileHandle iFileHandle) throw();
	};
}

#endif // MFileSystem_FileImpl_h
