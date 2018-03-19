//==============================================================================================
//	FILENAME	:	RWLock.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_RWLock_h
#define MSynch_RWLock_h

#include "Base/Defines/NoCopy.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Interfaces/BaseException.h"

#include "Synch.h"
#include "LockCategories.h"
#include "LockOrderViolationException.h"

namespace MSynch
{
	/// ReadWrite Lock allows multiple "readers" to pass at any time but
	/// only allows one "writer" at a time. When the "writer" is in then
	/// all readers are out too.
	class RWLock
	{
	public:

		typedef MBase::StrongPtr<RWLock, MBase::DeleteOperatorGeneric<RWLock> > Ptr;
		typedef MBase::ReturnPtr<RWLock, MBase::DeleteOperatorGeneric<RWLock> > ReturnPtr;

		/// ReadSmartLock is intended to be created in the stack only.
		class DLL_SYNCH_EXIM ReadSmartLock:
			public MBase::NoCopy
		{
		public:
			ReadSmartLock(RWLock& irRWLock, bool ibLocking = true); // bxia: add second param to control whether we lock in constructor
			~ReadSmartLock() throw();

			/**
			 * Read lock the resource.
			 * Will allow other readers but not writers.
			 * @throw This method does not throw, except for testing purposes, in 
			 * which case it may throw a LockOrderViolationException if a LOV occurs.
			 */
			void Lock();
			void Unlock() throw();

		private:
			RWLock& mrRWLock;
			bool mbLocked;
		};

		/// WriteSmartLock is intended to be created in the stack only.
		class DLL_SYNCH_EXIM WriteSmartLock:
			public MBase::NoCopy
		{
		public:
			WriteSmartLock(RWLock& irRWLock, bool ibLocking = true); // bxia: add second param to control whether we lock in constructor
			~WriteSmartLock() throw();

			/**
			 * Write lock the resource.
			 * Will not allow other threads to access it.
			 * @throw This method does not throw, except for testing purposes, in 
			 * which case it may throw a LockOrderViolationException if a LOV occurs.
			 */
			void Lock();
			void Unlock() throw();

		private:
			RWLock& mrRWLock;
			bool mbLocked;
		};
		virtual void Delete() throw() = 0;

	protected:
		friend class ReadSmartLock;
		friend class WriteSmartLock;

		/**
		 * Read lock the resource.
		 * Will allow other readers but not writers.
		 * @throw This method does not throw, except for testing purposes, in 
		 * which case it may throw a LockOrderViolationException if a LOV occurs.
		 */
		virtual void ReadLock() = 0; 	

		/**
		 * Write lock the resource.
		 * Will not allow other threads to access it.
		 * @throw This method does not throw, except for testing purposes, in 
		 * which case it may throw a LockOrderViolationException if a LOV occurs.
		 */
		virtual void WriteLock() = 0; 

		virtual void ReadUnlock() throw() = 0;	
		virtual void WriteUnlock() throw() = 0;	
	};

	// this typedef is obsolete
	typedef RWLock::Ptr RWLockPtr;

	DLL_SYNCH_EXIM RWLock* CreateRWLock(EnumDSSLockStackCategory iLockCategory = DssLkStkCatNotNestingLock);

	/** Instances of this class may be thrown from RWLock methods.
	*/
	class RWLockException:
		public MBase::BaseException
	{
	};
}

#endif // MSynch_RWLock_h
