//==============================================================================================
//	FILENAME	:	Semaphore.h
//	AUTHOR		:	Douglas Meyer
//	CREATION	:	10/29/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_Semaphore_h
#define MSynch_Semaphore_h

#include "Base/Defines/StrongPtr.h"
#include "Base/Defines/NoCopy.h"
#include "Base/Interfaces/BaseException.h"

#include "Synch.h"

namespace MSynch 
{
	/// Semaphore protects a pool of resources. When a resource is available, a waiting thread
	///	will enter.
	/** Threads will wait in an efficient wait state until a resource becomes available
		If more than one thread is waiting on a Semaphore, it is unpredictable
		which thread will enter first.
		Semaphore only works within the same process

		WARNING WARNING WARNING

		The use of a Semaphore involves a HIGH RISK OF DEADLOCK to the application. It is not
		protected by the LockStack mechanism. Before you use a Semaphore in your component, please
		talk to the Kernel Architect and/or the Synch owner. Thanks.
	*/
	class Semaphore 
	{
	public:

		typedef MBase::StrongPtr<Semaphore, MBase::DeleteOperatorGeneric<Semaphore> > Ptr;
		typedef MBase::ReturnPtr<Semaphore, MBase::DeleteOperatorGeneric<Semaphore> > ReturnPtr;

		enum WaitForResourceResult {TIMED_OUT = 0,RESOURCE_AVAILABLE};

		/// SmartLock is intended to be created in the stack only.
		/** Usage of Semaphore::SmartLock
			It is used to wait on a resource that is normally released by a different thread
			(just like an event). To use, 
			
			 1) instatiate a SmartLock (this does not cause it to wait for the resource)
			 2) call WaitForResource which will return when a resource is available
			 3) check the return value of WaitForResource call to handle errors or timeouts
			 4) resource is aquired, continue your operations
			 5) if operations are successful call Dismiss (this will prevent the 
					smartlock from returning the resource which is already yours to the semaphore)

			If there is an error (or an exception is thrown) during step 4 (before you
			call Dismiss on step 5), then the SmartLock	will return the resource to 
			the semaphore automatically (assuming the operation was aborted and the resource was 
			not used).
		*/
		class DLL_SYNCH_EXIM SmartLock:
			public MBase::NoCopy
		{
		public:		
			//The constructor does NOT attempt to wait for a resource.
			SmartLock(Semaphore& irSemaphore);

			//If a resource is being held the destructor will automatically make the resource available
			//If Dismiss was called before destruction then the resource will NOT be made available
			~SmartLock() throw();

			/**
			 * WaitForResource will efficiently wait for a resource. When one becomes available
			 * one of the threads waiting on this call will return. The thread chosen to return
			 * is unpredictable, but it is guaranteed that no threads will be starved.
			 *  TIMED_OUT means that the waiting time was reached and a resource has not been made available
			 *  RESOURCE_AVAILABLE means a resource has been acquired
			 * @param inMilliseconds Amount of time to wait for resource to be available.
			 * @return Result (either avialable or timeout)
			 * @throw This method does not throw, except for testing purposes, in 
			 * which case it may throw a LockOrderViolationException if a LOV occurs.
			 */
			WaitForResourceResult WaitForResource(unsigned Int32 inMilliseconds);

			// Same as WaitForResource, but will not come back until the resource is available.
			WaitForResourceResult WaitForeverForResource();

			//Causes the destructor to NOT make the resource available
			void Dismiss() throw();

			//Release the resource back to the semaphore. It can only release the resource that 
			//it previously acquired. Dismiss has no effect on the operation of this method.
			void ReleaseResource() throw();

		private:
			Semaphore& mrSemaphore;
			bool mbHoldingResource;
		};

		//ResourceIsAvailable adds a resource to the pool.
		virtual void ResourceIsAvailable() = 0;

		virtual void Delete() throw() = 0;

	private:
		friend class SmartLock;

		/**
		 * WaitForResource will efficiently wait for a resource. When one becomes available
		 * one of the threads waiting on this call will return. The thread chosen to return
		 * is unpredictable, but it is guaranteed that no threads will be starved.
		 *  TIMED_OUT means that the waiting time was reached and a resource has not been made available
		 *  RESOURCE_AVAILABLE means a resource has been acquired
		 * @param inMilliseconds Amount of time to wait for resource to be available.
		 * @return Result (either avialable or timeout)
		 * @throw This method does not throw, except for testing purposes, in 
		 * which case it may throw a LockOrderViolationException if a LOV occurs.
		 */
		virtual WaitForResourceResult WaitForResource(unsigned Int32 inMilliseconds) = 0;

		// Same as WaitForResource, but will not come back until the resource is available.
		virtual WaitForResourceResult WaitForeverForResource() = 0;
	};

	// this typedef is obsolete
	typedef Semaphore::Ptr SemaphorePtr;

	/** @param inInitialAvailableResourceCount initial resources that are available
	  * @param inMaximumResourceCount total resources that can be available at one time
	  */
	DLL_SYNCH_EXIM Semaphore* CreateSemaphore(
		unsigned Int32 inInitialAvailableResourceCount,
		unsigned Int32 inMaximumResourceCount);

}

#endif // MSynch_Semaphore_h
