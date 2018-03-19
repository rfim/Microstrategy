//==============================================================================================
//	FILENAME	:	WriteBlockStream.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	PDSerialization
//
//	CREATION	:	2002-07-15
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_WriteBlockStream_H
#define Serialization_WriteBlockStream_H

#include "PDSerialization.h"
#include "PDSerialization/PDSerialization/WriteDataStream.h"
#ifndef __ANDROID__
#include "Encryptor/Encryptor/EncryptorDecryptor.h"
#endif
namespace MSerialization
{
	/**
		The interface for writing list structure
		Since it inherits from WriteDataStream, it can also be used for write data directly.

		The usage scenario is:
		@par	WriteListStream * lpStream = ...;
		@par	lpStream->StartList();
		@par	(*lpStream) << 1.45;
		@par
		@par	lpStream->ContinueList();
		@par	(*lpStream) << 2.34;
		@par	lpStream->ContinueList();
		@par	(*lpStream) << 5.34;
		@par
		@par	(*lpStream) << 5.34;  // the first write without calling ContinueList() will terminate the list.
		@par

	*/
	class DLL_PDSERIALIZATION_EXIM WriteListStream :  public WriteDataStream
	{
	public:
		/// Start a new list
		virtual void StartList()=0;

		/// signal the next write is still inside the list.
		/// @throws WriteStreamException if there is no corresponding StartList().
		virtual void ContinueList()=0;

	protected:
		virtual ~WriteListStream() throw() {};
	};

	/**
		The class is for Write block information into the stream.
		Since it inherits from WriteDataStream, it can also be used for write data directly.

		The two function must be used in pairs. Each WriteBlockHeader() must have one and only one
		CloseBlock() call when the Stream is finalized using GiveoutBinary().

		I don't provide an AutoWriteBlockCloser because I want the caller always close the block
		intentionally. If any unexpected events occurs before it can close the block, the stream should
		be treated as corrupted and should not be used. A missing block end is a good way to indicate this
		error.
	*/
	class DLL_PDSERIALIZATION_EXIM WriteBlockStream :  public WriteListStream
	{
	public:

		/**
			Start a new block and write the specified block header into the stream.
			this new block can contain more than 4G data.
			@param iBlockType a 16-bit integer indicating the type of the block. The meaning of the type
			is defined by the caller. No global definition for this type exists.
		*/
		virtual void WriteBlockHeader(unsigned short iBlockType)=0;

		/// Close the block structure.
		/// @throws WriteStreamException if there is no open block when it is called.
		virtual void FinishWriteBlock()=0;

		/// Close the block structure.
		///	@param iBlockType a 16-bit integer indicating the type of the block. The meaning of the type
		///	is defined by the caller. No global definition for this type exists.
		/// @throws WriteStreamException if there is no open block when it is called.
		virtual void FinishWriteBlock(unsigned short iBlockType)=0;

	protected:
		virtual ~WriteBlockStream() throw() {};

	};

	typedef MBase::StrongPtr<WriteBlockStream, MBase::DeleteOperatorGeneric<WriteBlockStream> > WriteBlockStreamPtr;

	/**
		create a WriteBlockStream, using the specified stream header.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@param iHeader the stream header, containing the stream version information.
		@return a pointer to the WriteBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateWriteBlockStream(MBase::MemoryContract* ipMemCon=0,
																	  unsigned short iHeader = 1);

	/**
		create a fast but platform-dependent WriteBlockStream, using the specified stream header.
		This stream won't convert the data type into platform-neutral format. So its usage must
		be restricted in the same machine.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@param iHeader the stream header, containing the stream version information.
		@return a pointer to the WriteBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateRawWriteBlockStream(MBase::MemoryContract* ipMemCon=0,
																	  unsigned short iHeader = 1);

	/**
		Create a WriteBlockStream with extra safety information, using the specified stream header.
		This stream will put extra information into the stream to make the stream read safer
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@param iHeader the stream header, containing the stream version information.
		@return a pointer to the WriteBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateSafeWriteBlockStream(MBase::MemoryContract* ipMemCon=0,
																		  unsigned short iHeader = 1);

	/**
		Create a WriteBlockStream with extra safety information, using the specified stream header.
		This stream will put extra information into the stream to make the stream read safer

		This is a fast but platform-dependent WriteBlockStream, using the specified stream header.
		This stream won't convert the data type into platform-neutral format. So its usage must
		be restricted in the same machine.

		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@param iHeader the stream header, containing the stream version information.
		@return a pointer to the WriteBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateSafeRawWriteBlockStream(MBase::MemoryContract* ipMemCon=0,
																		  unsigned short iHeader = 1);

	/**	Create a WriteBlockStream from a BinaryCollection.
		The stream header will be decided by the data in the BinaryCollection.
		If the stream header has safe stream flag, the stream will be serialized
		using safe format.
		@par
		Any new writing operation wills start from the end of the stream
		The new WriteBlockStream will take the ownership of the BinaryCollection.
		@param ipBins the pointer to the BinaryCollection which will become the body
			of the WriteBlockStream. This BinaryCollection must
			contain header data. Otherwise, serialization exception will be thrown.
		@param ipMemCon a pointer to MemoryContract. Its default value is NULL.
		@return a pointer to the created WriteBlockStream.
		@throws WriteStreamException.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* ConvertBinaries2WriteBlockStream(
																MDataType::BinaryCollection* ipBinaries,
																MBase::MemoryContract* ipMemCon=0);

	/**
		We don't have a CreateWriteListStream() function here, because there is no need
		for a list stream which is not a block stream. Every list stream is a blcok stream too.
		We separate the interface just for the purpose of clarity.
	*/

	/**
		The following are overloaded methods for writing blocks to files instead of memory.
		Useful when serializing large data.
		@param iFilePtr Strong pointer to a Writeable file. The caller opens a Writeable file and
		gives up ownership of the file pointer to the block stream.  When all is done the block stream
		commits and closes the file.
	 */
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
									      unsigned short header=1);
 	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateRawWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
										 unsigned short header=1);
 	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateSafeWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
										  unsigned short header=1);
 	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreateSafeRawWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
										     unsigned short header=1);

	/**	Create a WriteBlockStream from a BinaryCollection.
		The stream header will be decided by the data in the BinaryCollection.
		If the stream header has safe stream flag, the stream will be serialized
		using safe format.
		The new data is writen at the end of the file.
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* ConvertFile2WriteBlockStream(MFileSystem::UpdateableFile::Ptr& iFilePtr, unsigned short header=1);

#ifndef __ANDROID__
	/**
		Similar to CreateSafeWriteBlockStream except char* and wchar_t* type data will be encrypted with specified algorithm
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreatePartialEncryptedSafeWriteBlockStream(MBase::MemoryContract* ipMemCon=0,
																		  unsigned short iHeader = 1,
																		  MEncryptor::EncryptionVersion iEncryptionVersion=MEncryptor::AES,
																		  const char* ipcKey=NULL);
	/**
		Overloaded method for writing blocks to files instead of memory
	*/
	DLL_PDSERIALIZATION_EXIM WriteBlockStream* CreatePartialEncryptedSafeWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
																		  unsigned short iHeader = 1,
																		  MEncryptor::EncryptionVersion iEncryptionVersion=MEncryptor::AES,
																		  const char* ipcKey=NULL);
#endif
}

#endif // Serialization_WriteBlockStream_H
