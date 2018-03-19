//==============================================================================================
//	FILENAME	:	InprocessFileLocker.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-12-01
//	Copyright (C) MicroStrategy Incorporated 2004
//	All Rights Reserved
//==============================================================================================
#ifndef MFileSystem_InprocessFileLocker_h
#define MFileSystem_InprocessFileLocker_h

#ifndef WIN32

#include "PDCstat.h"

#include "Base/Interfaces/ReferenceCounted.h"
#include "Base/Defines/SmartPtrI.h"
#include "Synch/Defines/ReferenceCountedImpl.h"
#include "Synch/Synch/ProtectedSource/InprocessSemaphore.h"

#include "FileImpl.h"

namespace MFileSystem
{
	class ModuleMain;

	/**
	 * This class is used for synchronizing access to a file
	 * within a process by multiple threads, because the file
	 * locking provided on UNIX (via fcntl call) only works
	 * among processes.
	 */
	class InprocessFileLocker:
		virtual public MBase::ReferenceCounted,
		public MSynch::ReferenceCountedImpl<>
	{
	public:

//		typedef MBase::SmartPtrI<InprocessFileLocker> Ptr;
		typedef MBase::ReturnPtrI<InprocessFileLocker> ReturnPtr;

		class LockPtr:
			protected MBase::SmartPtrI<InprocessFileLocker>
		{
		public:
			
			// default constructor
			LockPtr():
				MBase::SmartPtrI<InprocessFileLocker>()
			{
			}

			// destructor
			~LockPtr()
			{
				if (IsNull() == false)
				{
					Get()->Unlock();
				}
			}

			// assignment
#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__)
			LockPtr& operator=(const MBase::ReturnPtrI<InprocessFileLocker>& irReturnPtr) throw()
#else
			LockPtr& operator=(MBase::ReturnPtrI<InprocessFileLocker>& irReturnPtr) throw()
#endif
			{
				if (IsNull() == false)
				{
					Get()->Unlock();
				}

				MBase::SmartPtrI<InprocessFileLocker>* lpBaseClass = this;
				lpBaseClass->operator=(irReturnPtr);

				if (IsNull() == false)
				{
					Get()->Lock();
				}

				return *this;
			}

#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__)
			LockPtr& Assign(const MBase::ReturnPtrI<InprocessFileLocker>& irReturnPtr,bool bInProcessLock_NoWait=false)
#else
			LockPtr& Assign(MBase::ReturnPtrI<InprocessFileLocker>& irReturnPtr,bool bInProcessLock_NoWait=false)
#endif
			{
				if (IsNull() == false)
				{
					Get()->Unlock();
				}

				MBase::SmartPtrI<InprocessFileLocker>* lpBaseClass = this;
				lpBaseClass->operator=(irReturnPtr);

				if (IsNull() == false)
				{
					if (bInProcessLock_NoWait)
					{
						bool lbReturn = Get()->LockWithTimeOut();
						if (!lbReturn)
							throw ("MFileSystem::InprocessFileLocker::LockPtr::Assign() failed to get the InprocessLock");
						
					}
					else
						Get()->Lock();
				}

				return *this;
			}
			// accessor
			InprocessFileLocker* operator->() const throw()
			{
				return MBase::SmartPtrI<InprocessFileLocker>::operator->();
			}

			void Reset() throw()
			{
				if (IsNull() == false)
				{
					Get()->Unlock();
				}

				MBase::SmartPtrI<InprocessFileLocker>::Reset();
			}
		};

		/**
		 * The ID is a unique file identifier on UNIX,
		 * which consists of a device ID and I-node number.
		 */
		class ID
		{
		public:

			ID(const char* ipFilePath, FileHandle iFileHandle);

			inline bool operator<(const ID& iOtherID) const;

		private:

			dev_t mDevice;
			ino_t mInode;
		};

		InprocessFileLocker(const ID& irID, ModuleMain* ipModuleMain);
		virtual ~InprocessFileLocker() throw();

		inline const ID& GetID() const throw();

	protected:

		friend class LockPtr;
		void Lock();
		bool LockWithTimeOut();
		void Unlock() throw();

	protected:

		/// @see MSynch::ReferenceCountedImpl::ReferenceCountIsAboutToReachZero
		virtual void ReferenceCountIsAboutToReachZero() const throw();

	public:
		virtual void Release() const throw();
		void parentRelease() const throw()
		{
			MSynch::ReferenceCountedImpl<>::Release();
		}

	private:

		ModuleMain* mpModuleMain;
		const ID mID;

		MSynch::InprocessSemaphore mSemaphore;
	};
}

inline bool MFileSystem::InprocessFileLocker::ID::operator<(const ID& iOtherID) const
{
	if (mDevice == iOtherID.mDevice)
	{
		return (mInode < iOtherID.mInode);
	}
	else
	{
		return (mDevice < iOtherID.mDevice);
	}
}

inline const MFileSystem::InprocessFileLocker::ID& MFileSystem::InprocessFileLocker::GetID() const throw()
{
	return mID;
}

#endif // WIN32
#endif // MFileSystem_InprocessFileLocker_h
