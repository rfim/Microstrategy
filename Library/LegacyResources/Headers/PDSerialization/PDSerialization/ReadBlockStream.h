//==============================================================================================
//	FILENAME	:	ReadBlockStream.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	PDSerialization
//
//	CREATION	:	2002-07-15
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef Serialization_ReadBlockStream_H
#define Serialization_ReadBlockStream_H

#include "PDSerialization.h"
#include "DataType/DataType/Binary.h"
#include "PDSerialization/PDSerialization/ReadDataStream.h"

namespace MSerialization
{
	class WriteBlockStream;

	/**
		The interface for reading list structure

		The usage scenario is:
		@par	ReadListStream * lpStream = ...;
		@par	lpStream->StartReadList();
		@par	while ( lpStream->MoreElement() )
		@par	{
		@par		// read one and only one element from the list stream.
		@par		// the element must be basic data types which can be read through ReadDataStream.
		@par	}
	*/
	class DLL_PDSERIALIZATION_EXIM ReadListStream: public ReadDataStream
	{
	public:

		/**	Start reading a list.
			Before actually reading the data, the caller must call MoreElement()
			to make sure there is more element in the list, because we can have list with no element.
			@throws ReadStreamException if the stream is corrupted.
		*/
		virtual void StartReadList()=0;

		/**	check whether the list has more element for reading.
			This function must be called before each reading of the element in the list.
			@return @li @c true there are more element in the list.
				@li	@c false there is no more element in the list.
		*/
		virtual bool MoreElement() throw() =0;

	protected:
		virtual ~ReadListStream(){};

	};


	/**
		The interface is for read block information and navigate through blcok.
	*/
	class DLL_PDSERIALIZATION_EXIM ReadBlockStream: public ReadListStream
	{
	public:

		/**
			return the block type, and signal the start of a block.
			This function assumes the current reading pointer points to the start of
			a block. If this is not true, the behavior is unpredictable unless the stream
			is created using CreateSafeWriteBlockStream(). For safe stream, an exception will be
			thrown indicating the stream is corrupted.
			@return the block type.
			@throws ReadStreamException if the stream is corrupted.
		*/
		virtual unsigned short ReadBlockHeader()=0;

		/// Finish the reading of current block and skip any unread data.
		/// and park the reading pointer right after the end of the block.
		virtual void FinishReadBlock()=0;

		/// Check whether there is more data inside the current block.
		/// @return @li @c true if there is more data.
		///			@li @c false if there is no more data inside this block.
		virtual bool MoreDataInBlock()=0;


	protected:
		virtual ~ReadBlockStream(){};

	};

	/**
		This is a helper class to automate the call of ReadBlockStream::FinishCurrentBlock().
		The usage is:
		@par ReadBlockStream * lpStream = ...;
		@par {
		@par	AutoReadBlockStreamCloser lCloser(* lpStream);
		@par	unsigned short lBlockType = lpStream->GetBlockType();
		@par	// read elements from the block stream
		@par
		@par	// when the block is finished, don't call FinishCurrentBlock().
		@par	// instead, let ~AutoReadBlockStreamCloser() to do that.
		@par }

	*/
	class DLL_PDSERIALIZATION_EXIM AutoReadBlockStreamCloser
	{
	public:
		AutoReadBlockStreamCloser(ReadBlockStream* ipReadStream);
		~AutoReadBlockStreamCloser() throw();

	private:
		// pointer to the ReadBlockStream
		ReadBlockStream* mpReadStream;
	};


	typedef MBase::StrongPtr<ReadBlockStream, MBase::DeleteOperatorGeneric<ReadBlockStream> > ReadBlockStreamPtr;

	// factory function to create a ReadBlockStream from a Binary.

	/**
		Create a ReadBlockStream object from a Binary block.
		The ReadBlockStream TAKE OVER the ownership of the Binary, so if the caller want to retain
		the Binary for other purpose beyond the lifetime of the ReadDataStream, it must duplicate
		the Binary. This also means the caller should not try to delete the Binary once it is given
		to the ReadBlockStream.
		@par
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@param ipBinary The pointer to the Binary.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return a pointer to the ReadBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM ReadBlockStream * ConvertBinary2ReadBlockStream(MDataType::Binary * ipBinary,
																			 MBase::MemoryContract * ipMemCon = 0);

	/**
		Create a ReadBlockStream object from a BinaryCollection block.
		The ReadBlockStream TAKE OVER the ownership of the BinaryCollection, so if the caller want to retain
		the Binary for other purpose beyond the lifetime of the ReadDataStream, it must duplicate
		the Binary. This also means the caller should not try to delete the Binary once it is given
		to the ReadBlockStream.
		@par
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@param ipBinaryCol The pointer to the BinaryCollection.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return a pointer to the ReadBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM ReadBlockStream * ConvertBinaryCollection2ReadBlockStream(MDataType::BinaryCollection * ipBinaryCol,
																			 MBase::MemoryContract * ipMemCon = 0);

	/**
		Create a ReadBlockStream from a WriteBlockStream directly.
		The ReadBlockStream TAKE OVER the ownership of the underlying data in the WriteBlockStream,
		so the caller cannot use WriteBlockStream::GiveOutBinary() to get those data after calling
		this function.
		@par
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@param ipWriteStream The pointer to the WriteBlockStream.
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return a pointer to the ReadBlockStream object.
	*/
	DLL_PDSERIALIZATION_EXIM ReadBlockStream * ConvertWriteBlockStream2ReadBlockStream(
													WriteBlockStream* ipWriteStream, MBase::MemoryContract * ipMemCon = 0);

	/**
		We don't have a ConvertBinary2ReadListStream here, because there is no need
		for a list stream which is not a block stream. Every list stream is a blcok stream too.
		We separate the interface just for the purpose of clarity.
	*/

	/**
			The following method is used for reading serialized data from a file.
	 		@param iFilePtr Strong pointer to a Readable file that contains a block stream.
	 		The caller opens a Readable file and gives up ownership of the file pointer to the
	 		block stream.  When all is done the block stream commits and closes the file.
	 */
	DLL_PDSERIALIZATION_EXIM ReadBlockStream* GetReadBlockStreamFromFile(MFileSystem::ReadableFile::Ptr& iFilePtr);
}

#endif // Serialization_ReadBlockStream_H
