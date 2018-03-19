//==============================================================================================
//	FILENAME	:	ReturnPtrI.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ReturnPtrI_h
#define MBase_ReturnPtrI_h

#ifdef _MSC_VER
#pragma warning(disable:4786) // 'identifier' : identifier was truncated to 'number' characters in the debug information
#pragma warning(disable:4284) // return type for 'identifier::operator ->' is not a UDT or reference to a UDT. Will produce errors if applied using infix notation
#endif

#include "Asserte.h"
#include "Base/Defines/Null.h"

namespace MBase
{
	template<class ReferenceCountedT>
class SmartPtrI;
	/**
	 *	Use ReturnPtrI to return a pointer normally held by a SmartPtrI.
	 *	When returning a reference counted object, we must ensure that a reference
	 *	is valid when coming out of the call. The way COM does that is that by convention
	 *	the callee increments the ref count. This approach has a big problem: when constructing
	 *	a SmartPtrI from a raw pointer, we don't know if the ref count is already incremented
	 *	or not. new, for example, returns a ptr where the ref count has not been incremented.
	 *	When assinging a ptr (that was probably sent in as an input param) to a member SmartPtrI,
	 *	the ref count has not been incremented either.
	 *	COM handles this by returning raw ptrs always by reference in output params.
	 *	We want to be able to return them as return values.
	 *	We can also return a SmartPtrI itself, but that implies incrementing the ref count
	 *	twice for each Function call (incrementing and decrementing a ref count may lock).
	 *
	 *	To return a pointer to a reference counted object using ReturnPtrI, simply declare your
	 *	function as returning a ReturnPtrI. ( ReturnPtrI<T> Function(); ).
	 *	Within the function, use SmartPtrI to hold your pointers. When returning simply return
	 *	the SmartPtrI and the casting (and reference count managing) will be done implicitly.
	 *	When calling a function that returns a ReturnPtrI, use a SmartPtrI to receive the ptr.
	 *	( SmartPtrI<T> lPtr(Function()); )
	 *	A SmartPtrI will construct itself based on the ReturnPtrI and again handle the reference
	 *	counting properly.
	 */
	template<class ReferenceCountedT>
	class ReturnPtrI
	{
	public:
		/// Constructor from raw pointer
		explicit ReturnPtrI(ReferenceCountedT* ipObject)
		{
			// implicit upcast should happen automatically
			mpObject=ipObject;
			if (mpObject)
			{
				mpObject->AddRef();
			}
		}

#if defined(__IBMCPP__) || defined(__GNUG__) || defined(__hpux) || defined(sun)
		// VisualAge compiler doesn't resolve copy-constructors with non-const
		// arguments. So we have to make the argument const and make a const
		// cast inside.
		ReturnPtrI(const ReturnPtrI& irReturnPtr)
		{
			mpObject = const_cast<ReturnPtrI<ReferenceCountedT>&>(irReturnPtr).GiveUp();
		}
#else
		ReturnPtrI(ReturnPtrI &irReturnPtr)
		{
			mpObject = irReturnPtr.GiveUp();
		}
		ReturnPtrI(SmartPtrI<ReferenceCountedT> &irReturnPtr)
		{
			mpObject = irReturnPtr.GiveUp();
		}
#endif

		/// This function should only be used by SmartPtrI
		ReferenceCountedT* GiveUp() throw()
		{
			// give away reference
			ReferenceCountedT* lpObject=mpObject;
			mpObject=NULL;
			return lpObject;
		}
		~ReturnPtrI() throw()
		{
			// should have acquired this with a SmartPtrI!!
			// somebody ignored the returning pointer!

			// 2002-02-25 vovechkin: it's okay to not assign to a smart pointer,
			// specifically if you are only invoking a method on the returned value.
			//		_ASSERTE(!mpObject);

			if(mpObject)
			{
				// if somebody ignored the returning pointer, release it
				mpObject->Release();
			}
		}

		ReferenceCountedT* operator->() const throw()
		{
			_ASSERTE( mpObject != NULL );
			return mpObject;
		}

		ReferenceCountedT& operator*() const throw()
		{
			return *(operator->());
		}

		inline ReferenceCountedT* Get() const
		{
			return mpObject;
		}

	private:
		ReturnPtrI() // no default constructor allowed
		{
		};

		ReferenceCountedT* mpObject;		// pointer to underlying object
	};
}

#endif // MBase_ReturnPtrI_h
