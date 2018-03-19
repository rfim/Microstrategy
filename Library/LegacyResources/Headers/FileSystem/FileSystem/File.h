//==============================================================================================
//	FILENAME	:	File.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2001-11-28
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_File_h
#define MFileSystem_File_h

#include <stdio.h>
#include <string.h>

#include "FileSystem.h"

namespace MDataType
{
	// forward declarations
	class DateTime;
	class Binary;
}

namespace MFileSystem
{
	/**
	 * File contains functions operating upon a file.
	 */
	class DLL_FILE_SYSTEM_EXIM File
	{
	public:

		/**
		 * Moves a file/directory.
		 *
		 * The function overwrites the target if it exists.
		 */
		static void Move( const char* ipPathFrom, const char* ipPathTo );

		/**
		 * Deletes a file. Does not throw any exceptions.
		 *
		 * @see MFileSystem::Directory::DeleteEmpty
		 * @return true if deletion succeeded
		 */
		static bool Delete(const char* ipPath);

		/**
		 * Reads data from one descriptor/stream/path and writes that data
		 * into the other descriptor/stream using the Binary as a buffer.
		 *
		 * Note: reading starts at the current position and continues until the end of file.
		 *
		 * @param iDescriptorFrom/ipStreamFrom/ipPathFrom is the source.
		 * @param iDescriptorTo/ipStreamTo/ipPathTo is the destination.
		 * @param irBinary used as a buffer for copying.
		 */
		static void Copy(const char* ipPathFrom, const char* ipPathTo, MDataType::Binary& irBinary);

		/// This method allocates a default-size buffer (~16KB).
		static void Copy(const char* ipPathFrom, const char* ipPathTo);

		/// This method copies file attributes.
		static bool CopyAttributes(const char* ipPathFrom, const char* ipPathTo);

		/**
		 * Returns size of the specified file.
		 */
		static off_t GetSize( const char* ipPath );

		/**
		 * Returns the UTC time of last modification.
		 */
		static void GetLastModified(const char* ipPath, MDataType::DateTime& orDateTime);


		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////  O  B  S  O  L  E  T  E   ///////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		/**
		 * Defines a mode for opening a file.
		 *
		 * The available modes correspond to the values of the second parameter to fopen(),
		 * which, in turn, translate into the low level file-open flags defined in <fcntl.h>.
		 *
		 * @see fopen()
		 * @see open()
		 */
		enum OpenMode
		{
			OPEN_MODE_READ,                // "r"  --> read
			OPEN_MODE_WRITE_FROM_SCRATCH,  // "w"  --> write/create/truncate
			OPEN_MODE_WRITE_APPEND,        // "a"  --> write/create/append
			OPEN_MODE_UPDATE_EXISTING,     // "r+" --> read/write
			OPEN_MODE_UPDATE_FROM_SCRATCH, // "w+" --> read/write/create/truncate
			OPEN_MODE_UPDATE_APPEND,       // "a+" --> read/write/create/append
			OPEN_MODE_WRITE				   // write/create/NO truncate, This is added for OpenDescriptor
		};

		/**
		 * Defines a mode for sharing a file with other processes.
		 */
		enum ShareMode
		{
			SHARE_MODE_DEFAULT,        // default sharing
			SHARE_MODE_NONE,           // no sharing at all
			SHARE_MODE_READ,           // other processes can read this file
			SHARE_MODE_READ_AND_WRITE  // other processes can read/write this file
		};

		/**
		 * Platform-independent way to open a file descriptor.
		 *
		 * Note: on Windows, all files are opened in "binary" mode;
		 * the "text" mode is dead.
		 *
		 * Use the file descriptor with *fstream classes defined in <fstream.h>,
		 * with their constructor.
		 * 
		 * @return a file descriptor, Exception is thrown if the operation failed.
		 *
		 * @see open()
		 */
	/*	static int OpenDescriptor(
			const char *ipPath,
			OpenMode iOpenMode,
			ShareMode iShareMode = SHARE_MODE_DEFAULT);
*/
		/**
		 * Platform-independent way to open a file stream.
		 *
		 * Note: on Windows, all files are opened in "binary" mode;
		 * the "text" mode is dead.
		 *
		 * @return pointer to FILE, Exception is thrown if the operation failed.
		 *
		 * @see fopen()
		 */
		static FILE* OpenStream(
			const char* ipPath,
			OpenMode iOpenMode,
			ShareMode iShareMode = SHARE_MODE_DEFAULT);


		/**
		 * Reads data from one descriptor/stream/path and writes that data
		 * into the other descriptor/stream using the Binary as a buffer.
		 *
		 * Note: reading starts at the current position and continues until the end of file.
		 *
		 * @param iDescriptorFrom/ipStreamFrom/ipPathFrom is the source.
		 * @param iDescriptorTo/ipStreamTo/ipPathTo is the destination.
		 * @param irBinary used as a buffer for copying.
		 */
		static void Copy(int iDescriptorFrom, int iDescriptorTo, MDataType::Binary& irBinary);
		static void Copy(FILE* ipStreamFrom, FILE* ipStreamTo, MDataType::Binary& irBinary);

		/// This is a convenience method for backward compatibility.
		static void Copy(int iDescriptorFrom, const char* ipPathTo);

		/// This is a convenience method for backward compatibility.
		static void Copy(FILE* ipStreamFrom, const char* ipPathTo);

		/**
		 * Returns size of the specified file.
		 *
		 * @see OpenFileDescriptor( const char* )
		 */
		static off_t GetSize(int inDescriptor);
		static off_t GetSize(FILE* ipStream);

		/**
		 * Writes a null-terminated character string into a file stream.
		 * If iLength is 0, the function writes ::strlen() characters,
		 * otherwise, it uses the provided value of iLength.
		 *
		 * NOTE: it is required on most platforms to "flush" a buffered stream after
		 * writing and before reading/seeking. Otherwise, either SetPosition will fail,
		 * or the resulting file will be corrupt.
		 *
		 * @see MFileSystem::File::Flush
		 * @see MFileSystem::File::SetPosition
		 */
		static void Write( FILE* ipStream, const char* ipString, size_t iLength = 0 );

		/**
		 * Writes raw data into a file stream.
		 */
		static void Write( FILE* ipStream, const void* ipData, size_t iByteLength );

		/**
		 * Read raw data from a file stream. GetSize method of irBinary returns the 
		 * number of bytes read.
		 *
		 * @param iMaxSize how many bytes to read, if 0 maximum capacity of irBinary is read.
		 */
		static void Read(FILE* ipStream, MDataType::Binary& irBinary, size_t iMaxSize = 0);

		/**
		 * Read new-line delimited data from a file stream.
		 * The new-line character is preserved.
		 * If the last returned character is not '\n', then there is more data to be read.
		 */
		static void ReadLine(FILE* ipStream, MDataType::Binary& irBinary);

		/**
		 * Flushes a file descriptor/stream.
		 */
		static void Flush(int iDescriptor);
		static void Flush(FILE* ipStream);

		/**
		 * Truncates a file (changes file's size) to specified size.
		 */
		static void Truncate(int iDescriptor, off_t iSize = 0);
		static void Truncate(FILE* ipStream, off_t iSize = 0);

		/**
		 * Returns the curent file position.
		 */
		static off_t GetPosition(int iDescriptor);
		static off_t GetPosition(FILE* ipStream);

		/**
		 * Moves the file pointer relative to the current position.
		 */
		static void MovePosition(int iDescriptor, off_t iOffset);

		/**
		 * Moves the file pointer relative to the current position.
		 * Also if successful, this method clears the end-of-file indicator and
		 * any prior ungetc calls with this stream.
		 */
		static void MovePosition(FILE* ipStream, off_t iOffset);

		/**
		 * Moves the file pointer to the absolute position.
		 */
		static void SetPosition(int iDescriptor, off_t iOffset);

		/**
		 * Moves the file pointer to the absolute position.
		 * Also if successful, this method clears the end-of-file indicator and
		 * any prior ungetc calls with this stream.
		 */
		static void SetPosition(FILE* ipStream, off_t iOffset);

		/**
		 * Copies the data block at current position to iTo preserving the data
		 * if source and destination locations overlap.
		 *
		 * @param iSize is block size, if 0, the size is calculated as (file size - iFrom)
		 * @param irBinary used as a buffer for data copying
		 */
		static void CopyBlock(FILE* ipStream, MDataType::Binary& irBinary, off_t iTo, off_t iSize = 0);

		/**
		 * Exclusive-access smart lock object.
		 *
		 * CAUTION: file locking is NOT thread-safe!
		 * So when you lock a file -- you must be in
		 * a critical section that protects that file.
		 *
		 * NOTE 1: on Windows, file locking is mandatory,
		 * which means read/write operations may fail
		 * with ERROR_LOCK_VIOLATION on a locked file.
		 * On UNIX, however, file locking is voluntary
		 * (or advisory) so read/write will succeed
		 * even if somebody else has exclusively locked 
		 * a file.
		 *
		 * NOTE 2: on Windows, file locking seems to
		 * work per file handle -- so using a different
		 * handle to the same file within the same process
		 * may fail with ERROR_LOCK_VIOLATION.
		 */
		class DLL_FILE_SYSTEM_EXIM WriteSmartLock
		{
		public:
			WriteSmartLock( int iDescriptor );
			WriteSmartLock(FILE* ipStream);
			~WriteSmartLock() throw();

			void Lock();
			void Unlock() throw();

		private:
			const int mDescriptor;
			bool mIsLocked;
		};	

		/**
		 * Shared-access smart lock object.
		 *
		 * CAUTION: file locking is NOT thread-safe!
		 * So when you lock a file -- you must be in
		 * a critical section that protects that file.
		 *
		 * @see WriteSmartLock
		 */
		class DLL_FILE_SYSTEM_EXIM ReadSmartLock
		{
		public:
			ReadSmartLock( int iDescriptor );
			ReadSmartLock(FILE* ipStream);
			~ReadSmartLock() throw();

			void Lock();
			void Unlock() throw();

		private:
			const int mDescriptor;
			bool mIsLocked;
		};

	protected:
		friend class WriteSmartLock;
		friend class ReadSmartLock;

		friend class ReadableFileImpl;
		friend class WriteableFileImpl;

		static void WriteLock(int iDescriptor, bool bIsWait = true);
		static void ReadLock(int iDescriptor, bool bIsWait = true);
		static void Unlock( int iDescriptor ) throw();
	};
}

#endif // MFileSystem_File_h
