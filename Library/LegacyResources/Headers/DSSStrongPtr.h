/*
 *  DSSStrongPtr.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/13/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef STRONGPTR_H
#define STRONGPTR_H

#import "DSSDeleteOperator.h"

namespace MBase 
{
	/**
	 * ========================= NOTE =============================================
	 * This is a partial implmentation of Server side /Common/Base/Define/DSSStrongPtr
	 * it's only used to hold a local/temprary allocated object so that it can be 
	 * automatically released when out of scope or expection throws
	 * for more inofrmation about the usage of DSSStrongPtr, refer to the original file
	 * ============================================================================
	 *
	 */
	template<class T, class DSSDeleteOperator = DeleteC<T> >
	class DSSStrongPtr
	{
	public:
		/// The destructor is not virtual, because strong objects
		/// are never used polymorphically via a base class pointer
		virtual ~DSSStrongPtr(); 
		
		// Make the constructor explicit, so that we don't incur into
		// funny implicit conversion.
		explicit DSSStrongPtr(T* ipData = NULL) throw():
		mpData(ipData)
		{
		}
		
		/// Use the get operator to get a weak reference. You may use this reference
		/// to pass to a function that 1) is completely enclosed within the DSSStrongPtr scope, 
		/// and 2) does not store the resource in any way.
		/// Remember the DSSStrongPtr still keeps the ownership of the pointer
		T* Get() const	throw()
		{
			return mpData;
		}
		
		/// Reset grounds (deleting if necessary) the old member pointer.
		/// It also sets the member pointer to the new value sent in (if any).
		/// Careful only to reset the pointer if set to a different one.
		/// Resetting to the same one might inadvertly delete the object.
		void Reset(T* ipData = NULL) throw()
		{
			if(mpData!=ipData)
			{
				//To avoid problems with circular references make a temporary
				//copy first and set the member to the new one before deleting it
				T* lpData = mpData;
				mpData = ipData;
				DSSDeleteOperator()(lpData);
			}
		}
		
		/// GiveUp will relinquish ownership of a DSSStrongPtr, and will return
		/// the object
		T* GiveUp() throw()
		{
			T* lpTemp=mpData;
			mpData=NULL;
			return lpTemp;
		}
		
		/// Check for NULL
		bool IsNull() const	throw()
		{
			return mpData==NULL;
		}
		
	private:
		T* mpData;

		// No Copy constructor or assignment operator allowed on DSSStrongPtr.
		DSSStrongPtr(const DSSStrongPtr&);
		DSSStrongPtr& operator=(const DSSStrongPtr&);

		//not implemented methods. Make them private
		//template <class T2>
		//void operator=(const ReturnPtr<T2, DSSDeleteOperator>& irReturnPtr) throw();
		//void operator=(ReturnPtr<T2, DSSDeleteOperator>& irReturnPtr) throw();
		T* operator->() const throw();
		T& operator*() const throw();
		T** operator&() throw();
		template<class U,class DeleteOperatorU>
		void Acquire(DSSStrongPtr<U, DeleteOperatorU>& iStrongPtr) throw();
		void Acquire(T*& irpData) throw();
		template<class U,class DeleteOperatorU>
		void Swap(DSSStrongPtr<U, DeleteOperatorU>& iStrongPtr) throw();
		//ReturnPtr<T, DSSDeleteOperator> Return();
	};

	template<class T,class DSSDeleteOperator>
	DSSStrongPtr<T, DSSDeleteOperator>::~DSSStrongPtr()
	{
		DSSDeleteOperator()(mpData);
		mpData = NULL;
	}
}

#endif
