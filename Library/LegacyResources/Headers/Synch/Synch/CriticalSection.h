//==============================================================================================
//	FILENAME	:	CriticalSection.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_CriticialSection_h
#define MSynch_CriticialSection_h

#include "Base/Defines/StrongPtr.h"
#include "Base/Defines/NoCopy.h"

#include "Synch.h"
#include "LockCategories.h"

namespace MSynch
{
	/// CriticalSection allows only one thread at a time to pass.
	/** Other threads will wait in an efficient wait state until all other threads are out.
		If more than one thread is waiting on a CriticalSection, it is unpredictable
		which thread will enter first.
		CriticalSection only works within the same process
	*/
	class CriticalSection
	{
	public:

		typedef MBase::StrongPtr<CriticalSection, MBase::DeleteOperatorGeneric<CriticalSection> > Ptr;
		typedef MBase::ReturnPtr<CriticalSection, MBase::DeleteOperatorGeneric<CriticalSection> > ReturnPtr;

		/// SmartLock interface
		/** CriticalSection::SmartLock is intended to be created on the stack
			and automatically unlocks when it goes out of scope.

			Objects creted from this class may NOT cross Constellation boundaries
		*/
		class DLL_SYNCH_EXIM SmartLock:
			public MBase::NoCopy
		{
		public:
			// OPTIMIZATION WARNING! The methods on SmartLock are inlined
			// on purpose for performance reasons.
			SmartLock(CriticalSection& irCriticalSection):
				mrCriticalSection(irCriticalSection),
				mbLocked(true)
			{
				mrCriticalSection.Lock();
			}

			~SmartLock() throw()
			{
				if (mbLocked)
				{
					this->Unlock();
				}
			}

			void Lock()
			{
				_ASSERTE(!mbLocked);
				mrCriticalSection.Lock();
				mbLocked = true;
			}

			void Unlock() throw()
			{
				_ASSERTE(mbLocked);
				mrCriticalSection.Unlock();
				mbLocked = false;
			}

		private:
			//These members may never be changed or added to.
			CriticalSection& mrCriticalSection;
			bool mbLocked;
		};

		/// SetLockCategory for "late category binding"
		/** Needed for "late category binding" used primarily by the COM team.
			Note that once a category is set it cannot be changed.
		*/
		virtual void SetLockCategory(EnumDSSLockStackCategory iLockStackCategory) = 0;

		virtual void Delete() throw() = 0;

	protected:
		friend class SmartLock;		

		/**
		 * Enter a critical section. This is an infinite wait until all other
		 * threads are outside this critical section.
		 * @throw This method does not throw, except for testing purposes, in 
		 * which case it may throw a LockOrderViolationException if a LOV occurs.
		 */
		virtual void Lock() = 0;

		/// leave the critical section, any waiting thread may enter it now
		virtual void Unlock() throw() = 0;	

	public:
		// Pretend these two methods don't exist.
		// They are only for supporting obsolete
		// interfaces that have explicit lock/unlock
		virtual void Exception_unsafe_Lock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead() = 0;
		virtual void Exception_unsafe_Unlock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead() = 0;
	};

	// this typedef is obsolete
	typedef CriticalSection::Ptr CriticalSectionPtr;

	/**
	 * The factory method for CriticalSection
	 */
	DLL_SYNCH_EXIM CriticalSection* CreateCriticalSection(
		EnumDSSLockStackCategory iLockStackCategory = DssLkStkCatNotNestingLock);

	/**
	 * "Bootstrap" critical section is for module initialization framework only.
	 * This instance of CriticalSection is guaranteed to exist even if Synch has 
	 * not been initialized.
	 *
	 * @see MBase::ModuleMainPtr
	 * 
	 * 2002-10-14 vovechkin
	 * DO NOT USE FOR ANY OTHER PURPOSE!
	 */
	DLL_SYNCH_EXIM CriticalSection* GetBootstrapCriticalSection();
}

#endif // MSynch_CriticialSection_h
