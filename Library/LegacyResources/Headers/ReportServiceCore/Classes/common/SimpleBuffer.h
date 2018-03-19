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
	class SimpleBuffer : public Buffer
	{
	public:
		SimpleBuffer() :
        mBuffer(){}
		virtual void Delete() throw() {
            for(size_t i=0; i<mBuffer.size(); ++i)
                delete [] mBuffer[i].first;
            mBuffer.clear();
        }
		
		// Allocates a chunk of memory according to the size.
		// Any further memory request will be allocated from this chunk until the chunk is used up.
		// return true if success
		virtual bool Reserve(size_t iSize) { return false; }
        
		// Returns a pointer to the memory (equivalent of doing a new)
		// throws BufferException if buffer was previously locked
		// throws ContractManagerException (defined in BaseMemoryContract.h)
		virtual void* GetMem(size_t iSize) {
            unsigned char* lBuffer = new unsigned char[iSize];
            mBuffer.push_back(MemBlock(lBuffer, iSize));
            return lBuffer;
        }
        
		// returns the total memory reserved by the buffer, in KB
		virtual unsigned Int32 GetTotalMemSize() const { return -1; }
        
		// returns the total memory reserved AND used by the buffer, in KB
		virtual unsigned Int32 GetAllocMemSize() const { return -1; }
        
		//xfan. 4/13/09, TQMS 354241. Add new interface
		virtual unsigned Int64 GetTotalMemSizeInBytes() const { return -1; }
		virtual unsigned Int64 GetAllocMemSizeInBytes() const { return -1; }
        
		// Use this function to make the buffer "read-only"
		// when this function is called the memory contained by the
		// the buffer gets locked from writes
		// throws BufferException if unable to lock
		virtual void WriteLock() {}
        
		// set the maxium limit for the total allocated space size, if the size is over the limit,
		// the buffer.GetMem operation will throw an BufferException
		// it is currently to be used by SQL Engine only
		virtual void SetTotalAllocatedSpaceLimit(Int64 iLimit) {}
        
        typedef std::pair<unsigned char*, int> MemBlock;
        typedef std::vector<MemBlock> MemVec;
        ~SimpleBuffer() throw() {
            for(size_t i=0; i<mBuffer.size(); ++i)
                delete [] mBuffer[i].first;
            mBuffer.clear();
        }
    private:
        MemVec mBuffer;
	};
}

#endif // MBase_BufferImplementation_h
