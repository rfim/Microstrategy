//==============================================================================================
//	FILENAME	:	InprocessSemaphore.h
//	AUTHOR		:	yuwen
//	CREATION	:	2004-06-17
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MSynch_InprocessSemaphore_h
#define MSynch_InprocessSemaphore_h

#include "PDCHeader/PDCpthread.h"
#include "AbsoluteTimeout.h"
#include "InprocessRecursiveMutex.h"

#include "../../Defines/SmartIncrementor.h"

namespace MSynch
{
	class InprocessSemaphore
	{
	public:
		// @param ipname: Used for named semaphore. It is not supported/used here.
		// @param p: Argument to be passed in. It is not supported/used here. The SemaphoreImpl will simply
		//			 pass NULL to p.
		inline InprocessSemaphore(unsigned Int32 iInitialAvailableResourceCount,
								  unsigned Int32 iMaximumResourceCount = 0x7fffffff);
					
		inline ~InprocessSemaphore() throw();

		inline bool WaitForResource(unsigned Int32 iMilliseconds);
		inline void WaitForeverForResource();
		inline void ReleaseResource();

	private:

		InprocessRecursiveMutex mMutex;
		pthread_cond_t mConditionalVariable;

		const unsigned Int32 mMaxCount;

		unsigned Int32 mCount;
		unsigned Int32 mWaiters;
	};
}

///////////////////////////////////////////////////////////////////////////////
//
// MSynch::InprocessSemaphore
//
///////////////////////////////////////////////////////////////////////////////

inline MSynch::InprocessSemaphore::InprocessSemaphore(unsigned Int32 iInitialAvailableResourceCount, unsigned Int32 iMaximumResourceCount) :

	mMaxCount(iMaximumResourceCount),
	mCount(iInitialAvailableResourceCount), 
	mWaiters(0)

	{

		const int lResult = ::pthread_cond_init(&mConditionalVariable, NULL);

		if (lResult != 0)
		{
			throw ("::pthread_cond_init failed");
		}
	}

inline MSynch::InprocessSemaphore::~InprocessSemaphore() throw()
{
#ifndef WIN32
	const int lResult = ::pthread_cond_destroy(&mConditionalVariable);
	_ASSERT(lResult == 0);
#endif
}

inline bool MSynch::InprocessSemaphore::WaitForResource(unsigned Int32 iMilliseconds)
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	// Keep track of the number of waiters so that we can signal them
	SmartIncrementor<unsigned Int32> lInc(mWaiters);

	struct timespec lAbsoluteTimeout;
	GetAbsoluteTimeout(lAbsoluteTimeout, iMilliseconds);

	while (mCount == 0)
	{
		if (lLock.WaitUntilSpuriouslyWokenUpOrTimeoutExpired(mConditionalVariable, lAbsoluteTimeout) == false)
		{
			return false; //time out
		}
	}
		
	_ASSERT(mCount > 0);
	--mCount;

	return true; //success

}


inline void MSynch::InprocessSemaphore::WaitForeverForResource()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);
	
	SmartIncrementor<unsigned Int32> lInc(mWaiters);

	while (mCount == 0)
	{
		lLock.WaitUntilSpuriouslyWokenUp(mConditionalVariable);
	}

	_ASSERT(mCount > 0);
	--mCount;
}

inline void MSynch::InprocessSemaphore::ReleaseResource()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	_ASSERT(mCount <= mMaxCount);
	if (mCount == mMaxCount)
	{
		throw ("MSynch::InprocessSemaphore::ReleaseResource: exceeded the maximum.");
	}

	// Always allow a waiter to continue if there is one.
	if (mWaiters > 0)
	{
		const int lResult = ::pthread_cond_signal(&mConditionalVariable);
		// On success, pthread_cond_signal() returns 0
		if (lResult != 0)
		{
			throw ("::pthread_cond_signal failed");
		}
	}

	++mCount;
}

#endif // MSynch_InprocessSemaphore_h
