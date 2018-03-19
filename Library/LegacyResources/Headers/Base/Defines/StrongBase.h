//==============================================================================================
//	FILENAME	:	StrongBase.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StrongBase_h
#define MBase_StrongBase_h

#include "Null.h"
#include "DeleteOperator.h"
#include "Asserte.h"
#include "Base/Defines/BasicTypes.h"

namespace MBase 
{
	/**
	 * StrongBase is the base class for both StrongObj and StrongPtr.
	 * Do not use StrongBase directly. Use StrongObj or StrongPtr instead.
	 */
	template<class T,class DeleteOperator>
	class StrongBase
	{
	public:
		/// The destructor is not virtual, because strong objects
		/// are never used polymorphically via a base class pointer
		~StrongBase(); 

		/// Use the get operator to get a weak reference. You may use this reference
		/// to pass to a function that 1) is completely enclosed within the StrongBase scope, 
		/// and 2) does not store the resource in any way.
		/// Remember the StrongBase still keeps the ownership of the pointer
		T Get() const	throw()
		{
			return mData;
		}

		/// Reset grounds (deleting if necessary) the old member pointer.
		/// It also sets the member pointer to the new value sent in (if any).
		/// Careful only to reset the pointer if set to a different one.
		/// Resetting to the same one might inadvertly delete the object.
		void Reset(T iData = NULL) throw()
		{
			if(mData!=iData)
			{
				//To avoid problems with circular references make a temporary
				//copy first and set the member to the new one before deleting it
				T lData = mData;
				mData = iData;
				DeleteOperator()(lData);
			}
		}

		/// GiveUp will relinquish ownership of a StrongBase, and will return
		/// the object
		T GiveUp() throw()
		{
			T lTemp=mData;
			mData=NULL;
			return lTemp;
		}

		/// Check for NULL
		bool IsNull() const	throw()
		{
			return mData==NULL;
		}

	protected:
		// Make the constructor explicit, so that we don't incur into
		// funny implicit conversion.
		// The constructor is non-throw because allowing an exception 
		// to escape would mean a resource leak of some soft.
		explicit StrongBase(T iData = NULL) throw():
			mData(iData)
		{
		}

		T mData;
	private:

		////////////////////////////////////////////////////////////////////////
		// No Copy constructor or assignment operator allowed on StrongBase.
		// Since StrongBase has destructive copy, there will never be two
		// copies of the same resource. We chose to disallow the copy altogether,
		// to avoid further bugs.
		// If the user wants to transfer ownership between resources, he must
		// do so explicitly (by calling Acquire or a combination of Reset and GiveUp).
		// Declare and not define copy constructor and assignment operator
		// This prevents the compiler from generating them automatically.
		// It gives a compiler error if someone tries to use them.
		StrongBase(const StrongBase&);
		StrongBase& operator=(const StrongBase&);
		////////////////////////////////////////////////////////////////////////
	};

	template<class T,class DeleteOperator>
	StrongBase<T, DeleteOperator>::~StrongBase()
	{
		DeleteOperator()(mData);
		mData = T();
	}

}

#endif // MBase_StrongBase_h
