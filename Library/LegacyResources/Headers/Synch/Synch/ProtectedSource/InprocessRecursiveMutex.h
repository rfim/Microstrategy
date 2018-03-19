//==============================================================================================
//	FILENAME	:	InprocessRecursiveMutex.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-06-11
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MSynch_InprocessRecursiveMutex_h
#define MSynch_InprocessRecursiveMutex_h

#include "PDCHeader/PDCerrno.h"
#include "PDCHeader/PDCpthread.h"
#include "Base/Defines/Asserte.h"
#if defined(_NO_EXCEPTION)
#include <stdio.h>
#endif

namespace MSynch
{
	/**
	 * This class implements a basic critical section (mutex) that 
	 * can be used to synchronize among threads of a single process.
	 *
	 * The methods that can throw, may throw MBase::BaseException.
	 */
	class InprocessRecursiveMutex
	{
	public:
		InprocessRecursiveMutex();
		inline ~InprocessRecursiveMutex() throw();

		class SmartLock
		{
		public:

			inline SmartLock(InprocessRecursiveMutex& irMutex);
			inline ~SmartLock() throw();

			void Lock()
			{
				mrMyMutex.Lock();
				_ASSERT(mIsLocked == false);
				mIsLocked = true;
			}

			void Unlock() throw()
			{
				_ASSERT(mIsLocked == true);
				mIsLocked = false;
				mrMyMutex.Unlock();
			}

#ifndef WIN32
			/// this method is here only for the ManualEvent
			inline void WaitUntilSpuriouslyWokenUp(
				pthread_cond_t& irConditionalVariable) const;

			/// this method is here only for the ManualEvent
			/// @return false if the timeout occurred
			inline bool WaitUntilSpuriouslyWokenUpOrTimeoutExpired(
				pthread_cond_t& irConditionalVariable,
				const struct timespec& irAbsoluteTimeout) const;
#endif // WIN32

		private:

			bool mIsLocked;
			InprocessRecursiveMutex& mrMyMutex;
		};

	public:
		inline void Lock();
		inline bool Lock_NoWait();
		inline void Unlock() throw();

	private:
		friend class CriticalSectionImpl;
		friend class SmartLock;
		friend class BootstrapCriticalSectionImpl;

		pthread_mutex_t mMutex;
	};
}

///////////////////////////////////////////////////////////////////////////////
//
// MSynch::InprocessRecursiveMutex
//
///////////////////////////////////////////////////////////////////////////////

inline MSynch::InprocessRecursiveMutex::~InprocessRecursiveMutex() throw()
{
	const int lResult  = ::pthread_mutex_destroy(&mMutex);

	// can't throw
	_ASSERT(lResult == 0);

}

inline void MSynch::InprocessRecursiveMutex::Lock()
{
	const int lResult = ::pthread_mutex_lock(&mMutex);

	if (lResult != 0)
	{
		// pthread_mutex_lock will fail only if something
		// is seriously wrong, such as:
		// 1. this pointer is wrong
		// 2. mMutex is corrupt
		// 3. mMutex is not recursive
		_ASSERT(false);
#if !defined(_NO_EXCEPTION)
		throw ("::pthread_mutex_lock failed");
#else
		printf("%s\n","MSynch::InprocessRecursiveMutex::Lock: ::pthread_mutex_lock failed");
		return;
#endif		
	}

}

inline bool MSynch::InprocessRecursiveMutex::Lock_NoWait()
{
	const int lResult = ::pthread_mutex_trylock(&mMutex);

	if (lResult == EBUSY)
	{
		return false;
	}
	
	if (lResult != 0)
	{
		// pthread_mutex_lock will fail only if something
		// is seriously wrong, such as:
		// 1. this pointer is wrong
		// 2. mMutex is corrupt
		// 3. mMutex is not recursive
		_ASSERT(false);
#if !defined(_NO_EXCEPTION)
		throw ("::pthread_mutex_trylock failed");
#else
		printf("%s\n","MSynch::InprocessRecursiveMutex::Lock_NoWait: ::pthread_mutex_trylock failed");
		return false;
#endif	
	}
	
	return true;
}

inline void MSynch::InprocessRecursiveMutex::Unlock() throw()
{

	const int lResult = ::pthread_mutex_unlock(&mMutex);
	
	// this assertion may fail if:
	// 1. the mMutex is corrupt, or
	// 2. a previous call to pthread_mutex_lock failed
	// both of which are impossible, unless 
	// something bad like memory corruption occurs.
	_ASSERT(lResult == 0);
}

///////////////////////////////////////////////////////////////////////////////
//
// MSynch::InprocessRecursiveMutex::SmartLock
//
///////////////////////////////////////////////////////////////////////////////
inline MSynch::InprocessRecursiveMutex::SmartLock::SmartLock(InprocessRecursiveMutex& irMutex):
	mrMyMutex(irMutex),
	mIsLocked(false)
{
	mrMyMutex.Lock();
	mIsLocked = true;
}

inline MSynch::InprocessRecursiveMutex::SmartLock::~SmartLock() throw()
{
	if (mIsLocked)
	{
		mrMyMutex.Unlock();
	}
}

#ifndef WIN32
inline void MSynch::InprocessRecursiveMutex::SmartLock::WaitUntilSpuriouslyWokenUp(
	pthread_cond_t& irConditionalVariable) const
{
	const int lResult = ::pthread_cond_wait(&irConditionalVariable, &mrMyMutex.mMutex);

	if (lResult != 0)
	{
#if !defined(_NO_EXCEPTION)
		throw ("::pthread_cond_wait failed");
#else
		printf("%s\n","MSynch::InprocessRecursiveMutex::SmartLock::WaitUntilSpuriouslyWokenUp: ::pthread_cond_wait failed");
		return;
#endif	
	}
}

inline bool MSynch::InprocessRecursiveMutex::SmartLock::WaitUntilSpuriouslyWokenUpOrTimeoutExpired(
	pthread_cond_t& irConditionalVariable,
	const struct timespec& irAbsoluteTimeout) const
{
	const int lResult = ::pthread_cond_timedwait(&irConditionalVariable, &mrMyMutex.mMutex, &irAbsoluteTimeout);

	if (lResult == ETIMEDOUT)
	{
		return false;
	}
	else if (lResult != 0)
	{
#if !defined(_NO_EXCEPTION)
		throw ("::pthread_cond_timedwait failed");
#else
		printf("%s\n","MSynch::InprocessRecursiveMutex::SmartLock::WaitUntilSpuriouslyWokenUpOrTimeoutExpired: ::pthread_cond_timedwait failed");
		return false;
#endif	
	}

	return true;
}
#endif // WIN32

#endif // MSynch_InprocessRecursiveMutex_h
