//==============================================================================================
//	FILENAME	:	MultiReadStream.h
//	AUTHOR		:	Hurwood
//	PROJECT		:	PDSerialization
//
//	CREATION	:	2005-09-08
//	Copyright (C) MicroStrategy, Inc. 2005
//==============================================================================================
#ifndef MSerialization_MultiReadStream_h
#define MSerialization_MultiReadStream_h

#include "PDSerialization.h"
#include "ReadDataStream.h"
#include "WriteDataStream.h"
#include "ReadBlockStream.h"
#include "DataType/DataType/BinaryCollection.h"
#include "Base/Defines/SmartPtrI.h"
#include "Base/Defines/ReturnPtrI.h"
#include "Base/Interfaces/ReferenceCounted.h"
#include "Synch/Defines/ReferenceCountedImplementation.h"
#include "Base/Interfaces/BaseMemoryContract.h"

namespace MSerialization
{

	/**
		A multi-read stream is an object that can be used to make several read streams.
		The streams are 'multi-read' in the sense that multiple threads can read from
		the streams at the same time.
		@par
		The multi-read stream class is reference counted.  Each read stream created from
		the class takes a reference on the multi-read class that created it.  This ensures
		that no problem will occur if the multi-read stream is released before all of the
		read streams that were created from it were themselves deleted.
		@par
		There is no multi-write stream class.  Such a class does not make sense, since
		the data would be interleaved from different writers.  By design we do not include
		a method for the multiread stream to give up ownership of its collection.
	*/
	class DLL_PDSERIALIZATION_EXIM MultiReadStream :
		public MBase::ReferenceCounted
	{
	public:
		typedef MBase::SmartPtrI<MultiReadStream> Ptr;
		typedef MBase::ReturnPtrI<MultiReadStream> ReturnPtr;

	/**
		Factory method to create a multi-read stream from a binary collection.
		The caller passes in the binary collection whose data will be exposed by
		the read streams created from this class.
		@param ipCollection The data we expose in the stream.  Should not be NULL.
		@return Reference to the newly created multi-read object.
	*/
	static ReturnPtr Create(MDataType::BinaryCollection* ipCollection);

	/**
		Factory method to create a multi-read stream from a write data stream.
		The binary collection from the write data stream is taken by the read stream.
		Note that this method will work on write block streams as well.
		@param ipWriteStream A write data stream.  Must not be NULL.
		@return Reference to the newly created multi-read object.
	*/
	static ReturnPtr Create(WriteDataStream* ipWriteStream);

	/**
		Create a read data stream that reads from this multi-read stream.
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@par
		The read data stream should be deleted as usual when the caller is finished with it.
		It will hold a reference to this multi-read stream, so it
		is safe for the caller to release the multi-read stream before deleting the data stream.
		However the method on the data stream to give up the binary collection should not be
		called, since the collection still belongs to the multi-read stream, not the data stream.
		(An attempt to call it will _ASSERT and throw a SerializationException).
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return the pointer to the created ReadDataStream.
	*/
	ReadDataStream* GetAsReadDataStream(MBase::MemoryContract* ipMemCon = 0);

	/**
		Create a read block stream that reads from this multi-read stream.
		If necessary, the caller can provide a MemoryContract to guard the memory allocation during
		the stream operation.
		@par
		The read block stream should be deleted as usual when the caller is finished with it.
		It will hold a reference to this multi-read stream, so it
		is safe for the caller to release the multi-read stream before deleting the block stream.
		However the method on the block stream to give up the binary collection should not be
		called, since the collection still belongs to the multi-read stream, not the block stream.
		(An attempt to call it will _ASSERT and throw a SerializationException).
		@param ipMemCon the pointer to the memeory contractor. Its default value is NULL.
		@return the pointer to the created ReadBlockStream.
	*/
	ReadBlockStream* GetAsReadBlockStream(MBase::MemoryContract* ipMemCon = 0);

	/**
		Write the data in this stream into a given write stream.
		The data is written as binary data.  There is no way to write it using data types.
		@par
		More precisely this method first writes the number of binary objects in the binary
		collection that underlies this multi-read stream (as an unsigned Int32),
		and then writes the binaries in order, as binaries.
		@param ipWriteStream The stream into which the data is written.
	*/
	void WriteBinaryToStream(WriteDataStream* ipWriteStream);

	/*
		return the number of binary
	*/
	unsigned Int32 GetBinaryCount();

	/*
		caller does not assume ownership of this binary
	*/
	const MDataType::Binary* GetBinaryByIndex(unsigned Int32 iIndex);

	protected:

		/// Ensure the object cannot be deleted directly
		virtual ~MultiReadStream() throw() {}

	private:

		/**
			Create a binary collection owner for a given binary collection.
			@param ipCollection The collection to own.  Should never be NULL.
		*/
		MultiReadStream(MDataType::BinaryCollection* ipCollection);

		/// The owned binary collection
		MDataType::BinaryCollectionPtr mBinaryCollectionPtr;

		// Support reference counting
		REFERENCE_COUNTED_IMPLEMENTATION
	};

}

#endif // MSerialization_MultiReadStream_h
