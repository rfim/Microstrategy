//==============================================================================================
//	FILENAME	:	StringCollection.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	12/19/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef MDataType_StringCollection_h
#define MDataType_StringCollection_h

#include "DataType.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Defines/ReturnPtr.h"

namespace MDataType
{
	/**
	 *	Use this class to hold and do simple operations on a collection of String data.
	 */
	class StringCollection
	{
	public:
		// virtual destructor is a must
		virtual ~StringCollection() throw()
		{
		}

		typedef MBase::StrongPtr<StringCollection> Ptr;
		typedef MBase::ReturnPtr<StringCollection> ReturnPtr;

		/// insert a wide string into the collection.
		/// The memory will be allocated inside this function.
		/// NULL values are allowed
		virtual void Insert(const char* ipString) = 0;

		/// insert another string collection into this one
		virtual void Insert(const StringCollection& irStringCollection) = 0;

		/// get the number of items in this collection
		virtual size_t GetCount() const throw() = 0;

		/// get the string specified by its location in the collection.
		virtual const char* GetString(size_t iStringIndex) const = 0;

		/// remove all elements in the collection
		virtual void Clear() = 0;
	};

	// this typedef is obsolete
	typedef StringCollection::Ptr StringCollectionPtr;

	/**
	 *	This factory method creates a MDataType::StringCollection object that is empty.
	 */
	DLL_DATATYPE_EXIM StringCollection* CreateStringCollection();
}

#endif // MDataType_StringCollection_h
