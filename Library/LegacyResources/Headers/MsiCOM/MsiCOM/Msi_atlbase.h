// File:	atlbase.h
// Date:	10th July 2001
// Author:	Will Hurwood
// Modified by: Liqun Jin
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file replaces the atlbase.h file from ATL on UNIX
// File declares similar symbols and classes to the corresponding VC++ file

#ifndef __ATLBASE_H__
#define __ATLBASE_H__

#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#endif //!_ATL_NO_PRAGMA_WARNINGS

#define NOT_IMP_ASSERTE _ASSERTE(false && L"Called a function that is not implemented in MSI COM")

#include "Base/Defines/BasicTypes.h"
#include "MsiCOM/MsiCOM/MsiCOM.h"
#include "PDCHeader/PDCwindows.h"
#include "MsiCOM/MsiCOM/Msi_ole2.h"
#include "MsiCOM/MsiCOM/Msi_atldef.h"
#include "MsiCOM/MsiCOM/Msi_oleauto.h"
#include "Base/Base/GUID.h"

#include "PDCHeader/PDClimits.h"

#ifdef WIN64
typedef unsigned __int64 DWORD_PTR;
#else
typedef unsigned long DWORD_PTR;
#endif

#define CLSCTX_ALL 0
#include "Synch/Synch/LockCategories.h"
#include "Synch/Synch/CriticalSection.h"
#include "Synch/Synch/AtomicLong.h"

namespace ATL
{
	/////////////////////////////////////////////////////////////////////////////////
	// Provide a non threadsafe version of InterlockedIncrement and InterlockedDecrement
	inline LONG InterlockedIncrement(MSynch::AtomicLong * p) { return ++(*p); }
	inline LONG InterlockedDecrement(MSynch::AtomicLong * p) { return --(*p); }
//	inline LONG InterlockedExchange(MSynch::AtomicLong * p, LONG Value) { (*p) = Value; return (*p);}

	/////////////////////////////////////////////////////////////////////////////
	// GUID comparison

	inline BOOL InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
	{
		return MBase::IsEqualGUID(rguid1, rguid2)?TRUE:FALSE;
	}

	inline BOOL InlineIsGUIDNULL(REFGUID rguid1)
	{
		return MBase::IsGUIDNULL(rguid1)?TRUE:FALSE;
	}

	inline long InlineCompareGUID(const GUID& irGUID1, const GUID& irGUID2)
	{
		return MBase::CompareGUID(irGUID1, irGUID2);
	}

	inline BOOL InlineIsEqualUnknown(REFGUID rguid1)
	{
	   return (
		  ((PLONG) &rguid1)[0] == 0 &&
		  ((PLONG) &rguid1)[1] == 0 &&

#if defined(sun) || defined(_AIX) || defined(__hpux) // little-endian. 
		  ((PLONG) &rguid1)[2] == 0xC0000000 &&
		  ((PLONG) &rguid1)[3] == 0x00000046);
#elif defined(WIN32) || defined(linux)  // big-endian
		  ((PLONG) &rguid1)[2] == 0x000000C0 &&
		  ((PLONG) &rguid1)[3] == 0x46000000);
#else
		((PLONG) &rguid1)[2] == 0xC0000000 &&
		((PLONG) &rguid1)[3] == 0x00000046);
#endif
	}


	/////////////////////////////////////////////////////////////////////////////
	// QI Support

	typedef HRESULT (WINAPI _ATL_CREATORARGFUNC)(void* pv, REFIID riid, LPVOID* ppv, DWORD_PTR dw);
	typedef HRESULT (WINAPI _ATL_CREATORFUNC)(void* pv, REFIID riid, LPVOID* ppv);

	struct _ATL_INTMAP_ENTRY
	{
		const IID* piid;       // the interface id (IID)
		DWORD_PTR dw;
		_ATL_CREATORARGFUNC* pFunc; //NULL:end, 1:offset, n:ptr
	};

	// This is used in place of NULL when no function is available
#define _ATL_SIMPLEMAPENTRY ((_ATL_CREATORARGFUNC*)1)

	ATLAPI AtlInternalQueryInterface(void* pThis,
						 const _ATL_INTMAP_ENTRY* pEntries,
					 REFIID iid,
					 void** ppvObject);

	////////////////////////////////////////////////////////////////////////////////
	// Thread models

	// **** MicroStrategy modications begin here

	// Our objective is to replace CRITICAL_SECTION with MSICriticalSection
	// We also need to add a constructor, to pass in the LockCategory
	// We included the MSISynch headers earlier, outside the namespace

	// (MSI) All of the usages of critical sections in the
	// (MSI) ATL library files seem to be CComAutoCriticalSection, so we are okay with this.
	// (MSI) We do add a constructor to keep the three critical sections interchangable.
	// (MSI) Since CComCriticalSection is not used, we make CComCriticalSection same as CComFakeCriticalSection.
	class CComCriticalSection
	{
	public:
		// (MSI) Add a fake constructor to the class
		CComCriticalSection(MSynch::EnumDSSLockStackCategory /*iLockCategory*/)
		{
		}
		
		void Lock()
		{
		}

		void Unlock()
		{
		}

		void Init()
		{
		}

		void Term()
		{
		}
	};

	// (MSI) Change CComAutoCriticalSection so it uses the new critical section implementation
	// Don't use in user code. Only used by ATL.
	// This is a stack object, don't put on the heap
	class DLL_MSICOM_EXIM CComAutoCriticalSection
	{
	public:
		// (MSI) Add a constructor to the class
		CComAutoCriticalSection(MSynch::EnumDSSLockStackCategory iLockCategory);
		
		virtual ~CComAutoCriticalSection() throw();

		// (MSI) The Lock method is called Enter in the base class
		void Exception_unsafe_Lock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();

		// (MSI) The Unlock method is called Leave in the base class
		void Exception_unsafe_Unlock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();

		void SetLockCategory(MSynch::EnumDSSLockStackCategory iLockCat);

		// (MSI) Do nothing - MSICriticalSection is initialized on construction
		void Init();

		// (MSI) Do nothing - MSICriticalSection is deleted on destruction
		void Term();

		void Delete() throw();

	protected:
		void Lock();

		void Unlock();

	private:
		MSynch::CriticalSectionPtr mCriticalSectionPtr;
	};

	// (MSI) Added the constructor, so we don't get errors when we try to pass
	// (MSI) off a fake critical section as a real one
	class CComFakeCriticalSection
	{
	public:
		// (MSI) Add a fake constructor to the class
		CComFakeCriticalSection(MSynch::EnumDSSLockStackCategory /*iLockCategory*/)
		{
		}

		/*
		void Lock();  
		void Unlock();
		void Init();
		void Term();
		*/
		void Lock()  
		{
		}

		void Unlock()
		{
		}

		void Init()
		{
		}

		void Term()
		{
		}

		void Exception_unsafe_Lock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead() 
		{
		}

		// (MSI) The Unlock method is called Leave in the base class
		void Exception_unsafe_Unlock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead()
		{
		}

	};

	class DLL_MSICOM_EXIM CComMultiThreadModelNoCS
	{
	public:
		static ULONG WINAPI Increment(MSynch::AtomicLong * p)
		{
			return ++(*p);
		}

		static ULONG WINAPI Decrement(MSynch::AtomicLong * p)
		{
			return --(*p);
		}

		typedef CComFakeCriticalSection AutoCriticalSection;
		typedef CComFakeCriticalSection CriticalSection;
		typedef CComMultiThreadModelNoCS ThreadModelNoCS;
	};


	// BEWARE - We should not be using CComFakeCriticalSection for these
	// BEWARE - critical sections.  We expect to revisit critical sections at
	// BEWARe - a later point.
	class DLL_MSICOM_EXIM CComMultiThreadModel
	{
	public:
		static ULONG WINAPI Increment(MSynch::AtomicLong * p)
		{
			return ++(*p);
		}

		static ULONG WINAPI Decrement(MSynch::AtomicLong * p)
		{
			return --(*p);
		}

		typedef CComAutoCriticalSection AutoCriticalSection;
		typedef CComCriticalSection CriticalSection;
		typedef CComMultiThreadModelNoCS ThreadModelNoCS;
	};

	class DLL_MSICOM_EXIM CComSingleThreadModel
	{
	public:
		static ULONG WINAPI Increment(MSynch::AtomicLong * p)
		{
			return ++(*p);
		}

		static ULONG WINAPI Decrement(MSynch::AtomicLong * p)
		{
			return --(*p);
		}

		typedef CComFakeCriticalSection AutoCriticalSection;
		typedef CComFakeCriticalSection CriticalSection;
		typedef CComSingleThreadModel ThreadModelNoCS;
	};

	// We are only interested in free threaded environment
	typedef CComMultiThreadModel CComObjectThreadModel;
	typedef CComMultiThreadModel CComGlobalsThreadModel;


	////////////////////////////////////////////////////////////////////////////////
	// Object map

	// Structure used to record information about a co-class in an object
	// This structure is simpler than the one used in VC++'s ATL
	struct _ATL_OBJMAP_ENTRY
	{
		 const CLSID* pclsid;
		 _ATL_CREATORFUNC* pfnGetClassObject;
		 _ATL_CREATORFUNC* pfnCreateInstance;
		 IUnknown* pCF;
	};

	// Base class for a module
	struct _ATL_MODULE
	{
	public:
		 UINT cbSize;	// Used to detect the size of the module if it changes
		 _ATL_OBJMAP_ENTRY* m_pObjMap;
	};

	inline _ATL_OBJMAP_ENTRY* _NextObjectMapEntry(_ATL_MODULE* /*pM*/, _ATL_OBJMAP_ENTRY* pEntry)
	{
		 return pEntry+1;
	}


	////////////////////////////////////////////////////////////////////////////////
	// CComModule

	// Declaration for parts of the module implemented externally in the shared library
	ATLAPI AtlModuleGetClassObject(_ATL_MODULE* pM,
					   REFCLSID rclsid,
					   REFIID riid,
					   LPVOID* ppv);
	ATLAPI AtlModuleInit(_ATL_MODULE* pM, _ATL_OBJMAP_ENTRY* p, HINSTANCE h);
	ATLAPI AtlModuleTerm(_ATL_MODULE* pM);

	// The CComModule class is used in an inproc ATL DLL to keep track of the number
	// of COM objects currently made by the DLL so we can determine if it is safe
	// to unload the DLL.  It is the singleton class whose functions are used to
	// implement the standard entry points.
	class DLL_MSICOM_EXIM CComModule : public _ATL_MODULE 
	{
	public:
		CComModule() {}
		HRESULT Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL);
		void Term();   
		LONG GetLockCount();
		HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
		HRESULT UnregisterServer(const CLSID* pCLSID = NULL);
		HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);

		// Support the GetClassObject function
		HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

		// Lock method is intended to increment number of COM objects in module
		// We do not need it, since we don't intend to unload library
		LONG Lock();

		// Unlock method is intended to decrement number of COM objects in module
		// We do not need it, since we don't intend to unload library
		LONG Unlock();
	};

	///////////////////////////////////////////////////////////////////////////////
	// CComPtr

	// Smart Pointer helpers
	EXTERN_C DLL_MSICOM_EXIM inline IUnknown* AtlComPtrAssign(IUnknown** pp, IUnknown* lp)
	{
		if (*pp)
			(*pp)->Release();
		*pp = lp;
		if (*pp)
			(*pp)->AddRef();
		return *pp;
	}
	EXTERN_C DLL_MSICOM_EXIM IUnknown* AtlComQIPtrAssign(IUnknown** pp, IUnknown* lp, REFIID riid);

	// Connection Point Helpers
	ATLAPI AtlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw);
	ATLAPI AtlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw);

	template <class T>
	class _NoAddRefReleaseOnCComPtr : public T
	{
		private:
			STDMETHOD_(ULONG, AddRef)()=0;
			STDMETHOD_(ULONG, Release)()=0;
	};

	template <class T>
	class CComPtr
	{
	public:
		typedef T _PtrClass;
		CComPtr()
		{
			p=NULL;
		}
		CComPtr(T* lp)
		{
			if ((p = lp) != NULL)
				p->AddRef();
		}
		CComPtr(const CComPtr<T>& lp)
		{
			if ((p = lp.p) != NULL)
				p->AddRef();
		}
		~CComPtr()
		{
			if (p)
				p->Release();
		}
		void Release()
		{
			IUnknown* pTemp = p;
			if (pTemp)
			{
				p = NULL;
				pTemp->Release();
			}
		}
		operator T*() const
		{
			return (T*)p;
		}
		T& operator*() const
		{
			ATLASSERT(p!=NULL);
			return *p;
		}
		//The assert on operator& usually indicates a bug.  If this is really
		//what is needed, however, take the address of the p member explicitly.
		T** operator&()
		{
			ATLASSERT(p==NULL);
			return &p;
		}
		_NoAddRefReleaseOnCComPtr<T>* operator->() const
		{
			ATLASSERT(p!=NULL);
			return (_NoAddRefReleaseOnCComPtr<T>*)p;
		}
		T* operator=(T* lp)
		{
			return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
		}
		T* operator=(const CComPtr<T>& lp)
		{
			return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
		}
		bool operator!() const
		{
			return (p == NULL);
		}
		bool operator<(T* pT) const
		{
			return p < pT;
		}
		bool operator==(T* pT) const
		{
			return p == pT;
		}
		// Compare two objects for equivalence
		bool IsEqualObject(IUnknown* pOther)
		{
			if (p == NULL && pOther == NULL)
				return true; // They are both NULL objects

			if (p == NULL || pOther == NULL)
				return false; // One is NULL the other is not

			CComPtr<IUnknown> punk1;
			CComPtr<IUnknown> punk2;
			p->QueryInterface(IID_IUnknown, (void**)&punk1);
			pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
			return punk1 == punk2;
		}
		void Attach(T* p2)
		{
			if (p)
				p->Release();
			p = p2;
		}
		T* Detach()
		{
			T* pt = p;
			p = NULL;
			return pt;
		}
		HRESULT CopyTo(T** ppT)
		{
			ATLASSERT(ppT != NULL);
			if (ppT == NULL)
				return E_POINTER;
			*ppT = p;
			if (p)
				p->AddRef();
			return S_OK;
		}
		HRESULT SetSite(IUnknown* punkParent)
		{
			// return AtlSetChildSite(p, punkParent);
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
		{
			return AtlAdvise(p, pUnk, iid, pdw);
		}
		// BEWARE: CLSCTX_ALL is defined to 0 which is a fake value. if want to use this method, you need to define CLSCTX_ALL
		HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
		{
			/*
			ATLASSERT(p == NULL);
			return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		// BEWARE: CLSCTX_ALL is defined to 0 which is a fake value. if want to use this method, you need to define CLSCTX_ALL
		HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
		{
			/*
			CLSID clsid;
			HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
			ATLASSERT(p == NULL);
			if (SUCCEEDED(hr))
				hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
			return hr;
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		template <class Q>
		HRESULT QueryInterface(Q** pp) const
		{
			/*
			ATLASSERT(pp != NULL && *pp == NULL);
			return p->QueryInterface(__uuidof(Q), (void**)pp);
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		T* p;
	};

	//template <class T, const IID* piid = &__uuidof(T)>
	template <class T, const IID* piid>
	class CComQIPtr
	{
	public:
		typedef T _PtrClass;
		CComQIPtr()
		{
			p=NULL;
		}
		CComQIPtr(T* lp)
		{
			if ((p = lp) != NULL)
				p->AddRef();
		}
		CComQIPtr(const CComQIPtr<T,piid>& lp)
		{
			if ((p = lp.p) != NULL)
				p->AddRef();
		}
		CComQIPtr(IUnknown* lp)
		{
			p=NULL;
			if (lp != NULL)
				lp->QueryInterface(*piid, (void **)&p);
		}
		~CComQIPtr()
		{
			if (p)
				p->Release();
		}
		void Release()
		{
			IUnknown* pTemp = p;
			if (pTemp)
			{
				p = NULL;
				pTemp->Release();
			}
		}
		operator T*() const
		{
			return p;
		}
		T& operator*() const
		{
			ATLASSERT(p!=NULL); return *p;
		}
		//The assert on operator& usually indicates a bug.  If this is really
		//what is needed, however, take the address of the p member explicitly.
		T** operator&()
		{
			ATLASSERT(p==NULL);
			return &p;
		}
		_NoAddRefReleaseOnCComPtr<T>* operator->() const
		{
			ATLASSERT(p!=NULL);
			return (_NoAddRefReleaseOnCComPtr<T>*)p;
		}
		T* operator=(T* lp)
		{
			return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
		}
		T* operator=(const CComQIPtr<T,piid>& lp)
		{
			return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
		}
		T* operator=(IUnknown* lp)
		{
			return (T*)AtlComQIPtrAssign((IUnknown**)&p, lp, *piid);
		}
		bool operator!() const
		{
			return (p == NULL);
		}
		bool operator<(T* pT) const
		{
			return p < pT;
		}
		bool operator==(T* pT) const
		{
			return p == pT;
		}
		// Compare two objects for equivalence
		bool IsEqualObject(IUnknown* pOther)
		{
			if (p == NULL && pOther == NULL)
				return true; // They are both NULL objects

			if (p == NULL || pOther == NULL)
				return false; // One is NULL the other is not

			CComPtr<IUnknown> punk1;
			CComPtr<IUnknown> punk2;
			p->QueryInterface(IID_IUnknown, (void**)&punk1);
			pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
			return punk1 == punk2;
		}
		void Attach(T* p2)
		{
			if (p)
				p->Release();
			p = p2;
		}
		T* Detach()
		{
			T* pt = p;
			p = NULL;
			return pt;
		}
		HRESULT CopyTo(T** ppT)
		{
			ATLASSERT(ppT != NULL);
			if (ppT == NULL)
				return E_POINTER;
			*ppT = p;
			if (p)
				p->AddRef();
			return S_OK;
		}
		HRESULT SetSite(IUnknown* punkParent)
		{
			// return AtlSetChildSite(p, punkParent);
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
		{
			return AtlAdvise(p, pUnk, iid, pdw);
		}
		// BEWARE: CLSCTX_ALL is defined to 0 which is a fake value. if want to use this method, you need to define CLSCTX_ALL
		HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
		{
			/*
			ATLASSERT(p == NULL);
			return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		// BEWARE: CLSCTX_ALL is defined to 0 which is a fake value. if want to use this method, you need to define CLSCTX_ALL
		HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
		{
			/*
			CLSID clsid;
			HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
			ATLASSERT(p == NULL);
			if (SUCCEEDED(hr))
				hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
			return hr;
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		template <class Q>
		HRESULT QueryInterface(Q** pp)
		{
			/*
			ATLASSERT(pp != NULL && *pp == NULL);
			return p->QueryInterface(__uuidof(Q), (void**)pp);
			*/
			//Since we do not use it, we need not implement it
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		T* p;
	};
}

using namespace ATL;

#include "MsiCOM/MsiCOM/Msi_ccombstr.h"
#include "MsiCOM/MsiCOM/Msi_ccomvariant.h"

#endif // __ATLBASE_H__
