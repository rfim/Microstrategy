//==============================================================================================
//	FILENAME	:	SemaphoreImpl.h
//	AUTHOR		:	Douglas Meyer
//	CREATION	:	10/29/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_SemaphoreImpl_h
#define MSynch_SemaphoreImpl_h

#include "../Semaphore.h"
#include "../ProtectedSource/InprocessSemaphore.h"

namespace MSynch
{
	class SemaphoreImpl:
		public Semaphore
	{
	public:
		SemaphoreImpl(unsigned Int32 inInitialAvailableResourceCount,unsigned Int32 inMaximumResourceCount);

		void Delete() throw();

		Semaphore::WaitForResourceResult WaitForResource(unsigned Int32 iMilliseconds);

		// Same as WaitForResource but does not timeout. Waits forever for resource.
		Semaphore::WaitForResourceResult WaitForeverForResource();

		void ResourceIsAvailable();

	private:
		virtual ~SemaphoreImpl() throw();	
		InprocessSemaphore mSemaphore;
	};
}

#endif // MSynch_SemaphoreImpl_h
