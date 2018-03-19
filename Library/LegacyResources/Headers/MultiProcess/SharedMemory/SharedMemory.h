//==============================================================================================
//	FILENAME	:	SharedMemory.h
//	AUTHOR		:	Michael Yan
//	CREATION	:	3/15/2007
//	Copyright (C) MicroStrategy Incorporated 2007
//	All Rights Reserved
//==============================================================================================
#ifndef MMultiProcess_SharedMemory_h
#define MMultiProcess_SharedMemory_h

#include "SharedMemoryIncl.h"
#include "Base/Defines/StrongPtr.h"
#include "Base/Interfaces/BaseException.h"

#if defined(WIN32) || defined(WIN64)
#include "PDCHeader/PDCtchar.h"
#define MAX_INT_STR 65
#pragma warning(disable:4996)	// '_ui64tow': This function or variable may be unsafe. Consider using _ui64tow_s instead.
#endif

namespace MMultiProcess
{
	typedef unsigned Int32 shm_size_t;

	// Shared memory identifier, platform dependant
	class SharedMemoryID
	{
	public:
		SharedMemoryID(int id) : mSharedMemoryID(id) {}

		int GetID() { return mSharedMemoryID; }
		
#if defined(WIN32) || defined(WIN64)
		void GetID(TCHAR *ipIdStr)
		{
			//Pass me at least TCHAR ipIdStr[65]
			if(ipIdStr != NULL)
				_ui64tot(mSharedMemoryID,ipIdStr,10);		
		} 
#endif

	private:
		int mSharedMemoryID;
	};

	/**
	 * Shared memory is used as an IPC between multiple processes
	 * One process is responsible to create shared memory, others attach to it by using associated
	 * shared memory identifier
	 *
	 * The normal flow of usage:
	 * 1. The master process (server process in mstr) get a shared memory segment from pool manager.
	 * 2. The master process forks a child process and pass the shared memory identifier to it
	 * 3. Both master and child prcoesses create an instance of this class to handle IPC
	 * 4. Master and child process start read or write operations
	 * 5. Both sides delete its object of this class
	 * 6. Master process release shared memory segment back to pool manager.
	 */
	class DLL_SHAREDMEMORY_EXIM SharedMemory
	{
	public:

		/// Destructor
		/// Detach the shared memory
		virtual ~SharedMemory() throw() {}

		/// Query method
		/// @return maximum usable size in bytes of the shared memory
		virtual shm_size_t GetTotalSize() = 0;

		/// Reqest a block of shared memory to write
		/// May block
		/// @throw SharedMemoryException
		/// @param iReqestSize is size requsted
		/// @param oAcquiredSize return block size acquired. It maybe iReqestSize or smaller
		/// @param iTimeOutInMillisecond, if great than 0, the function will blcok no longer than the value.
		/// @return address of the block, NULL if fail, f.g timeout.
		virtual void* GetWriteBlock(shm_size_t iReqestSize, shm_size_t& orAcquiredSize, Int64 iTimeOutInMillisecond=-1) = 0;

		/// Reqest to read a block
		/// May block
		/// @throw SharedMemoryException
		/// @param opBlockSize should be either size of block ready to be read 
		///                              or 0 if nothing to be read and the other process is gone
		/// @param iTimeOutInMillisecond, if great than 0, the function will blcok no longer than the value.
		/// @return address of the block to read, NULL if fail, f.g. timeout.
		virtual void* GetReadBlock(shm_size_t& orBlockSize, Int64 iTimeOutInMillisecond=-1) = 0;

		// Return True if there is available read memory; False if time out.
		virtual bool WaitOnReadMemory(Int64 iTimeOutInMillisecond=-1) = 0;

		/// Declare that the caller has done with the block. It is either free or ready to be read.
		/// @throw SharedMemoryException
		/// @param ipStartAddr is block start address
		virtual void ReleaseBlock(void* ipStartAddr) = 0;

		/// Request for start address of shared memory
		/// @return start address of shared memory
		virtual void* GetHandle() = 0;

		virtual void Delete() = 0;
	};

	/// Shared memory is created in either read or write mode
	enum SHM_ACCESS_MODE
	{
		SHM_READ,
		SHM_WRITE,
		SHM_DUMMY
	};

	typedef MBase::StrongPtr<SharedMemory, MBase::DeleteOperatorGeneric<SharedMemory> > SharedMemoryPtr;

	/// Factory method to create an SharedMemory instance
	/// Attach my process to the shared memory specified by id
	/// @throw SharedMemoryException
	/// @param iSharedMemId is shared memory segment identifier
	/// @param iMode can be SHM_READ, SHM_WRITE or SHM_DUMMY (
	/// @return pointer to a SharedMemory object, NULL if fail
	DLL_SHAREDMEMORY_EXIM SharedMemory* CreateSharedMemory(const SharedMemoryID& iSharedMemId, SHM_ACCESS_MODE iMode = SHM_DUMMY);

	/// Factory method to create a SharedMemory instance
	/// Warning!!: this bypasses reader/writer process roles and related checks present in the above call
	/// Attach my process to the shared memory specified by id
	/// @throw SharedMemoryException
	/// @param iSharedMemId is shared memory segment identifier
	/// @return pointer to a SharedMemory object, NULL if fail
	//DLL_SHAREDMEMORY_EXIM SharedMemory* CreateSharedMemory(const SharedMemoryID& iSharedMemId);

	// Instances of this class may be thrown from Event methods.
	class SharedMemoryException : public MBase::BaseException
	{
	};
}

#endif // MMultiProcess_SharedMemory_h
