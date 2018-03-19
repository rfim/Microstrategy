//==============================================================================================
//	FILENAME	:	SimpleBuffer.h
//	AUTHOR		:	ydong
//	CREATION	:	1/28/2013
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_SimpleBuffer_h
#define MBase_SimpleBuffer_h

#include <vector>

#include "Base/Base/Buffer.h"


namespace MBase 
{
    //No Lock, auto release memory when destructure.
	class BufferImplementation : public Buffer
	{
    public:
        typedef std::pair<unsigned char*, int> MemBlock;
        typedef std::vector<MemBlock> MemVec;
	public:
        
		BufferImplementation();
        ~BufferImplementation() throw();
		virtual void Delete() throw();
		
		// Allocates a chunk of memory according to the size.
		// Any further memory request will be allocated from this chunk until the chunk is used up.
		// return true if success
		virtual bool Reserve(size_t iSize);
        
		// Returns a pointer to the memory (equivalent of doing a new)
		// throws BufferException if buffer was previously locked
		// throws ContractManagerException (defined in BaseMemoryContract.h)
		virtual void* GetMem(size_t iSize);
        
		// returns the total memory reserved by the buffer, in KB
		virtual unsigned Int32 GetTotalMemSize() const ;
        
		// returns the total memory reserved AND used by the buffer, in KB
		virtual unsigned Int32 GetAllocMemSize() const ;
        
		//xfan. 4/13/09, TQMS 354241. Add new interface
		virtual unsigned Int64 GetTotalMemSizeInBytes() const ;
		virtual unsigned Int64 GetAllocMemSizeInBytes() const ;
        
		// Use this function to make the buffer "read-only"
		// when this function is called the memory contained by the
		// the buffer gets locked from writes
		// throws BufferException if unable to lock
		virtual void WriteLock() ;
        
		// set the maxium limit for the total allocated space size, if the size is over the limit,
		// the buffer.GetMem operation will throw an BufferException
		// it is currently to be used by SQL Engine only
		virtual void SetTotalAllocatedSpaceLimit(Int64 iLimit) ;
        

    private:
        MemVec mBuffer;
	};
}

#endif // MBase_BufferImplementation_h
