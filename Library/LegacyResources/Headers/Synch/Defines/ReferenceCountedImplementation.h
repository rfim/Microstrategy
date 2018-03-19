//==============================================================================================
//	FILENAME	:	ReferenceCountedImplementation.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_ReferenceCountedImplementation_h
#define MSynch_ReferenceCountedImplementation_h

#include "Base/Defines/Asserte.h"
#include "Synch/Synch/AtomicLong.h"

#define REFERENCE_COUNTED_INITIALIZATION mnReferenceCount(0)

/*
 * Derive your class from MBase::ReferenceCounted to get the standard interface
 * of reference counting. You may get the reference counted implementation
 * by inserting the REFERENCE_COUNTED_IMPLEMENTATION macro on your implementation
 * class.
 */
#define REFERENCE_COUNTED_IMPLEMENTATION \
	public: \
		virtual void AddRef() const throw() \
		{ \
			++mnReferenceCount; \
		} \
		virtual void Release() const throw() \
		{ \
			/* Reference count is corrupted */ \
			_ASSERTE(mnReferenceCount>0);		\
			if ((--mnReferenceCount) == 0) \
			{ \
				/* That was the last reference */ \
				delete this; \
			} \
		} \
	private: \
		mutable MSynch::AtomicLong mnReferenceCount;

#endif // MSynch_ReferenceCountedImplementation_h
