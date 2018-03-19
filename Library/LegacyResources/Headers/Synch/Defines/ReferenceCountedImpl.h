//==============================================================================================
//	FILENAME	:	ReferenceCountedImpl.h
//	AUTHOR		:	vovechkin
//	CREATION	:	Dec 13, 2001
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_ReferenceCountedImpl_h
#define MSynch_ReferenceCountedImpl_h

#include "Base/Defines/Asserte.h"
#include "Synch/Synch/AtomicLong.h"
#include "Base/Interfaces/ReferenceCounted.h"

#ifdef _MSC_VER
#pragma warning(disable:4250) // 'class1' : inherits 'class2::member' via dominance
#endif

namespace MSynch
{
	/**
	 * Derive your class from MBase::ReferenceCounted to get the standard interface
	 * of reference counting. Ex:
	 *
	 *	class MyInterface: 
	 *		virtual public MBase::ReferenceCounted
	 *	{
	 *		...
	 *	};
	 *
	 * Note: Inheritance has to be virtual!
	 *
	 * Then, you get the reference counted implementation by inheriting from 
	 * MSynch::ReferenceCountedImpl. Ex:
	 *
	 *	class MyInterfaceImpl: 
	 *		public MyInterface,
	 *		public MSynch::ReferenceCountedImpl<>
	 *	{
	 *		...
	 *	};
	 * 
	 */
	template< class T = AtomicLong >
	class ReferenceCountedImpl:
		virtual public MBase::ReferenceCounted
	{
	public:
		// we have to initialize to 0 because class T may be 
		// a built-in type, without a constructor
		ReferenceCountedImpl():
			mnReferenceCounter(0)
		{
		}

		/// @see MBase::ReferenceCounted::AddRef
		virtual void AddRef() const throw()
		{
			++mnReferenceCounter;

			_ASSERTE(mnReferenceCounter > 0 && mnReferenceCounter < 100000);
		}

		/// @see MBase::ReferenceCounted::Release
		virtual void Release() const throw()
		{
			// reference counter is corrupted
			_ASSERTE(mnReferenceCounter > 0 && mnReferenceCounter < 100000);

			if (mnReferenceCounter == 1)
			{
				// At this time we know that we are about to release 
				// the last strong reference to this object.

				// But before we do, we notify the object via a virtual
				// method, so that the object has a chance to drop its 
				// weak references (if any).
				ReferenceCountIsAboutToReachZero();
			}

			if (--mnReferenceCounter == 0)
			{
				// that was the last reference
				delete this;
			}
		}

		/**
		 * This method should be overridden by an application object,
		 * which may be in a circular loop with weak references.
		 */
		virtual void ReferenceCountIsAboutToReachZero() const throw()
		{
		}

		inline const T& GetReferenceCount() const throw()
		{
			return mnReferenceCounter;
		}

	private:
		mutable T mnReferenceCounter;
	};
}

#endif // MSynch_ReferenceCountedImpl_h
