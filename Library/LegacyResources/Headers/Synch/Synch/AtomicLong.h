//==============================================================================================
//	FILENAME	:	AtomicLong.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_AtomicLong_h
#define MSynch_AtomicLong_h

#include "Base/Defines/BasicTypes.h"
#include "Synch.h"
//#ifdef __hpux
//#define HPUX32
//#undef HPUX64
//#endif

#ifdef _AIX
#include <sys/atomic_op.h>
#elif defined(sun) || defined(linux)
/*extern "C"
{
	long MSynch_AtomicIncrement(long *lp);
	long MSynch_AtomicDecrement(long *lp);
	long MSynch_AtomicSet(long *lp, long newVal);
	long MSynch_AtomicAdd(long *lp, long inValue);
}*/
#elif defined(__hpux)
	// To compile ExecLibRubyBridge on HP-UX PA-RISC machine There is no such
	// assembly defined functions. -- hzhang 12/22/2005
	#if #cpu(parisc)

	#else
	extern "C"
	{
		long MSynch_AtomicIncrement(long *lp);
		long MSynch_AtomicDecrement(long *lp);
		long MSynch_AtomicSet(long *lp, long newVal);
		long MSynch_AtomicAdd(long *lp, long inValue);
	}
	#endif //__hpux
#elif defined(WIN32)
	// do nothing
#else
#include "ProtectedSource/InprocessRecursiveMutex.h"
#endif

#ifdef __hpux
#include "PrivateSource/AtomicLongData.h"
#endif

namespace MSynch
{
// To compile ExecLibRubyBridge on HP-UX PA-RISC machine We have
// to distinguish between IA and PA machines. But Windows preprocessing
// do not accept #cpu(parisc) so we have to seperate tehm. -- hzhang 12/22/2005
#if defined(WIN32)
	// do nothing
#elif  defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/)
	// do nothing
#else
	//class AtomicLongData;
#endif

	/**	AtomicLong behaves similarly to a C++ long for incrementing and
		decrementing its value, with the additional benifit of being
		thread safe without the user needing to enclose it inside
		synchronization blocks.

		Objects created from this class may NOT cross Constellation boundaries
	*/
	class DLL_SYNCH_EXIM AtomicLong
	{
	public:
	
#if defined(WIN32)
		AtomicLong(const Int32 inValue = 0){ mnAtomicLongData = 0;}
		AtomicLong(const AtomicLong& irAtomicLong){ mnAtomicLongData = irAtomicLong.mnAtomicLongData;}
#elif defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) // HP-UX only works on IA machine -hzhang
		AtomicLong(const Int32 inValue = 0){ mnAtomicLongData = 0;}
		AtomicLong(const AtomicLong& irAtomicLong){ mnAtomicLongData = irAtomicLong.mnAtomicLongData;}
#else		
		/// default initialization value is zero
		AtomicLong(const Int32 inValue = 0){ mAtomicLongData = 0;}
		AtomicLong(const AtomicLong& irAtomicLong){ mAtomicLongData = irAtomicLong.mAtomicLongData;}
#endif
		~AtomicLong() throw(){}

		// pre-increment (++i)
		// IMPORTANT: operator++ will return to you a copy (a long) of the incremented value
		// this is thread safe, otherwise, you would increment the value, get pre-empted and then
		// extract a value that has been modified by other thread
		// you may not do ++(++i), it will not compile
// change to inline for unix platforms
#ifdef WIN32
		Int32 operator++() throw();
#else
		Int32 operator++() throw()
		{
		#ifdef _AIX
			//AIX system call returns original value, need to fix that.
			return ::fetch_and_addlp(&mnAtomicLongData, 1)+1;

		#elif defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) // HP-UX only works on IA machine -hzhang
			//return MSynch_AtomicIncrement(&mnAtomicLongData);
			return __sync_fetch_and_add(&mnAtomicLongData, 1)+1;
		#else
			return ++(mAtomicLongData);
		#endif
		}
#endif

		// Atomic addition of an increment value
		// Return value is the initial value
// change to inline for unix platforms
#ifdef WIN32
		Int32 operator+=(Int32 inValue) throw();
#else
		Int32 operator+=(Int32 inValue) throw()
		{
		#ifdef _AIX
			return ::fetch_and_addlp(&mnAtomicLongData, inValue);

		#elif defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) // HP-UX only works on IA machine -hzhang
			return __sync_fetch_and_add(&mnAtomicLongData, inValue)+inValue;
		#else
			return (mAtomicLongData)++;
		#endif
		}
#endif

		// pre-decrement (--i)
		// IMPORTANT: operator-- will return to you a copy (a long) of the decremented value
		// this is thread safe, otherwise, you would decrement the value, get pre-empted and then
		// extract a value that has been modified by other thread
		// you may not do --(--i), it will not compile
// change to inline for unix platforms
#ifdef WIN32
		Int32 operator--() throw();
#else
		Int32 operator--() throw()
		{
		#ifdef _AIX
			//AIX system call returns original value, need to fix that.
			return ::fetch_and_addlp(&mnAtomicLongData, -1)-1;

		#elif defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) // HP-UX only works on IA machine -hzhang
			//return MSynch_AtomicDecrement(&mnAtomicLongData);
			return __sync_fetch_and_sub(&mnAtomicLongData, 1)-1;

		#else
			return --(mAtomicLongData);
		#endif
		}
#endif

		operator Int32() const throw()
		{
		// To compile ExecLibRubyBridge on HP-UX PA-RISC machine We have
		// to distinguish between IA and PA machines. But Windows preprocessing
		// does not accept #cpu(parisc) so we have to seperate tehm. -- hzhang 12/22/2005
		#if defined(WIN32) /* need read_lock it ?  */
			return mnAtomicLongData;
		#elif defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) /* need read_lock it ?  */
			return mnAtomicLongData;
		#else
			return mAtomicLongData;
		#endif
		}

		Int32 GetValue() const throw()
		{
		// To compile ExecLibRubyBridge on HP-UX PA-RISC machine We have
		// to distinguish between IA and PA machines. But Windows preprocessing
		// do not accept #cpu(parisc) so we have to seperate tehm. -- hzhang 12/22/2005
		#if defined(WIN32) /* need read_lock it ?  */
			return mnAtomicLongData;
		#elif defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) /* need read_lock it ?  */
			return mnAtomicLongData;
		#else
			return mAtomicLongData;
		#endif
		}

#if defined(WIN32)
		void operator=(const Int32 inValue){mnAtomicLongData = 0;}
		void operator=(const AtomicLong& irAtomicLong){ mnAtomicLongData = irAtomicLong.mnAtomicLongData;}
#elif defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/)
		void operator=(const Int32 inValue){mnAtomicLongData = 0;}
		void operator=(const AtomicLong& irAtomicLong){ mnAtomicLongData = irAtomicLong.mnAtomicLongData;}
#else
		// WARNING Assignment operator to atomic long is never thread-safe.
		// It is meant only for (single-threaded) initialization.
		// Caller of atomic long must ensure that this call is only used when that atomic
		// long can only be accessed by a single thread.
		void operator=(const Int32 inValue){mAtomicLongData = 0;}

		// WARNING Assignment operator to atomic long is never thread-safe.
		// It is meant only for (single-threaded) initialization.
		// Caller of atomic long must ensure that this call is only used when that atomic
		// long can only be accessed by a single thread.
		void operator=(const AtomicLong& irAtomicLong){ mAtomicLongData = irAtomicLong.mAtomicLongData;}
		// Exchange sets the inValue as the value of the AtomicLong and returns
		// the PREVIOUS value held by the AtomicLong.
#endif
		Int32 Exchange(Int32 inValue)
		{
		#ifdef WIN32
			return ::InterlockedExchange(&mnAtomicLongData,inValue);

		#elif defined(_AIX)
			long retVal = mnAtomicLongData;
			while(FALSE == ::compare_and_swaplp(&mnAtomicLongData, &retVal, inValue))
			{
				// do nothing because compare_and_swaplp
				// will update retVal to the new value
			}
			return retVal;

		#elif defined(sun) || defined(linux) || (defined(__hpux)) /* && !#cpu(parisc))
            HP-UX only wors on IA machine -hzhang */
			//return MSynch_AtomicSet(&mnAtomicLongData,inValue);
			return __sync_lock_test_and_set(&mnAtomicLongData,inValue); 
        #elif defined(_MAC)
            _ASSERT(false);
            return 0;
		#else
			{
				InprocessRecursiveMutex::SmartLock lLock(mMutex);
				long lPreviousValue = mAtomicLongData;
				mAtomicLongData = inValue;
				return lPreviousValue;
			}
		#endif
		}

	private:
// To compile ExecLibRubyBridge on HP-UX PA-RISC machine We have
// to distinguish between IA and PA machines. But Windows preprocessing
// does not accept #cpu(parisc) so we have to seperate tehm. -- hzhang 12/22/2005
#if defined(WIN32)
		long mnAtomicLongData;
#elif defined(_AIX) || defined(sun) || defined(linux) || (defined(__hpux) /*&& !#cpu(parisc)*/) // HP-UX only works on IA machine -hzhang
		long mnAtomicLongData;
#elif defined(_MAC)
        Int32 mAtomicLongData;
#else
		Int32 mAtomicLongData;
		mutable InprocessRecursiveMutex mMutex;
#endif
	};
}

#endif // MSynch_AtomicLong_h
