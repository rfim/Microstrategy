//==============================================================================================
//	FILENAME	:	BaseMemoryContract.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_MemoryContract_h
#define MBase_MemoryContract_h

#include "BaseException.h"
#include "ReferenceCounted.h"
#include "Base/Defines/SmartPtrI.h"

typedef long MCMRESULT;
#define MCM_OK                                   ((MCMRESULT)0x00000000L)

namespace MBase 
{
	/**
	 * A MemoryContract allows you to reserve a certain amount of memory before actually
	 * allocating it. The idea is to know at the point of reserving if there is enough
	 * memory to allocate, before actually requesting the memory to the OS.
	 * Call Alloc before any allocation, with the same size of the allocation.
	 * Call Free after the successful allocation. (This means that you now have the memory
	 * from the OS and it is no longer required to count it as part of the contract. Not
	 * freeing at this point will cause the memory to be counted twice.) Notice that Free
	 * is called after successful allocation and not at the point of deleting, which means that
	 * we are tied to the current implementation of ContractManager, and a MemoryContract is
	 * totally uncapable of controlling the amount of memory used by a specific piece. Its use only
	 * allows us to verify if the OS will have enough memory for our next requests.
	 * Call Reserve when you have an estimate of how much memory you will use. More calls on
	 * Reserve accumulate. (Two calls for 1000 bytes will end up reserving at least 2000 bytes).
	 * MemoryContract is not guaranteed to use ONLY the memory you request. It might reserve more
	 * to avoid calling the ContractManager too often.
	 * The reason why MemoryContract is in Base instead of ContractManager project is because not
	 * all the components that depend on MemoryContract depend on the component implementing
	 * ContractManager, which is application specific.
	 */
	class MemoryContract: public ReferenceCounted
	{
	public:
		/// Call Alloc prior to your allocation. 
		/// Use the same size as you will use for the allocation.
		virtual MCMRESULT Alloc(unsigned Int32 inBytes)=0;

		/// Call Free after a successful allocation for which you used Alloc. 
		/// Do not call free after deletion.
		virtual void Free(unsigned Int32 inBytes) throw()=0;

		/// Call Reserve when you can estimate how much memory you will use in subsequent
		/// allocations. Calling Reserve is NOT a substitute for calling Alloc. Calling
		/// Reserve allows the ContractManager to estimate ahead of time if it will be
		/// able to fulfill the request.
		virtual MCMRESULT Reserve(unsigned Int32 inBytes)=0;

		//64 bit version with a switch to indicate if the block needs to be continuous
		virtual MCMRESULT Alloc64(unsigned Int64 inBytes, bool ibContinuous = true)=0;
		virtual void Free64(unsigned Int64 inBytes) throw()=0;

	};

	class ContractManagerAllocException: public BaseException
	{
	};

	typedef MBase::SmartPtrI<MemoryContract> MemoryContractPtr;
}

#endif // MBase_MemoryContract_h

