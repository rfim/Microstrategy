//==============================================================================================
//	FILENAME	:	StringCollectionImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	12/19/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef MDataType_StringCollectionImpl_h
#define MDataType_StringCollectionImpl_h

#include "PDCvector"
#include <string>

#include "../StringCollection.h"

namespace MDataType
{
	/**
	 *	The implementation class for StringCollection interface.
	 */
	class StringCollectionImpl:
		public StringCollection
	{
	public:
	
		StringCollectionImpl();
		virtual ~StringCollectionImpl() throw()
		{
		}

	protected:

		/// @see MDataType::StringCollection::Insert
		virtual void Insert(const char* ipString);

		/// @see MDataType::StringCollection::Insert
		virtual void Insert(const StringCollection& irStringCollection);

		/// @see MDataType::StringCollection::GetCount
		virtual size_t GetCount() const throw();

		/// @see MDataType::StringCollection::GetString
		virtual const char* GetString(size_t iStringIndex) const;

		/// @see MDataType::StringCollection::Clear
		virtual void Clear();

	public:

		// The elements (pointers) of mvStrings are allocated on heap.
		typedef std::vector<std::string*> StringVector;

	private:

		StringVector mvStrings;

		/**
		 * this class is responsible for cleaning up a StringVector
		 */
		class StringVectorGuard
		{
		public:

			StringVectorGuard(StringVector& irStringVector);
			~StringVectorGuard() throw();
			void Dismiss() throw();

		private:

			StringVector& mrStringVector;
			bool mDismissed;
		};

		const StringVectorGuard mCleanupGuard;
	};
}

#endif // MDataType_StringCollectionImpl_h
