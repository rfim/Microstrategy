//==============================================================================================
//	FILENAME	:	SpinLock.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-10-22
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MBase_SpinLock_h
#define MBase_SpinLock_h

#include "../Base/Atomic.h"

#if defined(linux)
#include <sched.h> // for ::sched_yield
#endif
#include "../../PDCHeader/PDCunistd.h" // for ::sleep

namespace MBase
{
	/**
	 * SpinLock implements a busy/lazy/unfair synchronization mechanism, so avoid it.
	 */
	class SpinLock
	{
	public:

		SpinLock()
		{
			// 2004-10-22 vovechkin:
			// it is important that the constructor be empty,
			// because SpinLock is used by ModuleMainPtr, which
			// has to have an empty C++ constructor.
		}

		class SmartLock
		{
		public:

			SmartLock(SpinLock& irSpinLock):
				mrSpinLock(irSpinLock),
				mIsLocked(true)
			{
				mrSpinLock.Lock();
			}

			~SmartLock() throw()
			{
				if (mIsLocked)
				{
					mrSpinLock.Unlock();
				}
			}

			void Unlock() throw()
			{
				if (mIsLocked)
				{
					mrSpinLock.Unlock();
					mIsLocked = false;
				}
			}

		private:

			SpinLock& mrSpinLock;
			bool mIsLocked;
		};

	private:

		friend class SmartLock;

		void Lock() throw()
		{
			// 2004-10-22 vovechkin
			// since the constructor is empty (on purpose)
			// we use 0x12345678 to differentiate from any
			// other (uninitialized) value.
			while (MBase::AtomicExchange(mValue, 0x12345678) == 0x12345678)
			{
				// 2004-10-23 vovechkin
				// it would be better to use MBase::YieldToAnotherThread
				// however, that would bring a link-dependency on Base,
				// which many projects do not have.
#ifdef WIN32
				::Sleep(0);
#elif defined(linux)
				// on Linux, sleep(0) does not do anything
				::sched_yield();
#else
				::sleep(0);
#endif
			}
		}

		void Unlock() throw()
		{
			MBase::AtomicExchange(mValue, 0);
		}

	private:

		long mValue;
	};
}

#endif // MBase_SpinLock_h
