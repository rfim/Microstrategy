//==============================================================================================
//	FILENAME	:	SmartPtrI.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_SmartPtrI_h
#define MBase_SmartPtrI_h

#include "Asserte.h"
#include "ReturnPtrI.h"
#include "Null.h"

namespace MBase 
{
	/**
	 * SmartPtrI is a Smart Pointer that acts upon an object that has reference count mechanism.
	 * It relies on the reference count being on the object itself.
	 * You may pass pointers to reference counted objects across constellations (provided that
	 * the interfaces of such objects are constellation safe).
	 * You may handle such pointers using SmartPtrI.
	 * You may use SmartPtrI in stl collections.
	 * You may pass in a raw pointer to a reference counted object. You may use a SmartPtrI to handle 
	 * such a pointer inside your function.
	 * To pass a pointer to a reference counted object out as a return value, use ReturnPtrI (see
	 * header of ReturnPtrI for explanation).
	 * Though you may use SmartPtrI on COM objects, prefer CComPtr for COM objects instead.
	 *
	 * Important: SmartPtrI may not declare operator&(). Some stl collections rely on that operator
	 * returning the address of the object. In order for SmartPtrI to be used in a collection, it
	 * may never declare operator&().
	 */
	template<class ReferenceCountedT>
	class SmartPtrI
	{
	public:
		SmartPtrI(ReferenceCountedT* ipData=NULL) throw() :
		  mpData(ipData)
		{
			if(mpData)
			{
				mpData->AddRef();
			}
		}

		// copy constructor allowed on SmartPtr
		// upcast should be implicit
		template<class ReferenceCountedU>
		SmartPtrI(const SmartPtrI<ReferenceCountedU>& irSmartPtrI) throw()
		{
			if (!irSmartPtrI.IsNull())
			{
				mpData = irSmartPtrI.Get();
				mpData->AddRef();
			}
			else
			{
				mpData = NULL;
			}
		}

		SmartPtrI(const SmartPtrI<ReferenceCountedT>& irSmartPtrI) throw()
		{
			if (!irSmartPtrI.IsNull())
			{
				mpData = irSmartPtrI.Get();
				mpData->AddRef();
			}
			else
			{
				mpData = NULL;
			}
		}

		// constructor from a return ptr, upcast should be implicit

#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined(__hpux)
		// VisualAge compiler doesn't resolve copy-constructors with non-const
		// arguments. So we have to make the argument const and make a const
		// cast inside.
		template<class ReferenceCountedU>
		SmartPtrI(const ReturnPtrI<ReferenceCountedU>& irReturnPtrI) throw()
		{
			// do not add ref again, "steal" the reference from the return ptr
			mpData = const_cast<ReturnPtrI<ReferenceCountedU>&>(irReturnPtrI).GiveUp();
		}
#else
		template<class ReferenceCountedU>
		SmartPtrI(ReturnPtrI<ReferenceCountedU>& irReturnPtrI) throw()
		{
			// do not add ref again, "steal" the reference from the return ptr
			mpData = irReturnPtrI.GiveUp();
		}
#endif

		~SmartPtrI() throw()
		{
			Dispose();
		}

		void Reset(ReferenceCountedT* ipData=NULL) throw()
		{
			if(mpData!=ipData)
			{
				Dispose();
				mpData=ipData;
				if(mpData)
				{
					mpData->AddRef();
				}
			}
		}

		// Attach to a raw pointer without AddRef
		void Attach(ReferenceCountedT* ipData=NULL) throw()
		{
			Dispose();
			mpData=ipData;
		}

		// Assignment operator of raw pointer
		// upcast should be implicit
		SmartPtrI& operator=(ReferenceCountedT* ipData) throw()
		{
			Reset(ipData);
			return *this;
		}

		// Assignment operator
		// upcast should be implicit
		template<class ReferenceCountedU>
		SmartPtrI& operator=(const SmartPtrI<ReferenceCountedU>& irSmartPtrI) throw()
		{
			if (!irSmartPtrI.IsNull())
			{
				Reset(irSmartPtrI.Get());
			}
			else
			{
				Reset(NULL);
			}
			return *this;
		}

		SmartPtrI& operator=(const SmartPtrI<ReferenceCountedT>& irSmartPtrI) throw()
		{
			if (!irSmartPtrI.IsNull())
			{
				Reset(irSmartPtrI.Get());
			}
			else
			{
				Reset(NULL);
			}
			return *this;
		}

		// Assignment operator from a return ptr
		// upcast should be implicit
		template<class ReferenceCountedU>
		SmartPtrI& operator=(ReturnPtrI<ReferenceCountedU>& irReturnPtrI) throw()
		{
			// do not add ref again, "steal" the reference from the return ptr
			Reset(NULL);
			mpData=irReturnPtrI.GiveUp();
			return *this;
		}

#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined(__hpux)
		SmartPtrI& operator=(const ReturnPtrI<ReferenceCountedT>& irReturnPtrI) throw()
#else
		SmartPtrI& operator=(ReturnPtrI<ReferenceCountedT>& irReturnPtrI) throw()
#endif
		{
			// do not add ref again, "steal" the reference from the return ptr
			Reset(NULL);

#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined(__hpux)
			mpData = const_cast<ReturnPtrI<ReferenceCountedT>&>(irReturnPtrI).GiveUp();
#else
			mpData=irReturnPtrI.GiveUp();
#endif

			return *this;
		}
		ReferenceCountedT* operator->() const throw()
		{
			_ASSERTE(mpData!=NULL); 
			return mpData;
		}
		
		ReferenceCountedT& operator*() const throw()
		{
			_ASSERTE(mpData!=NULL); 
			return *mpData;
		}

		ReferenceCountedT* Get() const throw()
		{
			return mpData;
		}

		bool IsNull() const throw()
		{
			return !mpData;
		}

		template<class ReferenceCountedU>
		void DownCastFrom(SmartPtrI<ReferenceCountedU>& irSmartPtrI) throw()
		{
			// Only modify if iData is different from the current member
			if(static_cast<ReferenceCountedU*>(mpData) != irSmartPtrI.Get())
			{
				Reset(reinterpret_cast<ReferenceCountedT*>(irSmartPtrI.Get()));
			}
		}

		bool operator==(const SmartPtrI& iSmartBase) const
		{
			return mpData==iSmartBase.mpData;
		}

		bool operator<(const SmartPtrI& iSmartBase) const
		{
			return mpData<iSmartBase.mpData;
		}

		// GiveUp will relinquish this reference to the object
		// (we will not call release when it goes out of scope)
		ReferenceCountedT* GiveUp() throw()
		{
			ReferenceCountedT* lpTemp=mpData;
			mpData=NULL;
			return lpTemp;
		}

		// implicit cast to return ptr (to allow user to just call "return SmartPtrI")
		operator ReturnPtrI<ReferenceCountedT>() const
		{
			return Return();
		}

		// create a ReturnPtrI from this smart ptr
		// this will increment the reference count by 1 (upon creating the return ptr)
		ReturnPtrI<ReferenceCountedT> Return() const
		{
			return ReturnPtrI<ReferenceCountedT>(mpData);
		}
	private:
		void Dispose() throw()
		{
			//To avoid problems with circular references make a temporary
			//copy first and set the member to the new one before deleting it
			ReferenceCountedT* lpDataTemp = mpData;
			if(lpDataTemp)
			{
				mpData=NULL;
				lpDataTemp->Release();
			}
		}
		ReferenceCountedT* mpData;
	};
}

#endif // MBase_SmartPtrI_h
