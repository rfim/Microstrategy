//==============================================================================================
//	FILENAME	:	Base.h
//	AUTHOR		:	Wayne Li
//	CREATION	:	9/26/1996
//	Copyright (C) MicroStrategy Incorporated 1996
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Buffer_h
#define MBase_Buffer_h

#include "PDCHeader/PDCstddef.h"
#include "Base.h"
#ifndef __ANDROID__
#include "Base/Interfaces/BaseException.h"
#endif
#include "Base/Defines/StrongPtr.h"

#include "Base/Base/NewOperator.h"

// Ensure that the new operator for buffer has been defined, and that the new
// operator that reports leaks has been defined.
// If the new operator of buffer is used, but is not defined, it will call the
// placement new operator (MBase::Buffer* will be implicitly upcasted to void*),
// and we will get an AV.
#ifdef WIN32
	#ifndef _BUFFER_NEW_OPERATOR_DEFINED_
		#ifndef _OBJECT_DUMP_NEW_OPERATOR_DEFINED_
			#define _NEW_OPERATOR_UNDEFINED_
			#define new _NEW_OPERATOR_NOT_DEFINED_
		#endif
	#endif
#endif

namespace MBase 
{
	/**
	 * Buffer is supposed to make performance better by allocating medium chunks (1 page)
	 * of contiguous memory, and giving it out to the client object as it requests it
	 * (in theory the client object will request for smaller chunks).
	 * Given that the OS provides good allocators (and we use SmartHeap that provides an
	 * even better one), this performance gain is not really true.
	 * Buffer is also supposed to reduce fragmentation, by keeping memory that has similar
	 * lifespan in one contiguous block. However, given that again, OS and SmartHeap allocators
	 * use memory pools, fragmentation is not really reduced significantly by buffer.
	 * Finally, Buffer was thought of a way of reducing memory leaks. The idea is that you can
	 * allocate something in the buffer and it will magically get deleted.
	 * However, though memory gets deleted, destructors of the allocated objects are not
	 * called, which violates a principle of C++ (where you are supposed to be able to rely
	 * on destructors getting called). Furthermore, though leaks do not originate from Buffer,
	 * it has severe unnecessary memory usage. On avergae, half a page will be wasted per buffer.
	 * Furthermore, objects that may have gone out of scope early on, will continue to linger
	 * and use up memory until the Buffer gets destroyed.
	 *
	 * In short, DO NOT USE Buffer. :)
	 * We only keep it here to support the current code base, and hope that sometime we will
	 * be able to retire it.
	 */
	class Buffer
	{
	public:
		enum BlockSizes
		{
			MIN_BLOCK_SIZE = 256,
			NT_BLOCK_SIZE = 4096
		};

		virtual void Delete() throw() = 0;
		
		// Allocates a chunk of memory according to the size. 
		// Any further memory request will be allocated from this chunk until the chunk is used up. 
		// return true if success
		virtual bool Reserve(size_t iSize) = 0;

		// Returns a pointer to the memory (equivalent of doing a new)
		// throws BufferException if buffer was previously locked
		// throws ContractManagerException (defined in BaseMemoryContract.h)
		virtual void* GetMem(size_t iSize) = 0;

		// returns the total memory reserved by the buffer, in KB
		virtual unsigned Int32 GetTotalMemSize() const = 0;

		// returns the total memory reserved AND used by the buffer, in KB
		virtual unsigned Int32 GetAllocMemSize() const = 0;

		//xfan. 4/13/09, TQMS 354241. Add new interface
		virtual unsigned Int64 GetTotalMemSizeInBytes() const = 0;
		virtual unsigned Int64 GetAllocMemSizeInBytes() const = 0;

		// Use this function to make the buffer "read-only"
		// when this function is called the memory contained by the
		// the buffer gets locked from writes
		// throws BufferException if unable to lock
		virtual void WriteLock() = 0;

		// set the maxium limit for the total allocated space size, if the size is over the limit, 
		// the buffer.GetMem operation will throw an BufferException
		// it is currently to be used by SQL Engine only
		virtual void SetTotalAllocatedSpaceLimit(Int64 iLimit) = 0;

//	protected:
		virtual ~Buffer() throw()
		{
		};
	};

	class MemoryContract;

	///////////////
	// Factory Functions

	/// throws ContractManagerException (defined in BaseMemoryContract.h)
	// xfan, add comments. 12/06/07
	// iSize is the initial block number, iExtSize is each incremental block number. 
	// iBlkSize is block size. 
	// if ibOnPageBoundary is set to true, it will allocate/return memory directly from OS;
	// and the memory will be allocated at least one page.
	// if ibOnPageBoundary is false, it will though the memory menager such as smartheap
	DLL_BASE_EXIM Buffer* CreateBuffer(Int32 iSize,Int32 iExtSize,
									   Int32 iBlkSize = Buffer::NT_BLOCK_SIZE, 
									   bool ibOnPageBoundary = false);

	/// throws ContractManagerException (defined in BaseMemoryContract.h)
	DLL_BASE_EXIM Buffer* CreateBuffer(Int32 iSize,Int32 iExtSize,
									   MBase::MemoryContract* ipMemoryContract,
									   Int32 iBlkSize = Buffer::NT_BLOCK_SIZE,
									   bool ibOnPageBoundary = false);

	typedef StrongPtr<Buffer,DeleteOperatorGeneric<Buffer> > BufferPtr;
#ifndef __ANDROID__
	class BufferException: public BaseException
	{
	};
#endif
}

#endif // MBase_Buffer_h
