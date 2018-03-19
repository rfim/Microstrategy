//==============================================================================================
//	FILENAME	:	RWLockImpl.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_RWLockImpl_h
#define MSynch_RWLockImpl_h

#include "../LockCategories.h"
#include "../RWLock.h"

#include "SynchObject.h"
#include "../ProtectedSource/InprocessRWLock.h"

namespace MSynch
{
	class RWLockImpl:
		public RWLock,
		public SynchObject
	{
	public:
		RWLockImpl(EnumDSSLockStackCategory iLockCategory);
		void SetLockCategory(EnumDSSLockStackCategory iLockCatagory);
		void Delete() throw();

		/// @see MSynch::SynchObject::IsLocked
		bool IsLocked() const;

	protected:
		/// read lock the resource will allow other readers but not writers
		/// Throws a LockOrderViolationException if it has an invalid category, category has no order, or order is wrong
		/// Throws RWLockException when the lock cannot be acquired for read
		void ReadLock();
		/// write lock the resource, will not allow any other threads
		/// Throws a LockOrderViolationException if it has an invalid category, category has no order, or order is wrong
		/// Throws RWLockException when the lock cannot be acquired for write
		void WriteLock();

		void ReadUnlock() throw();
		void WriteUnlock() throw();

	private:
		virtual ~RWLockImpl() throw();
		EnumDSSLockStackCategory mLockCategory;

		InprocessRWLock mRWLock;

		bool mbLockHasBeenCalled;
		bool mbCategoryHasBeenChanged;

		// mnWriterCount is for debugging purposes
		// mnWriterCount indicates the number of times the lock has been entered for 
		// writing without leaving. only one thread may enter for writing at any given
		// time. that thread may enter the same lock several times without leaving.
		unsigned int mnWriterCount;
		

		//////////////////////////////////////////////////////////////////////////
		// We wanted to include a count for readers for debugging purposes also, 
		// but we couldn't make it thread safe without the use of an extra AtomicLong.
		// mnReadersCount is for debugging purposes
		// it indicates the number of times the lock has been entered for reading 
		// without leaving (any thread may enter it several times, and also
		// different threads may also enter it simultaneously for reading)
		// unsigned int mnReadersCount;
		///////////////////////////////////////////////////////////////////////////
	};
}

#endif // MSynch_RWLockImpl_h
