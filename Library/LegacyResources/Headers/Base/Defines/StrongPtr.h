//==============================================================================================
//	FILENAME	:	StrongPtr.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StrongPtr_h
#define MBase_StrongPtr_h

#ifdef _MSC_VER
#pragma warning(disable:4786) // 'identifier' : identifier was truncated to 'number' characters in the debug information
#pragma warning(disable:4284) // return type for 'identifier::operator ->' is not a UDT or reference to a UDT. Will produce errors if applied using infix notation
#endif

#include "BasicTypes.h"
#include "StrongBase.h"
#include "ReturnPtr.h"

namespace MBase 
{
	/**
	 * StrongPtr is a smart pointer with destructive copy.
	 * A StrongPtr object OWNS the object it is pointing to.
	 * One and only one StrongPtr may point to the same object in any
	 * given time. 
	 * StrongPtr may not be used with collections 
	 * (may not be in lists, maps, vectors)
	 *	A strong pointer is used to represent direct ownership of a
	 *	resource. Use this template in cases where the ownership of
	 *	a resource is well defined. Other objects can reference the
	 *	resource by using weak pointers (regular C pointers)
	 *	If the owner or a resorce needs to yield ownership of it
	 *	it can do so using the Acquire method.
	 *	DeleteOperator defines which class of Function Object to use
	 *	for deletion. If your pointer has a special way of freeing
	 *	itself, declare a deletion class and use it as an argument
	 *	for the template.
	 *
	 * We allow for StrongPtr to have NULL value. Checking will be done
	 * when dereferencing or by calling IsNull().
	 * Checking on StrongPtr will be done only with asserts.
	 * We will forbid throwing any exception when using SmartPtr.
	 * An exception will be thrown if the NULL is used on the client anyway.
	 * 
	 * StrongPtrs will be typically used for local variables or for data members
	 * where strong ownership (constainment) is desired.
	 */
	template<class T, class DeleteOperator = DeleteC<T> >
	class StrongPtr: public StrongBase<T*, DeleteOperator>
	{
	public:
		// Make the constructor explicit, so that we don't incur into
		// funny implicit conversion.
		explicit StrongPtr(T* ipData = NULL) throw():
			StrongBase<T*,DeleteOperator>(ipData)
		{
		}

#if defined(__IBMCPP__) || defined(__GNUG__) || defined(__hpux)
		// VisualAge compiler doesn't resolve copy-constructors with non-const
		// arguments. So we have to make the argument const and make a const
		// cast inside.
		template <class T2>
		explicit StrongPtr(const ReturnPtr<T2, DeleteOperator>& irReturnPtr) throw()
		{
			// "steal" the ownership from the return ptr
			(this->mData) = const_cast<ReturnPtr<T2, DeleteOperator>&>(irReturnPtr).GiveUp();
		}
#else
		template <class T2>
		explicit StrongPtr(ReturnPtr<T2, DeleteOperator>& irReturnPtr) throw()
		{
			// "steal" the ownership from the return ptr
			this->mData = irReturnPtr.GiveUp();
		}
#endif // __IBMCPP__

#if defined(__IBMCPP__) || defined(__GNUG__) || defined(__hpux)
		// VisualAge compiler doesn't resolve copy-constructors with non-const
		// arguments. So we have to make the argument const and make a const
		// cast inside.
		template <class T2>
		void operator=(const ReturnPtr<T2, DeleteOperator>& irReturnPtr) throw()
		{
			// "steal" the ownership from the return ptr
			Reset(const_cast<ReturnPtr<T2, DeleteOperator>&>(irReturnPtr).GiveUp());
		}
#else
		template <class T2>
		void operator=(ReturnPtr<T2, DeleteOperator>& irReturnPtr) throw()
		{
			// "steal" the ownership from the return ptr
			Reset(irReturnPtr.GiveUp());
		}
#endif // __IBMCPP__

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

		/// We will allow for the & operator for 2 reasons: 
		/// 1. stl relies on the & to actually return the address of the object
		/// but StrongPtr may not be used in stl in the first place.
		/// 2. We will require (by asserting) that the previous value of the pointer
		/// is null. On release mode, this is a leak.
		T** operator&() throw()
		{
			_ASSERTE((this->mData)==NULL);
			return &(this->mData);
		}

		/// Acquire gets the ownership from another strong pointer.
		/// This is destructive copy and we require the user to be aware of it
		/// by calling acquire explicitly. Acquire is a template function to allow
		/// implicit upcast.
		template<class U,class DeleteOperatorU>
		void Acquire(StrongPtr<U, DeleteOperatorU>& iStrongPtr) throw()
		{
			Reset(iStrongPtr.GiveUp());
		}

		/// Acquire with a raw pointer as a parameter will do a reset plus ground the
		/// raw incoming pointer
		void Acquire(T*& irpData) throw()
		{
			this->Reset(irpData);
			irpData=NULL;
		}

		/// Swap the contents of two strong pointers
		template<class U,class DeleteOperatorU>
		void Swap(StrongPtr<U, DeleteOperatorU>& iStrongPtr) throw()
		{
			swap((this->mData), iStrongPtr.mData);
		}

		ReturnPtr<T, DeleteOperator> Return()
		{
			return ReturnPtr<T, DeleteOperator>(this->GiveUp());
		}
	};

	///////////////////////////////////////////////
	// StrongArrayPtr is a specialization of StrongPtr for use with C++ arrays.
	template<class T>
	class StrongArrayPtr: 
		public StrongPtr< T, DeleteArray<T> >
	{
	public:
		explicit StrongArrayPtr(T* ipData = NULL) throw():
			StrongPtr<T, DeleteArray<T> >(ipData) 
		{
		}

		T& operator[](Int32 i) const
		{
			_ASSERTE((this->mData)!=NULL); 
			return (this->mData)[i];
		}
	};


	///////////////////////////////////////////////
	//xhan 09/25/05 
	// StrongBufferPtr is a specialization of StrongPtr for the object allocated from a buffer.
	template<class T>
	class StrongBufferPtr: 
		public StrongPtr< T, Destroy<T> >
	{
	public:
		explicit StrongBufferPtr(T* ipData = NULL) throw():
			StrongPtr<T, Destroy<T> >(ipData) 
		{
		}
	};

	
}

#endif // MBase_StrongPtr_h
