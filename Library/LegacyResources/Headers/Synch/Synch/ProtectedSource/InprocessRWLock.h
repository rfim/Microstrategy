//==============================================================================================
//	FILENAME	:	InprocessRWLock.h
//	AUTHOR		:	Yulin Wen
//	CREATION	:	2004-06-19
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MSynch_InprocessRWLock_h
#define MSynch_InprocessRWLock_h

#include "Base/Defines/ExceptionImpl.h"

#ifdef WIN32
#include "PDCHeader/PDCwindows.h"
#include "Base/Defines/WindowsStrongObjects.h"
#else
#include "PDCHeader/PDCpthread.h"
#endif

#include "InprocessRecursiveMutex.h"
#include "../../Defines/SmartIncrementor.h"

namespace MSynch
{
	class InprocessRWLock
	{
	public:
		inline InprocessRWLock();
		inline ~InprocessRWLock() throw();

		inline void ReadLock();
		inline bool ReadLock_NoWait();

		inline void WriteLock();
		inline bool WriteLock_NoWait();

		inline void Release() throw();

	private:

		// -1 == one writer
		//  0 == no reader/writer
		// >0 == number of readers
		Int32 mRefCount;
		
		unsigned Int32 mNumWaitingWriter;
		unsigned Int32 mNumWaitingReader;

		InprocessRecursiveMutex mMutex;

#ifdef WIN32
		MBase::StrongHandle mhReaderME;
		MBase::StrongHandle mhWriterAE;	
#else
		pthread_cond_t mWaitingReaderCV;
		pthread_cond_t mWaitingWriterCV;
#endif
	};
}

///////////////////////////////////////////////////////////////////////////////
//
// MSynch::InprocessRWLock
//
///////////////////////////////////////////////////////////////////////////////

inline MSynch::InprocessRWLock::InprocessRWLock(): 
#ifdef WIN32
	mhReaderME(::CreateEventW(NULL, TRUE, FALSE, NULL)),
	mhWriterAE(::CreateEventW(NULL, FALSE, FALSE, NULL)),
#endif
	mRefCount(0),
	mNumWaitingWriter(0),
	mNumWaitingReader(0)
{
#ifdef WIN32
	if (mhReaderME.Get() == NULL)
	{
		throw MBase::ExceptionImpl<>("::CreateEventW failed", ::GetLastError());
	}

	if (mhWriterAE.Get() == NULL)
	{
		throw MBase::ExceptionImpl<>("::CreateEventW failed", ::GetLastError());
	}
#else
	int lResult = 0;

	lResult = ::pthread_cond_init(&mWaitingReaderCV, NULL);
	if (lResult != 0)
	{
		throw MBase::ExceptionImpl<>("::pthread_cond_init failed", lResult);
	}

	lResult = ::pthread_cond_init(&mWaitingWriterCV, NULL);
	if (lResult != 0)
	{
		throw MBase::ExceptionImpl<>("::pthread_cond_init failed", lResult);
	}
#endif // WIN32
}

inline MSynch::InprocessRWLock::~InprocessRWLock() throw()
{
#ifndef WIN32
	int lResult = ::pthread_cond_destroy(&mWaitingReaderCV);
	_ASSERT(lResult == 0);

	lResult = ::pthread_cond_destroy(&mWaitingWriterCV);
	_ASSERT(lResult == 0);
#endif
}

void MSynch::InprocessRWLock::ReadLock()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	// Give preference to writers who are waiting.
//	while ( (mRefCount < 0) || (mNumWaitingWriter > 0) )
	//kxiao: Remove the writer to avoid deadlock: 
	//		One thread holds a read lock and some other threads are waiting for a write lock
	//		The same thread ask for a read lock again and will cause the dead lock.
	//		We can also save all read lock thread ID to avoid this.
	while ( mRefCount < 0 )
	{
		SmartIncrementor<unsigned Int32> lInc(mNumWaitingReader);
		
#ifdef WIN32
		lLock.Unlock();
		DWORD lResult = ::WaitForSingleObject(mhReaderME.Get(), INFINITE);
		lLock.Lock();

		if (lResult != WAIT_OBJECT_0)
		{
			throw MBase::ExceptionImpl<>("::WaitForSingleObject failed", ::GetLastError());
		}
#else
		lLock.WaitUntilSpuriouslyWokenUp(mWaitingReaderCV);
#endif // WIN32
	}

	++mRefCount;
	_ASSERT(mRefCount > 0);
}

bool MSynch::InprocessRWLock::ReadLock_NoWait()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	// Give preference to writers who are waiting.
	if ((mRefCount < 0) || (mNumWaitingWriter > 0))
	{
		// do not wait
		return false;
	}

	++mRefCount;
	_ASSERT(mRefCount > 0);

	return true;
}

void MSynch::InprocessRWLock::WriteLock()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	while (mRefCount != 0)
	{
		SmartIncrementor<unsigned Int32> lInc(mNumWaitingWriter);

#ifdef WIN32
		// reset the reader event whenever a new writer is 
		// about to get a lock or wait for a lock, because
		// we give priority to waiting writers over new
		// readers coming in.
		if (::ResetEvent(mhReaderME.Get()) == 0)
		{
			throw MBase::ExceptionImpl<>("::ResetEvent failed", ::GetLastError());
		}

		// wait for writer auto event
		lLock.Unlock();
		DWORD lResult = ::WaitForSingleObject(mhWriterAE.Get(), INFINITE);
		lLock.Lock();

		if (lResult != WAIT_OBJECT_0)
		{
			throw MBase::ExceptionImpl<>("::WaitForSingleObject failed", ::GetLastError());
		}
#else
		lLock.WaitUntilSpuriouslyWokenUp(mWaitingWriterCV);
#endif // WIN32
	}

	mRefCount = -1;
}

bool MSynch::InprocessRWLock::WriteLock_NoWait()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	if (mRefCount != 0)
	{
		// no wait
		return false;
	}

	mRefCount = -1;
	return true;
}

void MSynch::InprocessRWLock::Release() throw()
{
	InprocessRecursiveMutex::SmartLock lLock(mMutex);

	if (mRefCount > 0)
	{
		// Releasing a reader.
		--mRefCount;
	}
	else if (mRefCount == -1)
	{
		// Releasing a writer.
		mRefCount = 0;
	}
	else
	{
		// count should not be 0!!!
		_ASSERT(false);
	}
	
	if ((mNumWaitingWriter > 0) && (mRefCount == 0))
	{
		// give preference to writers over readers...
#ifdef WIN32
		const BOOL lResult = ::SetEvent(mhWriterAE.Get());
		_ASSERT(lResult != 0);
#else
		const int lResult = ::pthread_cond_signal(&mWaitingWriterCV);
		_ASSERT(lResult == 0);
#endif
	}
	else if ((mNumWaitingWriter == 0) && (mNumWaitingReader > 0))
	{
#ifdef WIN32
		const BOOL lResult = ::SetEvent(mhReaderME.Get());
		_ASSERT(lResult != 0);
#else
		const int lResult = ::pthread_cond_broadcast(&mWaitingReaderCV);
		_ASSERT(lResult == 0);
#endif
	}
}

#endif // MSynch_InprocessRWLock_h
