//==============================================================================================
//	FILENAME	:	SmartPtr.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	10/19/2001
//  Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_SmartPtr_h
#define MSynch_SmartPtr_h

#include "SmartBase.h"

#ifdef _MSC_VER
#pragma warning(disable:4284) // return type for 'identifier::operator ->' is not a UDT or reference to a UDT. Will produce errors if applied using infix notation
#endif

namespace MSynch
{
	///////////////////////////////////////////////////////////////
	// Use SmartPtr when you wish to manipulate a resource by using
	// reference count technique, if and only if 1) that resource does
	// not implement reference counting already and 2) you will never
	// pass that resource outside the current constellation. If either 1)
	// or 2) is not true, you must use SmartObjX.
	// If you do not wish to use reference counting, you may use StrongObj.
	// Remember that StrongObj doesn't work with collections and SmartPtr does.
	//
	// We _ASSERTE() when the reference count goes over 10000, which we consider
	// corrupt.
	// Notice that the constructor of a SmartPtr may throw, but it will
	// cleanup the resource it is guarding if that happens.
	// We verify errors when dereferencing, not when initializing, and we
	// do so by asserting.
	// SmartPtr does not have the & operator defined. This is on purpose, we cannot
	// allow the user to modify the data without the reference count.
	///////////////////////////////////////////////////////////////

	template<class T,class DeleteOperator=MBase::DeleteC<T> >
	class SmartPtr:
		public SmartBase<T*,DeleteOperator>
	{
	public:
		SmartPtr(T* const ipData = NULL): SmartBase<T*,DeleteOperator>(ipData)
		{
		}

		// copy constructor allowed on SmartPtr
		// upcast should be implicit
		template<class U,class DeleteOperatorU>
		SmartPtr(const SmartPtr<U,DeleteOperatorU>& irSmartPtr) throw() : 
			SmartBase<T*,DeleteOperator>(
				irSmartPtr.mData,
				reinterpret_cast<Int32*>(irSmartPtr.mpnRefCount))
		{
		}

		// Assignment operator of raw pointer
		// upcast should be implicit
		SmartPtr& operator=(T* const ipData)
		{
			this->Reset(ipData);
			return *this;
		}

		
		// Assignment operator (template specialization)
		// upcast should be implicit
		SmartPtr& operator=(const SmartPtr& irSmartPtr) throw()
		{
			// only modify if not the same pointed object
			if (this->mData != static_cast<T*>(irSmartPtr.mData))
			{
				this->ReplaceWith(irSmartPtr.mData,reinterpret_cast<Int32*>(irSmartPtr.mpnRefCount));
			}
			return *this;
		}

		T* operator->() const throw()
		{
			_ASSERTE(this->mData!=NULL);
			return this->mData;
		}

		T& operator*() const throw()
		{
			_ASSERTE(this->mData!=NULL);
			return *(this->mData);
		}

		template<class U,class DeleteOperatorU>
		void DownCastFrom(SmartPtr<U,DeleteOperatorU>& irSmartPtr) throw()
		{
			// Only modify if iData is different from the current member
			if(static_cast<U*>(this->mData) != irSmartPtr.mData)
			{
				ReplaceWith(reinterpret_cast<T*>(irSmartPtr.mData),irSmartPtr.mpnRefCount);
			}
		}
	};


	// 2002-04-11 -- vovechkin: [SmartPtrArray vs. SmartArrayPtr]
	// As I mentioned to Juan, the name SmartPtrArray gives an impression
	// of an "array of smart pointers", which is not how this template works. 
	// In reality, it is a "smart pointer to an array of T". 
	// Hence, a better name: SmartPtrArray.

	///////////////////////////////////////////////
	// SmartArrayPtr is a specialization of SmartPtr for use with C++ arrays.
	template<class T>
	class SmartArrayPtr:
		public SmartPtr<T, MBase::DeleteArray<T> >
	{
	public:
		SmartArrayPtr(T* ipData = NULL):
			SmartPtr<T,MBase::DeleteArray<T> >(ipData)
		{
		}

		// copy constructor allowed on SmartPtr
		// upcast should be implicit
		template<class U>
		SmartArrayPtr(SmartArrayPtr<U>& irSmartPtr) throw(): 
			SmartPtr<T,MBase::DeleteArray<T> >(irSmartPtr)
		{
		}

		SmartArrayPtr& operator=(T* const ipData)
		{
			SmartPtr<T,MBase::DeleteArray<T> >::operator=(ipData);
			return *this;
		}

		template<class U>
		SmartArrayPtr& operator=(const SmartArrayPtr<U>& irSmartPtr) throw()
		{
			SmartPtr<T,MBase::DeleteArray<T> >::operator=(irSmartPtr);
			return *this;
		}

		T& operator[](int i) const
		{
			_ASSERTE(this->mData!=NULL); 
			return (this->mData)[i];
		}
	};
}

#endif // MSynch_SmartPtr_h
