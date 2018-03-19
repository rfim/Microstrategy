//==============================================================================================
//	FILENAME	:	ReturnPtr.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2003-03-07
//	Copyright (C) MicroStrategy Incorporated 2003
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ReturnPtr_h
#define MBase_ReturnPtr_h

#include "Asserte.h"
#include "Null.h"
#include "DeleteOperator.h"

namespace MBase
{
	/**
	 * Use ReturnPtr to return a pointer normally held by a StrongPtr in a type-safe
	 * and exception-safe manner.
	 */
	template <class T, class DeleteOperator = DeleteC<T> >
	class ReturnPtr
	{
	public:
		/// Constructor from raw pointer
		ReturnPtr(T* ipObject):
			mpObject(ipObject)
		{
		}

#if defined(__IBMCPP__) || defined(__GNUG__) || defined(__hpux) || defined(sun)
		// VisualAge compiler doesn't resolve copy-constructors with non-const
		// arguments. So we have to make the argument const and make a const
		// cast inside.
		ReturnPtr(const ReturnPtr& irReturnPtr):
			mpObject(const_cast<ReturnPtr&>(irReturnPtr).GiveUp())
		{
		}
#else
		ReturnPtr(ReturnPtr& irReturnPtr):
			mpObject(irReturnPtr.GiveUp())
		{
		}
#endif

		/// This function should only be used by StrongPtr/ReturnPtr
		T* GiveUp() throw()
		{
			// give away reference
			T* lpObject = mpObject;
			mpObject = NULL;
			return lpObject;
		}

		~ReturnPtr() throw()
		{
			if(mpObject != NULL)
			{
				// if somebody ignored the returning pointer, delete it
				DeleteOperator()(mpObject);
				mpObject = NULL;
			}
		}

		T* operator->() const throw()
		{
			_ASSERTE(mpObject != NULL);
			return mpObject;
		}

		T& operator*() const throw()
		{
			return *(operator->());
		}

		inline T* Get() const
		{
			return mpObject;
		}

		bool IsNull() const
		{
			return (mpObject == NULL);
		}

	private:
		ReturnPtr() // no default constructor allowed
		{
		};

		T* mpObject;		// pointer to underlying object
	};
}

#endif // MBase_ReturnPtr_h
