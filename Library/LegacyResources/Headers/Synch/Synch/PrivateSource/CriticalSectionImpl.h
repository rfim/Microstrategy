//==============================================================================================
//	FILENAME	:	CriticalSectionImpl.h
//	AUTHOR		:	Jing Ning
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_CriticalSectionImpl_h
#define MSynch_CriticalSectionImpl_h

#include "PDCHeader/PDCvector"

#include "../LockCategories.h"
#include "../CriticalSection.h"

#include "SynchObject.h"
#include "../ProtectedSource/InprocessRecursiveMutex.h"

namespace MSynch
{
	class CriticalSectionImpl:
		public CriticalSection,
		public SynchObject
	{
	public:
		CriticalSectionImpl(EnumDSSLockStackCategory iLockCategory);
		void Delete() throw();
		void SetLockCategory(EnumDSSLockStackCategory iLockCatagory);

		/// @see MSynch::SynchObject::IsLocked
		bool IsLocked() const;

		void InitState(EnumDSSLockStackCategory iLockCategory); 
		static CriticalSectionImpl* GetCS(EnumDSSLockStackCategory iLockCategory);
		static void ReleaseCS(CriticalSectionImpl* cs);

	protected:
		virtual void Lock();
		virtual void Unlock() throw();		

	private:

		EnumDSSLockStackCategory mLockCategory;

		// use our mutex as underlying implementation
		InprocessRecursiveMutex mMutex;

		bool mbLockHasBeenCalled;
		bool mbCategoryHasBeenChanged;

		// mnLastOccupyingThreadID is for debugging purposes
		// read this member to find out which thread is holding the lock
		unsigned int mnLastOccupyingThreadID;
		// mnOccupiedCount is for debugging purposes
		// it counts how many times the lock has been entered (without leaving) by the same thread
		unsigned int mnOccupiedCount;


	public:
		virtual void Exception_unsafe_Lock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();
		virtual void Exception_unsafe_Unlock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();
	};
}

#endif // MSynch_CriticalSectionImpl_h
