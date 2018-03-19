//==============================================================================================
//	FILENAME	:	BinaryCollection.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	DataType
//
//	CREATION	:	08/16/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef MDataType_BinaryCollection_h
#define MDataType_BinaryCollection_h

#include "DataType.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Interfaces/BaseMemoryContract.h"

namespace MDataType
{
	class Binary;

	/**
		The class is a collection of Binaries.
	*/
	class BinaryCollection
	{
	public:

		/**
			Insert one Binary into the collection.
			The BinaryCollection will take the ownership of the ipBinary. So the caller should not
			delete the Binary after calling this function.
			@param ipBinary the pointer to the inserted Binary.
		*/
		virtual void Insert(Binary* ipBinary)=0;

		/**
			Merge the other BinaryCollection into this collection.
			The BinaryCollection will take the ownership of the Binaries in the merged collection. 
			So the caller should not try to get those Binaries through the merged collection
			after calling this function.
			@param ipBinaryCollection the pointer to the merged BinaryCollection.
		*/
		virtual void Merge(BinaryCollection* ipBinaryCollection)=0;

		///	Get the number of Binaries in the collection.
		virtual unsigned Int32 GetBinaryCount() const throw() = 0;

		/**
			Get the Binary specified by the iBinaryIndex.
			@param iBinaryIndex The index to the desired binary in the list. The index starts from 0.
			@return a const pointer to the Binary.
			@throws MBase::BaseException if the required Binary cannot be returned.
		*/
		virtual const Binary* GetBinary(unsigned Int32 iBinaryIndex) const = 0;

		/// Get the total size of all the binary
		virtual unsigned Int64 GetTotalLength() const throw() = 0;

		/// delete this object. Used by StrongPtr.
		virtual void Delete() throw() = 0;

	protected:

		virtual ~BinaryCollection() throw()
		{
		};
	};

	typedef MBase::StrongPtr< BinaryCollection, MBase::DeleteOperatorGeneric<BinaryCollection> > BinaryCollectionPtr;

	DLL_DATATYPE_EXIM BinaryCollection* CreateBinaryCollection();

	/**
		Create a single Binary from a collection of Binaries.
		The data in the ipBinaryCollection will NOT be affected by this operation. 
		So the caller can still get the binary data from the original BinaryCollection.
	*/
	DLL_DATATYPE_EXIM Binary* CreateBinary(BinaryCollection* ipBinaryCollection, MBase::MemoryContract* ipMemContract = NULL);

	/**
		Create a collection of Binaries by combining binaries from the original
		collection of Binaries. The binaries in the new collection will have an 
		upper limit on their sizes.
		The data in the ipBinaryCollection will NOT be affected by this operation. 
		So the caller can still get the binary data from the original BinaryCollection.
	*/
	DLL_DATATYPE_EXIM BinaryCollection* CombineBinaryCollection(
						BinaryCollection* ipBinaryCollection,
						size_t inBinaryUB);

	/**
		This is the MCM governed verion of CombineBinaryCollection
		Create a collection of Binaries by combining binaries from the original
		collection of Binaries. The binaries in the new collection will have an 
		upper limit on their sizes.
		The data in the ipBinaryCollection will NOT be affected by this operation. 
		So the caller can still get the binary data from the original BinaryCollection.
	*/
	DLL_DATATYPE_EXIM BinaryCollection* CombineBinaryCollectionGoverned(
						BinaryCollection* ipBinaryCollection,
						size_t inBinaryUB,
						MBase::MemoryContractPtr& irMemContract);
}

#endif // MDataType_BinaryCollection_h
