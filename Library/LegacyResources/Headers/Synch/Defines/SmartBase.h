//==============================================================================================
//	FILENAME	:	SmartBase.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	10/19/2001
//  Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_SmartBase_h
#define MSynch_SmartBase_h

#include "Base/Defines/Null.h"
#include "Base/Defines/Asserte.h"
#include "Base/Defines/DeleteOperator.h"
#if defined(_NO_EXCEPTION)
#include <stdio.h>
#endif

namespace MSynch
{
	template<class T,class DeleteOperator>
	class SmartBase
	{
	public:
		~SmartBase() throw();

		T Get() const throw()
		{
			return mData;
		}

		// if reset throws, it will free the resource it's being set to guard
		// IMPORTANT: reset assumes that the assignment of T does not throw
		void Reset(T iData=NULL)
		{
			_ASSERTE(*mpnRefCount<10000);	// Reference count is corrupted
			_ASSERTE(*mpnRefCount>0);		// Reference count is corrupted

			// Only modify if iData is different from the current member
			if(mData!=iData)
			{
				//create a new reference count and start using that one

				// first try to allocate the new ref count
				Int32* lpnTempRefCount=NULL;
#if !defined(_NO_EXCEPTION)
				try
				{
					lpnTempRefCount=new Int32(1);
				}
				catch(...)
				{
					// if memory allocation fails,
					// delete resource we were assigned to guard and rethrow
					// this SmartPtr remains unchanged
					DeleteOperator()(iData);
					throw;
				}
#else
				lpnTempRefCount=new Int32(1);
#endif

				// if we were able to allocate the new refcount, then we
				// are safe, decrement the original refcount and copy over
				// the new guarded object
				Dispose();

				mpnRefCount=lpnTempRefCount;
				lpnTempRefCount=NULL;

				mData = iData;
			}
		}

		bool operator==(const SmartBase& iSmartBase) const
		{
			return mData==iSmartBase.mData;
		}

		bool operator<(const SmartBase& iSmartBase) const
		{
			return mData<iSmartBase.mData;
		}

		bool IsNull() const	throw()
		{
			return mData==NULL;
		}
	protected:
		explicit SmartBase(T iData = NULL);

		explicit SmartBase(const SmartBase& iSmartPtr) throw() :
			mpnRefCount(iSmartPtr.mpnRefCount),
			mData(iSmartPtr.mData)
		{
			_ASSERTE(*mpnRefCount<10000);	// Reference count is corrupted
			_ASSERTE(*mpnRefCount>0);		// Reference count is corrupted

			++(*mpnRefCount);
		}

		SmartBase(T iData, Int32* ipRefCount) throw() :
			mpnRefCount(ipRefCount),
			mData(iData)
		{
			_ASSERTE(*mpnRefCount<10000);	// Reference count is corrupted
			_ASSERTE(*mpnRefCount>0);		// Reference count is corrupted

			++(*mpnRefCount);
		}

		void CallDeleteOperator()
		{
			//To avoid problems with circular references make a temporary
			//copy first and set the member to NULL.
			T lData = mData;
			if (lData)
			{
				mData = NULL;
				DeleteOperator()(lData);
				lData = NULL;
			}
		}

		void Dispose() throw()
		{
			if(mpnRefCount)
			{
				_ASSERTE(*mpnRefCount<10000);	// Reference count is corrupted
				_ASSERTE(*mpnRefCount>=0);	// Reference count is corrupted

				if ((--(*mpnRefCount)) == 0)
				{
					//To avoid problems with circular references make a temporary
					//copy first and set the member to NULL.
					Int32* lpnRefCount = mpnRefCount;

					if (lpnRefCount)
					{
						mpnRefCount = NULL;
						delete lpnRefCount;
						lpnRefCount=NULL;
					}
					CallDeleteOperator();
				}
			}
		}

		void ReplaceWith(T iData,Int32* ipRefCount) throw()
		{
			Dispose();

			_ASSERTE(*ipRefCount<10000);	// Reference count is corrupted
			_ASSERTE(*ipRefCount>0);		// Reference count is corrupted

			mpnRefCount = ipRefCount;
			++(*mpnRefCount);
			mData = iData;
		}

	private:
		SmartBase& operator=(const SmartBase&);

	public:
		//////////////////////////////////////////////////////////////////
		// For all purposes, these data members should be considered protected.
		// They are only public here to allow SmartPtrs to have polymorphism.
		// No other class other than SmartBase derived classes should access these members.
		T	  mData;

		Int32* mpnRefCount;
	};

	template<class T,class DeleteOperator>
	SmartBase<T, DeleteOperator>::~SmartBase() throw()
	{
		Dispose();
	}

 	template<class T,class DeleteOperator>
	SmartBase<T, DeleteOperator>::SmartBase(T iData):
			mData(iData),
			mpnRefCount(NULL)
	{
		// When the first smart pointer is created, we must allocate the
		// space for the reference count. If it fails to allocate that space,
		// cleanup mData (which we already own) and rethrow.
#if !defined(_NO_EXCEPTION)
		try
		{
			mpnRefCount = new Int32(1);
		}
		catch(...)
		{
			CallDeleteOperator();
			throw;
		}
#else
		mpnRefCount = new Int32(1);
#endif	

	}

}

#endif // MSynch_SmartBase_h
