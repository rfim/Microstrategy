// File:	atlcom.h
// Date:	10th July 2001
// Author:	Will Hurwood
// Modified by: Liqun Jin
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file replaces the atlcom.h file from ATL on UNIX
// File declares similar symbols and classes to the corresponding VC++ file

#ifndef __ATLCOM_H__
#define __ATLCOM_H__

#include "PDCHeader/PDCwindows.h"
#include "PDCHeader/PDCwchar.h"

#include "Msi_ole2.h"
#include "Msi_atldef.h"
#include "Msi_ccombstr.h"
#include "Msi_atlbase.h"

#ifdef WIN64
#pragma warning(disable:4312)	// 'type cast' : conversion from 'DWORD' to 'LPWSTR' of greater size
#endif

#define _ATL_DECLARE_GET_UNKNOWN(x) IUnknown* GetUnknown() {return _GetRawUnknown();}

//If you get a message that FinalConstruct is ambiguous then you need to
// override it in your class and call each base class' version of this
#define BEGIN_COM_MAP(x) public: \
	typedef x _ComMapClass; \
	static HRESULT WINAPI _Cache(void* pv, REFIID iid, void** ppvObject, DWORD_PTR dw)\
	{\
	   _ComMapClass* p = (_ComMapClass*)pv;\
	   p->Lock();\
	   HRESULT hRes = CComObjectRootBase::_Cache(pv, iid, ppvObject, dw);\
	   p->Unlock();\
	   return hRes;\
	}\
	IUnknown* _GetRawUnknown() \
	{ ATLASSERT(_GetEntries()[0].pFunc == _ATL_SIMPLEMAPENTRY); return (IUnknown*)((DWORD_PTR)this+_GetEntries()->dw); } \
	_ATL_DECLARE_GET_UNKNOWN(x)\
	HRESULT _InternalQueryInterface(REFIID iid, void** ppvObject) \
	{ return InternalQueryInterface(this, _GetEntries(), iid, ppvObject); } \
	const static _ATL_INTMAP_ENTRY* WINAPI _GetEntries() { \
	static const _ATL_INTMAP_ENTRY _entries[] = {

#define DECLARE_GET_CONTROLLING_UNKNOWN() public:\
	virtual IUnknown* GetControllingUnknown() {return GetUnknown();}

#define _ATL_IIDOF(x) IID_##x

#define COM_INTERFACE_ENTRY(x)\
	{&_ATL_IIDOF(x), \
	offsetofclass(x, _ComMapClass), \
	_ATL_SIMPLEMAPENTRY},

#define COM_INTERFACE_ENTRY_IID(iid, x)\
	{&iid,\
	offsetofclass(x, _ComMapClass),\
	_ATL_SIMPLEMAPENTRY},

#define COM_INTERFACE_ENTRY2(x, x2)\
	{&_ATL_IIDOF(x),\
	(DWORD_PTR)((x*)(x2*)((_ComMapClass*)8))-8,\
	_ATL_SIMPLEMAPENTRY},

#define COM_INTERFACE_ENTRY_IMPL(x)\
	COM_INTERFACE_ENTRY_IID(_ATL_IIDOF(x), x##Impl<_ComMapClass>)

#define COM_INTERFACE_ENTRY_AGGREGATE(iid, punk)\
	{&iid,\
	(DWORD)offsetof(_ComMapClass, punk),\
	_Delegate},

#define COM_INTERFACE_ENTRY_CHAIN(classname)\
	{NULL,\
	(DWORD_PTR)&_CComChainData<classname, _ComMapClass>::data,\
	_Chain},

#define END_COM_MAP() {NULL, 0, 0}}; return _entries;}

#define _ATL_DEBUG_ADDREF_RELEASE_IMPL(className)\
	virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;\
	virtual ULONG STDMETHODCALLTYPE Release(void) = 0;

#define DECLARE_PROTECT_FINAL_CONSTRUCT()\
	void InternalFinalConstructAddRef() {InternalAddRef();}\
	void InternalFinalConstructRelease() {InternalRelease();}

#define DECLARE_NOT_AGGREGATABLE(x) public:\
	typedef CComCreator2< CComCreator< CComObject< x > >, CComFailCreator<CLASS_E_NOAGGREGATION> > _CreatorClass;
#define DECLARE_AGGREGATABLE(x) public:\
	typedef CComCreator2< CComCreator< CComObject< x > >, CComCreator< CComAggObject< x > > > _CreatorClass;
#define DECLARE_ONLY_AGGREGATABLE(x) public:\
	typedef CComCreator2< CComFailCreator<E_FAIL>, CComCreator< CComAggObject< x > > > _CreatorClass;

#define DECLARE_CLASSFACTORY_EX(cf) typedef CComCreator< CComObjectCachedClassFactory< cf > > _ClassFactoryCreatorClass;
#define DECLARE_CLASSFACTORY() DECLARE_CLASSFACTORY_EX(CComClassFactory)

/////////////////////////////////////////////////////////////////////////////
// Support for the OBJECT_MAP
#define BEGIN_OBJECT_MAP(x) static _ATL_OBJMAP_ENTRY x[] = {
#define END_OBJECT_MAP()   {NULL, NULL, NULL}};
#define OBJECT_ENTRY(clsid, class) {\
	&clsid,\
	class::_ClassFactoryCreatorClass::CreateInstance,\
	class::_CreatorClass::CreateInstance },\

// We do not use the registry
#define DECLARE_REGISTRY_RESOURCEID(x)

// We do not implement Classes for connection point
#define CComConnectionPointContainerImpl IConnectionPointContainerImpl
#define BEGIN_CONNECTION_POINT_MAP(x)
#define CONNECTION_POINT_ENTRY(iid)
#define END_CONNECTION_POINT_MAP()

#ifndef MSI_min
#define MSI_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)(l))
#endif // LOWORD

#ifndef HIWORD
#define HIWORD(l)           ((WORD)(((DWORD_PTR)(l) >> 16) & 0xFFFF))
#endif // HIWORD

#ifndef MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) (LPWSTR)((DWORD)((WORD)(i)))
#endif //MAKEINTRESOURCE

#ifndef _DEFAULT_VECTORLENGTH
#define _DEFAULT_VECTORLENGTH 4
#endif

namespace ATL
{
	////////////////////////////////////////////////////////////////////////////////
	// Macros used to support QueryInterface

	struct _ATL_CACHEDATA
	{
		DWORD dwOffsetVar;
		_ATL_CREATORFUNC* pFunc;
	};

	struct _ATL_CHAINDATA
	{
		DWORD_PTR dwOffset;
		const _ATL_INTMAP_ENTRY* (WINAPI *pFunc)();
	};

	template <class base, class derived>
	class _CComChainData
	{
	public:
		static _ATL_CHAINDATA data;
	};

	template <class base, class derived>
	_ATL_CHAINDATA _CComChainData<base, derived>::data =
	{
		offsetofclass(base, derived),
		base::_GetEntries
	};

	//////////////////////////////////////////////////////////////////////////////
	// Classes used to define COM objects

	// Base class used for all COM objects implemented by ATL
	// Its methods are not virtual since they are called in top level template
	class DLL_MSICOM_EXIM CComObjectRootBase
	{
	public:
		CComObjectRootBase();

		HRESULT FinalConstruct();

		void FinalRelease();

		static HRESULT WINAPI InternalQueryInterface(void* pThis,
							 const _ATL_INTMAP_ENTRY* pEntries,
							 REFIID iid,
							 void** ppvObject);

	//Outer funcs
		ULONG OuterAddRef();

		ULONG OuterRelease();

		HRESULT OuterQueryInterface(REFIID iid, void ** ppvObject);

		void SetVoid(void*);

		void InternalFinalConstructAddRef();

		void InternalFinalConstructRelease();

		static HRESULT WINAPI _Cache(void* pv, REFIID iid, void** ppvObject, DWORD_PTR dw);

		static HRESULT WINAPI _Delegate(void* pv, REFIID iid, void** ppvObject, DWORD_PTR dw)
		{
			HRESULT hRes = E_NOINTERFACE;
			IUnknown* p = *(IUnknown**)((DWORD_PTR)pv + dw);
			if (p != NULL)
				hRes = p->QueryInterface(iid, ppvObject);
			return hRes;
		}

		static HRESULT WINAPI _Chain(void* pv, REFIID iid, void** ppvObject, DWORD_PTR dw)
		{
			_ATL_CHAINDATA* pcd = (_ATL_CHAINDATA*)dw;
			void* p = (void*)((DWORD_PTR)pv + pcd->dwOffset);
			return InternalQueryInterface(p, pcd->pFunc(), iid, ppvObject);
		}

		// BEWARE: since ATOMICLONG is not allowed in the union, we just remove union.
		//union {
		MSynch::AtomicLong m_dwRef;
		IUnknown* m_pOuterUnknown;
		//};
	};

	// CComObjectRootEx is the main base class for a COM object
	// BEWARE - Since we expect to have to use some kind of MsiCriticalSection
	// BEWARE - we have removed the critical sections from this code
	// 10/17/01 yma, solve above BEWARE
	//foward declaration
	template <class ThreadModel>
	class CComObjectRootEx;

	template <class ThreadModel>
	class CComObjectLockT
	{
	public:
		CComObjectLockT(CComObjectRootEx<ThreadModel>* p)
		{
			if (p)
				p->Lock();
			m_p = p;
		}

		~CComObjectLockT()
		{
			if (m_p)
				m_p->Unlock();
		}
		CComObjectRootEx<ThreadModel>* m_p;
	};

	//template <> class CComObjectLockT<CComSingleThreadModel>;

	// CComObjectRootEx contains a critical section.  We have modified the
	// definition of AutoCriticalSection so that it is an MSICriticalSection.
	// We add a constructor so that the caller can pass in a parameter to
	// specify the lock category of the critical section

	template <class ThreadModel>
	class CComObjectRootEx : public CComObjectRootBase
	{
	public:
		typedef ThreadModel _ThreadModel;
		typedef typename _ThreadModel::AutoCriticalSection _CritSec;
		typedef CComObjectLockT<_ThreadModel> ObjectLock;

		CComObjectRootEx(MSynch::EnumDSSLockStackCategory iLockCategory)
		{
			if (iLockCategory == MSynch::DssLkStkCatNoLock)
			{
				// this object does not need a lock; no need to create critical section for it
				m_pcritsec = NULL;
			}
			else
			{
				m_pcritsec = new _CritSec(iLockCategory);
			}
		}

		virtual ~CComObjectRootEx()
		{
			if (m_pcritsec)
			{
				delete m_pcritsec;
				m_pcritsec = NULL;
			}
		}

		// (MSI) Added this method to allow objects to reset their lock category defn.
		void SetLockCategory(MSynch::EnumDSSLockStackCategory iLockCat)
		{
			if (m_pcritsec)
				m_pcritsec->SetLockCategory(iLockCat);
				//(m_pcritsec->pCriticalSection.Get())->SetLockCategory(iLockCat);
		}

		ULONG InternalAddRef()
		{
			ATLASSERT(m_dwRef != -1L);
			return _ThreadModel::Increment(&m_dwRef);
		}
		ULONG InternalRelease()
		{
			ATLASSERT(m_dwRef > 0);
			return _ThreadModel::Decrement(&m_dwRef);
		}

		void Lock()
		{
			if (m_pcritsec)
				m_pcritsec->Exception_unsafe_Lock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();
#ifdef _DEBUG
			// in debug build, assertion failure because caller has promised not to use lock
			// when passing in DssLkStkCatNoLock to constructor
			else
				_ASSERTE("CComObjectRootEx::Lock: no lock" == 0);
#endif
		}

		void Unlock()
		{
			if (m_pcritsec)
				m_pcritsec->Exception_unsafe_Unlock__which_should_NOT_be_used_because_of_imminent_deadlocks__Use_SmartLock_instead();
#ifdef _DEBUG
			// in debug build, assertion failure because caller has promised not to use lock
			// when passing in DssLkStkCatNoLock to constructor
			else
				_ASSERTE("CComObjectRootEx::Unlock: no lock" == 0);
#endif
		}

	protected:
		// (MSI) Added this method
		// (MSI) A class that can be used as a subject object, supports this method
		void _SetSubjectObject()
		{
		}

	private:
		_CritSec *m_pcritsec;
	};

	// npratt:102099:We cannot allow a default lock category in the server
	// So we remove the default param. However, many clases across COM, Engine and
	// Kernel, inherit from CComObjectRooEx but dont actually use locks (specifically
	// CComSmartLock) - so we provide an alternative class to derive from that
	// doesnt contain a critical section so that we dont have to allocate
	// needless lock categories to objects which are single threaded hence dont have
	// a lock inside them.

	// Template class that doesnt contain a critical section
	// hence does NOT support CComSmartLock. If you need/want CComSmartLock
	// then you have to use CComObjectRootEx, and specify the lock category
	template <class ThreadModel>
	class CComObjectRootEx_NoLock : public CComObjectRootBase
	{
	public:
		typedef ThreadModel _ThreadModel;
		typedef typename _ThreadModel::AutoCriticalSection _CritSec;
		typedef CComObjectLockT<_ThreadModel> ObjectLock;

		// (MSI) Added this constructor
		CComObjectRootEx_NoLock()
		{
		}

		ULONG InternalAddRef()
		{
			ATLASSERT(m_dwRef != -1L);
			return _ThreadModel::Increment(&m_dwRef);
		}
		ULONG InternalRelease()
		{
			ATLASSERT(m_dwRef > 0);
			return _ThreadModel::Decrement(&m_dwRef);
		}

		void Lock()
		{
		}
		
		void Unlock()
		{
		}
	};

	// **** MicroStrategy modifications end here


	//////////////////////////////////////////////////////////////////////////////////
	// Top level templates used to implement IUnknown methods

	//Base is the user's class that derives from CComObjectRoot and whatever
	//interfaces the user wants to support on the object
	template <class Base>
	class CComObject : public Base
	{
	public:
		typedef Base _BaseClass;
		CComObject(void* = NULL)
		{
		}

		// Set refcount to 1 to protect destruction
		~CComObject()
		{
			this->m_dwRef = 1L;
			this->FinalRelease();
		}

		//If InternalAddRef or InternalRelease is undefined then your class
		//doesn't derive from CComObjectRoot
		STDMETHOD_(ULONG, AddRef)()
		{
			return this->InternalAddRef();
		}

		STDMETHOD_(ULONG, Release)()
		{
			ULONG l = this->InternalRelease();
			if (l == 0)
				delete this;
			return l;
		}

		//if _InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{
			return this->_InternalQueryInterface(iid, ppvObject);
		}

		static HRESULT WINAPI CreateInstance(CComObject<Base>** pp);
	};

	template <class Base>
	HRESULT WINAPI CComObject<Base>::CreateInstance(CComObject<Base>** pp)
	{
		ATLASSERT(pp != NULL);
		HRESULT hRes = E_OUTOFMEMORY;
		CComObject<Base>* p = NULL;
		ATLTRY(p = new CComObject<Base>())
		if (p != NULL)
		{
			p->SetVoid(NULL);
			p->InternalFinalConstructAddRef();
			hRes = p->FinalConstruct();
			p->InternalFinalConstructRelease();
			if (hRes != S_OK)
			{
				delete p;
				p = NULL;
			}
		}
		*pp = p;
		return hRes;
	}

	template <class Base> //Base must be derived from CComObjectRoot
	class CComContainedObject : public Base
	{
	public:
		typedef Base _BaseClass;
		CComContainedObject(void* pv)
		{
			this->m_pOuterUnknown = (IUnknown*)pv;
		}

		STDMETHOD_(ULONG, AddRef)()
		{
			return this->OuterAddRef();
		}

		STDMETHOD_(ULONG, Release)()
		{
			return this->OuterRelease();
		}

		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{
			HRESULT hr = this->OuterQueryInterface(iid, ppvObject);
			if (FAILED(hr) && this->_GetRawUnknown() != this->m_pOuterUnknown)
				hr = this->_InternalQueryInterface(iid, ppvObject);
			return hr;
		}

		//GetControllingUnknown may be virtual if the Base class has declared
		//DECLARE_GET_CONTROLLING_UNKNOWN()
		IUnknown* GetControllingUnknown()
		{
			return this->m_pOuterUnknown;
		}
	};

	// contained is the user's class that derives from CComObjectRoot and whatever
	// interfaces the user wants to support on the object
	template <class contained>
	class CComAggObject :
		public IUnknown,
		public CComObjectRootEx< typename contained::_ThreadModel::ThreadModelNoCS >
	{
	public:
		typedef contained _BaseClass;

		// **** MicroStrategy modications begin here
		// (MSI) We have added a lock category to this aggregation object, but we do not use it in Unix environment
		CComAggObject(void* pv):
			m_contained(pv),
#if defined(WIN32) && !defined(WIN64)
			CComObjectRootEx<contained::_ThreadModel::ThreadModelNoCS>(MSynch::DssLkStkCatAggObject)
#else
			CComObjectRootEx<typename contained::_ThreadModel::ThreadModelNoCS>(MSynch::DssLkStkCatAggObject)
#endif
		{
		}
		// **** MicroStrategy modifications end here

		//If you get a message that this call is ambiguous then you need to
		// override it in your class and call each base class' version of this
		HRESULT FinalConstruct()
		{
#if defined(WIN32) && !defined(WIN64)
			CComObjectRootEx<contained::_ThreadModel::ThreadModelNoCS>::FinalConstruct();
#else
			CComObjectRootEx<typename contained::_ThreadModel::ThreadModelNoCS>::FinalConstruct();
#endif
			return m_contained.FinalConstruct();
		}

		void FinalRelease()
		{
#if defined(WIN32) && !defined(WIN64)
			CComObjectRootEx<contained::_ThreadModel::ThreadModelNoCS>::FinalRelease();
#else
			CComObjectRootEx<typename contained::_ThreadModel::ThreadModelNoCS>::FinalRelease();
#endif
			m_contained.FinalRelease();
		}

		// Set refcount to 1 to protect destruction
		~CComAggObject()
		{
			this->m_dwRef = 1L;
			this->FinalRelease();
		}

		STDMETHOD_(ULONG, AddRef)()
		{
			return this->InternalAddRef();
		}

		STDMETHOD_(ULONG, Release)()
		{
			ULONG l = this->InternalRelease();
			if (l == 0)
				delete this;
			return l;
		}

		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{
			HRESULT hRes = S_OK;
			if (InlineIsEqualUnknown(iid))
			{
				if (ppvObject == NULL)
				return E_POINTER;
				*ppvObject = (void*)(IUnknown*)this;
				AddRef();
			}
			else
			{
				hRes = m_contained._InternalQueryInterface(iid, ppvObject);
			}

			return hRes;
		}

		static HRESULT WINAPI CreateInstance(LPUNKNOWN pUnkOuter, CComAggObject<contained>** pp)
		{
			ATLASSERT(pp != NULL);
			HRESULT hRes = E_OUTOFMEMORY;
			CComAggObject<contained>* p = NULL;
			ATLTRY(p = new CComAggObject<contained>(pUnkOuter))
			if (p != NULL)
			{
				p->SetVoid(NULL);
				p->InternalFinalConstructAddRef();
				hRes = p->FinalConstruct();
				p->InternalFinalConstructRelease();
				if (hRes != S_OK)
				{
					delete p;
					p = NULL;
				}
			}
			*pp = p;
			return hRes;
		}

		CComContainedObject<contained> m_contained;
	};

	// This is the top-level template used for class factory objects
	// In the original they have different synchronization requirements
	// BEWARE - Since we have no synchronization in this prototype the
	// BEWARE - difference is not apparent.  I've left in a different template
	// BEWARE - so we can separate it out again.
	template <class Base>
	class CComObjectCachedClassFactory : public Base
	{
	public:
		typedef Base _BaseClass;

		CComObjectCachedClassFactory(void* = NULL)
		{
		}

		// Set refcount to 1 to protect destruction
		~CComObjectCachedClassFactory()
		{
			this->m_dwRef = 1L;
			this->FinalRelease();
		}

		//If InternalAddRef or InternalRelease is undefined then your class
		//doesn't derive from CComObjectRoot
		STDMETHOD_(ULONG, AddRef)()
		{
			ULONG l = this->InternalAddRef();
			return l;
		}

		STDMETHOD_(ULONG, Release)()
		{
			ULONG l = this->InternalRelease();
			if (l == 0)
				delete this;
			return l;
		}

		//if _InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{
			return this->_InternalQueryInterface(iid, ppvObject);
		}
	};


	////////////////////////////////////////////////////////////////////////////////
	// Classes used to create an object

	template <class T1>
	class CComCreator
	{
	public:
		static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
		{
			ATLASSERT(*ppv == NULL);
			HRESULT hRes = E_OUTOFMEMORY;
			T1* p = NULL;
			ATLTRY(p = new T1(pv))
			if (p != NULL)
			{
				p->SetVoid(pv);
				p->InternalFinalConstructAddRef();
				hRes = p->FinalConstruct();
				p->InternalFinalConstructRelease();
				if (hRes == S_OK)
					hRes = p->QueryInterface(riid, ppv);
				if (hRes != S_OK)
					delete p;
			}
			return hRes;
		}
	};

	template <HRESULT hr>
	class CComFailCreator
	{
	public:
		static HRESULT WINAPI CreateInstance(void*, REFIID, LPVOID*)
		{
			return hr;
		}
	};

	template <class T1, class T2>
	class CComCreator2
	{
	public:
		static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
		{
			ATLASSERT(*ppv == NULL);
			return (pv == NULL) ?
				T1::CreateInstance(NULL, riid, ppv) :
				T2::CreateInstance(pv, riid, ppv);
		}
	};

	////////////////////////////////////////////////////////////////////////////////
	// Classes used for COM CoClass

	// This is the class used to implement IClassFactory on the coclass object
	class DLL_MSICOM_EXIM CComClassFactory : public IClassFactory,
				 public CComObjectRootEx<CComGlobalsThreadModel>
	{
	public:
		// **** MicroStrategy modications begin here
		// We'd need to modify CComClassFactory since it uses a lock as well
		// and we are trying to always provide a lock category

		// (MSI) we added the constructor here, so pass in a lock category
		CComClassFactory():
			CComObjectRootEx<CComGlobalsThreadModel>(MSynch::DssLkStkCatClassFactory)
		{
		}

		// **** MicroStrategy modifications end here

		BEGIN_COM_MAP(CComClassFactory)
		COM_INTERFACE_ENTRY(IClassFactory)
		END_COM_MAP()

	// IClassFactory
		STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj);

		STDMETHOD(LockServer)(BOOL fLock);

		// helper
		void SetVoid(void* pv);
		_ATL_CREATORFUNC* m_pfnCreateInstance;
	};

	template <class T, const CLSID* pclsid = &CLSID_NULL>
	class CComCoClass
	{
	public:
		DECLARE_CLASSFACTORY()
		DECLARE_AGGREGATABLE(T)
		typedef T _CoClass;

		static const CLSID& WINAPI GetObjectCLSID()
		{
			return *pclsid;
		}
	};

	// We do not implement IDispatch
	// We do not plan to support IDispatch on the Intelligence Server
	// We still need to stub out the template to allow for compilation
	// We must inherit from the base interface, since this is how the interface is included
	template <class T, const IID* piid, const GUID* plibid>
	class IDispatchImpl : public T
	{
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
			LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
			EXCEPINFO* pexcepinfo, UINT* puArgErr)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
	};

	template <const IID* piid>
	class ATL_NO_VTABLE _ICPLocator
	{
	public:
		//this method needs a different name than QueryInterface
		STDMETHOD(_LocCPQueryInterface)(REFIID riid, void ** ppvObject) = 0;
		virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;\
		virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
	};

	// IConnectionPointImpl
	template <class T, const IID* piid, class CDV/* = CComDynamicUnkArray*/ >
	class ATL_NO_VTABLE IConnectionPointImpl : public _ICPLocator<piid>
	{
	public:
		~IConnectionPointImpl()
		{
		}

		STDMETHOD(_LocCPQueryInterface)(REFIID riid, void ** ppvObject)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(GetConnectionInterface)(IID* piid2)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer** ppCPC)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(Advise)(IUnknown* pUnkSink, DWORD* pdwCookie)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(Unadvise)(DWORD dwCookie)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(EnumConnections)(IEnumConnections** ppEnum)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
	};

	// IConnectionPointContainerImpl
	template <class T>
	class ATL_NO_VTABLE IConnectionPointContainerImpl : public IConnectionPointContainer
	{
	public:
		STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints** ppEnum)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
		
		STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint** ppCP)
		{
			NOT_IMP_ASSERTE;
			return E_NOTIMPL;
		}
	};

	/////////////////////////////////////////////////////////////////////////////
	// CComEnumImpl

	// These _CopyXXX classes are used with enumerators in order to control
	// how enumerated items are initialized, copied, and deleted

	// Default is shallow copy with no special init or cleanup
	template <class T>
	class _Copy
	{
	public:
		static HRESULT copy(T* p1, T* p2)
		{
			memmove(p1, p2, sizeof(T));
			return S_OK;
		}

		static void init(T*)
		{
		}
		
		static void destroy(T*)
		{
		}
	};

	template<>
	class _Copy<VARIANT>
	{
	public:
		static HRESULT copy(VARIANT* p1, VARIANT* p2)
		{
			return VariantCopy(p1, p2);
		}
		
		static void init(VARIANT* p)
		{
			p->vt = VT_EMPTY;
		}
		
		static void destroy(VARIANT* p)
		{
			VariantClear(p);
		}
	};

	template<>
	class _Copy<LPOLESTR>
	{
	public:
		static HRESULT copy(LPOLESTR* p1, LPOLESTR* p2)
		{
			HRESULT hr = S_OK;
#ifdef NO_WCHAR_SUPPORT
			(*p1) = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR)*(ULONG)(::strlen(*p2)+1));
#else
			(*p1) = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR)*(ULONG)(::wcslen(*p2)+1));
#endif
			if (*p1 == NULL)
				hr = E_OUTOFMEMORY;
			else
#ifdef WIN32
	#ifdef NO_WCHAR_SUPPORT
				::strcpy_s(*p1,(ULONG)(::strlen(*p2)+1),*p2);
	#else
				::wcscpy_s(*p1,(ULONG)(::wcslen(*p2)+1),*p2);
	#endif
#else // for Unix
	#ifdef NO_WCHAR_SUPPORT
				::strlcpy(*p1, *p2, strlen(*p2)+1);
	#else
				::wcslcpy(*p1, *p2, wcslen(*p2)+1);
	#endif
#endif
			return hr;
		}
		
		static void init(LPOLESTR* p)
		{
			*p = NULL;
		}
		
		static void destroy(LPOLESTR* p)
		{
			CoTaskMemFree(*p);
		}
	};

	template<>
	class _Copy<CONNECTDATA>
	{
	public:
		static HRESULT copy(CONNECTDATA* p1, CONNECTDATA* p2)
		{
			*p1 = *p2;
			if (p1->pUnk)
				p1->pUnk->AddRef();
			return S_OK;
		}
		
		static void init(CONNECTDATA* )
		{
		}
		
		static void destroy(CONNECTDATA* p)
		{
			if (p->pUnk) p->pUnk->Release();
		}
	};

	template <class T>
	class _CopyInterface
	{
	public:
		static HRESULT copy(T** p1, T** p2)
		{
			*p1 = *p2;
			if (*p1)
				(*p1)->AddRef();
			return S_OK;
		}
		
		static void init(T** )
		{
		}
		
		static void destroy(T** p)
		{
			if (*p) (*p)->Release();
		}
	};

	template<class T>
	class ATL_NO_VTABLE CComIEnum : public IUnknown
	{
	public:
		STDMETHOD(Next)(ULONG celt, T* rgelt, ULONG* pceltFetched) = 0;
		STDMETHOD(Skip)(ULONG celt) = 0;
		STDMETHOD(Reset)(void) = 0;
		STDMETHOD(Clone)(CComIEnum<T>** ppEnum) = 0;
	};

	template <class Base, const IID* piid, class T, class Copy, class ThreadModel = CComObjectThreadModel>
	class ATL_NO_VTABLE CComEnum;

	enum CComEnumFlags
	{
		//see FlagBits in CComEnumImpl
		AtlFlagNoCopy = 0,
		AtlFlagTakeOwnership = 2,
		AtlFlagCopy = 3 // copy implies ownership
	};

	template <class Base, const IID* piid, class T, class Copy>
	class ATL_NO_VTABLE CComEnumImpl : public Base
	{
	public:
		CComEnumImpl()
		{
			m_begin = m_end = m_iter = NULL;
			m_dwFlags = 0;
		}
		~CComEnumImpl();
		STDMETHOD(Next)(ULONG celt, T* rgelt, ULONG* pceltFetched);
		STDMETHOD(Skip)(ULONG celt);
		STDMETHOD(Reset)(void)
		{
			m_iter = m_begin;
			return S_OK;
		}

		STDMETHOD(Clone)(Base** ppEnum);
		HRESULT Init(T* begin, T* end, IUnknown* pUnk,
			CComEnumFlags flags = AtlFlagNoCopy);
		CComPtr<IUnknown> m_spUnk;
		T* m_begin;
		T* m_end;
		T* m_iter;
		DWORD m_dwFlags;
	protected:
		enum FlagBits
		{
			BitCopy=1,
			BitOwn=2
		};
	};

	template <class Base, const IID* piid, class T, class Copy>
	CComEnumImpl<Base, piid, T, Copy>::~CComEnumImpl()
	{
		if (m_dwFlags & BitOwn)
		{
			for (T* p = m_begin; p != m_end; p++)
				Copy::destroy(p);
			delete [] m_begin;
		}
	}

	template <class Base, const IID* piid, class T, class Copy>
	STDMETHODIMP CComEnumImpl<Base, piid, T, Copy>::Next(ULONG celt, T* rgelt,
		ULONG* pceltFetched)
	{
		if (rgelt == NULL || (celt != 1 && pceltFetched == NULL))
			return E_POINTER;
		if (m_begin == NULL || m_end == NULL || m_iter == NULL)
			return E_FAIL;
		ULONG nRem = (ULONG)(m_end - m_iter);
		HRESULT hRes = S_OK;
		if (nRem < celt)
			hRes = S_FALSE;
		ULONG nMin = MSI_min(celt, nRem);
		if (pceltFetched != NULL)
			*pceltFetched = nMin;
		T* pelt = rgelt;
		while(nMin--)
		{
			HRESULT hr = Copy::copy(pelt, m_iter);
			if (FAILED(hr))
			{
				while (rgelt < pelt)
					Copy::destroy(rgelt++);
				if (pceltFetched != NULL)
					*pceltFetched = 0;
				return hr;
			}
			pelt++;
			m_iter++;
		}
		return hRes;
	}

	template <class Base, const IID* piid, class T, class Copy>
	STDMETHODIMP CComEnumImpl<Base, piid, T, Copy>::Skip(ULONG celt)
	{
		m_iter += celt;
		if (m_iter >= m_end)
		{
			m_iter = m_end;
			return S_FALSE;
		}
		if (m_iter < m_begin)
		{
			m_iter = m_begin;
			return S_FALSE;
		}
		return S_OK;
	}

	template <class Base, const IID* piid, class T, class Copy>
	STDMETHODIMP CComEnumImpl<Base, piid, T, Copy>::Clone(Base** ppEnum)
	{
		typedef CComObject<CComEnum<Base, piid, T, Copy> > _class;
		HRESULT hRes = E_POINTER;
		if (ppEnum != NULL)
		{
			*ppEnum = NULL;
			_class* p;
			hRes = _class::CreateInstance(&p);
			if (SUCCEEDED(hRes))
			{
				// If the data is a copy then we need to keep "this" object around
				if(m_dwFlags & BitCopy)
					hRes = p->Init(m_begin, m_end, this);
				else
					hRes = p->Init(m_begin, m_end, m_spUnk);

				if (SUCCEEDED(hRes))
				{
					p->m_iter = m_iter;
					hRes = p->_InternalQueryInterface(*piid, (void**)ppEnum);
				}
				if (FAILED(hRes))
					delete p;
			}
		}
		return hRes;
	}

	template <class Base, const IID* piid, class T, class Copy>
	HRESULT CComEnumImpl<Base, piid, T, Copy>::Init(T* begin, T* end, IUnknown* pUnk,
		CComEnumFlags flags)
	{
		if (flags == AtlFlagCopy)
		{
			ATLASSERT(m_begin == NULL); //Init called twice?
			ATLTRY(m_begin = new T[end-begin])
			m_iter = m_begin;
			if (m_begin == NULL)
				return E_OUTOFMEMORY;
			for (T* i=begin; i != end; i++)
			{
				Copy::init(m_iter);
				HRESULT hr = Copy::copy(m_iter, i);
				if (FAILED(hr))
				{
					T* p = m_begin;
					while (p < m_iter)
						Copy::destroy(p++);
					delete [] m_begin;
					m_begin = m_end = m_iter = NULL;
					return hr;
				}
				m_iter++;
			}
			m_end = m_begin + (end-begin);
		}
		else
		{
			m_begin = begin;
			m_end = end;
		}
		m_spUnk = pUnk;
		m_iter = m_begin;
		m_dwFlags = flags;
		return S_OK;
	}

	// CComEnum
	template <class Base, const IID* piid, class T, class Copy, class ThreadModel /*= CComObjectThreadModel*/>
	class ATL_NO_VTABLE CComEnum :
		public CComEnumImpl<Base, piid, T, Copy>,
		public CComObjectRootEx< ThreadModel >
	{
	public:
		typedef CComEnum<Base, piid, T, Copy> _CComEnum;
		typedef CComEnumImpl<Base, piid, T, Copy > _CComEnumBase;

		CComEnum():
			CComEnumImpl<Base, piid, T, Copy>(),
			CComObjectRootEx<ThreadModel>(MSynch::DssLkStkCatNoLock)
		{
		}
		
		BEGIN_COM_MAP(_CComEnum)
			COM_INTERFACE_ENTRY_IID(*piid, _CComEnumBase)
		END_COM_MAP()
	};

	// CComEnumOnSTL
	template <class Base, const IID* piid, class T, class Copy, class CollType, class ThreadModel = CComObjectThreadModel>
	class ATL_NO_VTABLE CComEnumOnSTL;
	
	template <class Base, const IID* piid, class T, class Copy, class CollType>
	class ATL_NO_VTABLE IEnumOnSTLImpl : public Base
	{
	public:
		HRESULT Init(IUnknown *pUnkForRelease, CollType& collection)
		{
			m_spUnk = pUnkForRelease;
			m_pcollection = &collection;
			m_iter = m_pcollection->begin();
			return S_OK;
		}
		STDMETHOD(Next)(ULONG celt, T* rgelt, ULONG* pceltFetched);
		STDMETHOD(Skip)(ULONG celt);
		STDMETHOD(Reset)(void)
		{
			if (m_pcollection == NULL)
				return E_FAIL;
			m_iter = m_pcollection->begin();
			return S_OK;
		}
		STDMETHOD(Clone)(Base** ppEnum);
	//Data
		CComPtr<IUnknown> m_spUnk;
		CollType* m_pcollection;
		typename CollType::iterator m_iter;
	};

	template <class Base, const IID* piid, class T, class Copy, class CollType>
	STDMETHODIMP IEnumOnSTLImpl<Base, piid, T, Copy, CollType>::Next(ULONG celt, T* rgelt,
		ULONG* pceltFetched)
	{
		if (rgelt == NULL || (celt != 1 && pceltFetched == NULL))
			return E_POINTER;
		if (m_pcollection == NULL)
			return E_FAIL;

		ULONG nActual = 0;
		HRESULT hr = S_OK;
		T* pelt = rgelt;
		while (SUCCEEDED(hr) && m_iter != m_pcollection->end() && nActual < celt)
		{
			hr = Copy::copy(pelt, &*m_iter);
			if (FAILED(hr))
			{
				while (rgelt < pelt)
					Copy::destroy(rgelt++);
				nActual = 0;
			}
			else
			{
				pelt++;
				m_iter++;
				nActual++;
			}
		}
		if (pceltFetched)
			*pceltFetched = nActual;
		if (SUCCEEDED(hr) && (nActual < celt))
			hr = S_FALSE;
		return hr;
	}

	template <class Base, const IID* piid, class T, class Copy, class CollType>
	STDMETHODIMP IEnumOnSTLImpl<Base, piid, T, Copy, CollType>::Skip(ULONG celt)
	{
		HRESULT hr = S_OK;
		while (celt--)
		{
			if (m_iter != m_pcollection->end())
				m_iter++;
			else
			{
				hr = S_FALSE;
				break;
			}
		}
		return hr;
	}

	template <class Base, const IID* piid, class T, class Copy, class CollType>
	STDMETHODIMP IEnumOnSTLImpl<Base, piid, T, Copy, CollType>::Clone(Base** ppEnum)
	{
		typedef CComObject<CComEnumOnSTL<Base, piid, T, Copy, CollType> > _class;
		HRESULT hRes = E_POINTER;
		if (ppEnum != NULL)
		{
			*ppEnum = NULL;
			_class* p;
			hRes = _class::CreateInstance(&p);
			if (SUCCEEDED(hRes))
			{
				hRes = p->Init(m_spUnk, *m_pcollection);
				if (SUCCEEDED(hRes))
				{
					p->m_iter = m_iter;
					hRes = p->_InternalQueryInterface(*piid, (void**)ppEnum);
				}
				if (FAILED(hRes))
					delete p;
			}
		}
		return hRes;
	}

	template <class Base, const IID* piid, class T, class Copy, class CollType, class ThreadModel>
	class ATL_NO_VTABLE CComEnumOnSTL :
		public IEnumOnSTLImpl<Base, piid, T, Copy, CollType>,
		public CComObjectRootEx<ThreadModel>
	{
	public:
		typedef CComEnumOnSTL<Base, piid, T, Copy, CollType, ThreadModel> _CComEnum;
		typedef IEnumOnSTLImpl<Base, piid, T, Copy, CollType> _CComEnumBase;

		CComEnumOnSTL():
			IEnumOnSTLImpl<Base, piid, T, Copy, CollType>(),
			CComObjectRootEx<ThreadModel>(MSynch::DssLkStkCatNoLock)
		{
		}

		BEGIN_COM_MAP(_CComEnum)
			COM_INTERFACE_ENTRY_IID(*piid, _CComEnumBase)
		END_COM_MAP()
	};


	/////////////////////////////////////////////////////////////////////////////
	// ISupportErrorInfoImpl

	template <const IID* piid>
	class ATL_NO_VTABLE ISupportErrorInfoImpl : public ISupportErrorInfo
	{
	public:
		STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
		{
			return (InlineIsEqualGUID(riid,*piid)) ? S_OK : S_FALSE;
		}
	};


	/////////////////////////////////////////////////////////////////////////////
	// IDispatch Error handling

	inline HRESULT WINAPI AtlSetErrorInfo(const CLSID& clsid, LPCOLESTR lpszDesc, DWORD dwHelpID,
		LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes, HINSTANCE hInst)
	{

		USES_CONVERSION;
		TCHAR szDesc[1024] = _T("Unknown Error");

		// For a valid HRESULT the id should be in the range [0x0200, 0xffff]
		if (HIWORD(lpszDesc) == 0) //id
		{
			UINT nID = LOWORD((DWORD_PTR)lpszDesc);
			ATLASSERT((nID >= 0x0200 && nID <= 0xffff) || hRes != 0);

#if defined(WIN32) && !defined(_USE_MSI_COM)
			if (LoadString(hInst, nID, szDesc, 1024) == 0)
			{
				ATLASSERT(FALSE);
			}
#endif // WIN32 && !_USE_MSI_COM

			lpszDesc = szDesc;
			if (hRes == 0)
				hRes = MAKE_HRESULT(3, FACILITY_ITF, nID);
		}

		CComPtr<ICreateErrorInfo> pICEI;
		if (SUCCEEDED(CreateErrorInfo(&pICEI)))
		{
			CComPtr<IErrorInfo> pErrorInfo;
			pICEI->SetGUID(iid);
			LPOLESTR lpsz = NULL;
			ProgIDFromCLSID(clsid, &lpsz);
			if (lpsz != NULL)
				pICEI->SetSource(lpsz);
			if (dwHelpID != 0 && lpszHelpFile != NULL)
			{
				pICEI->SetHelpContext(dwHelpID);
				pICEI->SetHelpFile(const_cast<LPOLESTR>(lpszHelpFile));
			}
			CoTaskMemFree(lpsz);
			pICEI->SetDescription((LPOLESTR)lpszDesc);
			if (SUCCEEDED(pICEI->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo)))
				SetErrorInfo(0, pErrorInfo);
		}
		return (hRes == 0) ? DISP_E_EXCEPTION : hRes;
	}

	/////////////////////////////////////////////////////////////////////////////
	// AtlReportError

	inline HRESULT WINAPI AtlReportError(const CLSID& clsid, UINT nID, const IID& iid,
		HRESULT hRes, HINSTANCE hInst)
	{
		return AtlSetErrorInfo(clsid, (LPCOLESTR)MAKEINTRESOURCE(nID), 0, NULL, iid, hRes, hInst);
	}

	inline HRESULT WINAPI AtlReportError(const CLSID& clsid, UINT nID, DWORD dwHelpID,
		LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes, HINSTANCE hInst)
	{
		return AtlSetErrorInfo(clsid, (LPCOLESTR)MAKEINTRESOURCE(nID), dwHelpID,
			lpszHelpFile, iid, hRes, hInst);
	}

	inline HRESULT WINAPI AtlReportError(const CLSID& clsid, LPCOLESTR lpszDesc,
		const IID& iid, HRESULT hRes)
	{
		return AtlSetErrorInfo(clsid, lpszDesc, 0, NULL, iid, hRes, NULL);
	}

	inline HRESULT WINAPI AtlReportError(const CLSID& clsid, LPCOLESTR lpszDesc, DWORD dwHelpID,
		LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes)
	{
		return AtlSetErrorInfo(clsid, lpszDesc, dwHelpID, lpszHelpFile, iid, hRes, NULL);
	}


	/////////////////////////////////////////////////////////////////////////////
	// Connection Points
	class CComDynamicUnkArray
	{
	public:
		CComDynamicUnkArray()
		{
			m_nSize = 0;
			m_ppUnk = NULL;
		}

		~CComDynamicUnkArray()
		{
			if (m_nSize > 1)
				free(m_ppUnk);
		}
		
		DWORD Add(IUnknown* pUnk);
		BOOL Remove(DWORD dwCookie);
		
		DWORD WINAPI GetCookie(IUnknown** pp)
		{
			DWORD iIndex;
			iIndex = DWORD(pp-begin());
			return iIndex+1;
		}
		
		static IUnknown* WINAPI GetUnknown(DWORD dwCookie)
		{
			return (IUnknown*)dwCookie;
		}
		
		IUnknown** begin()
		{
			return (m_nSize < 2) ? &m_pUnk : m_ppUnk;
		}
		
		IUnknown** end()
		{
			return (m_nSize < 2) ? (&m_pUnk)+m_nSize : &m_ppUnk[m_nSize];
		}

		IUnknown* GetAt(int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nSize)
				return NULL;

			return (m_nSize < 2) ? m_pUnk : m_ppUnk[nIndex];
		}
		
		int GetSize() const
		{
			return m_nSize;
		}

		void clear()
		{
			if (m_nSize > 1)
				free(m_ppUnk);
			m_nSize = 0;
		}
	protected:
		union
		{
			IUnknown** m_ppUnk;
			IUnknown* m_pUnk;
		};
		int m_nSize;
	};

	inline DWORD CComDynamicUnkArray::Add(IUnknown* pUnk)
	{
		DWORD iIndex;

		IUnknown** pp = NULL;
		if (m_nSize == 0) // no connections
		{
			m_pUnk = pUnk;
			m_nSize = 1;
			return 1;//(DWORD)m_pUnk;
		}
		else if (m_nSize == 1)
		{
			//create array
			pp = (IUnknown**)malloc(sizeof(IUnknown*)*_DEFAULT_VECTORLENGTH);
			if (pp == NULL)
				return 0;
			memset(pp, 0, sizeof(IUnknown*)*_DEFAULT_VECTORLENGTH);
			*pp = m_pUnk;
			m_ppUnk = pp;
			m_nSize = _DEFAULT_VECTORLENGTH;
		}
		
		for (pp = begin();pp<end();pp++)
		{
			if (*pp == NULL)
			{
				*pp = pUnk;
				iIndex = (DWORD)(pp-begin());
				return iIndex+1; //(DWORD)pUnk;
			}
		}
		int nAlloc = m_nSize*2;
		pp = (IUnknown**)realloc(m_ppUnk, sizeof(IUnknown*)*nAlloc);
		if (pp == NULL)
			return 0;
		m_ppUnk = pp;
		memset(&m_ppUnk[m_nSize], 0, sizeof(IUnknown*)*m_nSize);
		m_ppUnk[m_nSize] = pUnk;
		iIndex = m_nSize;
		m_nSize = nAlloc;
		return iIndex+1;
	}

	inline BOOL CComDynamicUnkArray::Remove(DWORD dwCookie)
	{
		if (dwCookie == 0)
			return FALSE;
		if (m_nSize == 0)
			return FALSE;
		DWORD iIndex = dwCookie-1;
		if (iIndex >= (DWORD)m_nSize)
			return FALSE;
		if (m_nSize == 1)
		{
			m_nSize = 0;
			return TRUE;
		}
		begin()[iIndex] = NULL;

		return TRUE;
	}
}

#endif /* __ATLCOM_H__ */
