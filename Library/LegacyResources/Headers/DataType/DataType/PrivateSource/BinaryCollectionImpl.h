//==============================================================================================
//	FILENAME	:	BinaryCollectionImpl.h
//	AUTHOR		:	Yi Luo
//	PROJECT		:	DataType
//
//	CREATION	:	08/16/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef DataType_BinaryCollectionImpl_h
#define DataType_BinaryCollectionImpl_h

#include "PDCHeader/PDCvector"

#include "DataType/DataType/BinaryCollection.h"

namespace MDataType
{
	/**
		The class is the implementation of BinaryCollection.
	*/
	class BinaryCollectionImpl : public BinaryCollection
	{
	public:

		/**
			Insert one Binary into the collection.
			The BinaryCollection will take the ownership of the ipBinary. So the caller should not
			delete the Binary after calling this function.
			@param ipBinary the pointer to the inserted Binary.
		*/
		void Insert(Binary* ipBinary);

		void Merge(BinaryCollection* ipBinaryCollection);

		///	Get the number of Binaries in the collection.
		unsigned Int32 GetBinaryCount() const throw();

		/// Get the Binary specified by the iBinaryIndex.
		/// @throws MBase::BaseException if the required Binary cannot be returned.
		const Binary* GetBinary(unsigned Int32 iBinaryIndex) const;

		/// Get the total size of all the binary
		unsigned Int64 GetTotalLength() const throw();

		void Delete() throw();

		BinaryCollectionImpl();
		virtual ~BinaryCollectionImpl() throw();

	protected:
		
		// Give up the ownership of all the Binary in the collection.
		// Used by Merge().
		void GiveUp() throw();

	private:
		std::vector<Binary*> mBinaries;
		
	};
}

#endif // DataType_BinaryCollectionImpl_h
