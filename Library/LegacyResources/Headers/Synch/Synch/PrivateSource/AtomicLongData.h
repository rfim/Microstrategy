//==============================================================================================
//	FILENAME	:	AtomicLongData.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	2001-09-26
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_AtomicLongData_h
#define MSynch_AtomicLongData_h

#include "Base/Defines/BasicTypes.h"
#include "../ProtectedSource/InprocessRecursiveMutex.h"

namespace MSynch
{
	// This class is all inlined because
	// 1. It is not exported
	// 2. It is called very, very frequently and we need to speed it up
	class AtomicLongData
	{
	public:
		AtomicLongData(const Int32 inValue = 0):
			mValue(inValue)
		{
		}

		AtomicLongData(const AtomicLongData& irAtomicLong):
			mValue(irAtomicLong.mValue)
		{
		}

		Int32 operator++() throw()
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			return ++mValue;
		}

		Int32 AddAndReturnOld(Int32 iValue) throw()
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			Int32 lRes = mValue;
			mValue += iValue;
			return lRes;
		}


		Int32 operator--() throw()
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			return --mValue;
		}

		Int32 GetValue() const throw()
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			return mValue;
		}

		// WARNING Assignment operator to atomic long is never thread-safe.
		// It is meant only for (single-threaded) initialization.
		// Caller of atomic long must ensure that this call is only used when that atomic
		// long can only be accessed by a single thread.
		void operator=(const Int32 inValue)
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			mValue=inValue;
		}

		long Exchange(long inValue)
		{
			InprocessRecursiveMutex::SmartLock lLock(mMutex);
			long lPreviousValue = mValue;
			mValue = inValue;
			return lPreviousValue;
		}

	private:
		// disallow assignment operator
		// assignment is never thread safe
		Int32 operator=(AtomicLongData& irAtomicLongData);

		mutable InprocessRecursiveMutex mMutex;
		Int32 mValue;
	};
}

#endif // MSynch_AtomicLongData_h

