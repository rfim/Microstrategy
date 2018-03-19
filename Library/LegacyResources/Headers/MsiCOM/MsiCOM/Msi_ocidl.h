// File:	ocidl.h
// Date:	10th July 2001
// Author:	Will Hurwood
// Modified by: Liqun Jin	
//				Ke Xiao	Add IPersist & IPersistMemory Interface
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'ocidl.h' header file used in VC++
#ifndef __OCIDL_H__
#define __OCIDL_H__

#ifndef __IEnumConnections_FWD_DEFINED__
#define __IEnumConnections_FWD_DEFINED__
typedef struct IEnumConnections IEnumConnections;
#endif 	/* __IEnumConnections_FWD_DEFINED__ */

#ifndef __IConnectionPoint_FWD_DEFINED__
#define __IConnectionPoint_FWD_DEFINED__
typedef struct IConnectionPoint IConnectionPoint;
#endif 	/* __IConnectionPoint_FWD_DEFINED__ */

#ifndef __IEnumConnectionPoints_FWD_DEFINED__
#define __IEnumConnectionPoints_FWD_DEFINED__
typedef struct IEnumConnectionPoints IEnumConnectionPoints;
#endif 	/* __IEnumConnectionPoints_FWD_DEFINED__ */

#ifndef __IConnectionPointContainer_FWD_DEFINED__
#define __IConnectionPointContainer_FWD_DEFINED__
typedef struct IConnectionPointContainer IConnectionPointContainer;
#endif 	/* __IConnectionPointContainer_FWD_DEFINED__ */

#ifndef __IPersistMemory_FWD_DEFINED__
#define __IPersistMemory_FWD_DEFINED__
typedef struct IPersistMemory IPersistMemory;
#endif 	/* __IPersistMemory_FWD_DEFINED__ */

#ifndef __IPersistStreamInit_FWD_DEFINED__
#define __IPersistStreamInit_FWD_DEFINED__
typedef struct IPersistStreamInit IPersistStreamInit;
#endif 	/* __IPersistStreamInit_FWD_DEFINED__ */

// ============================================================================
// IConnectionPoint
EXTERN_C const IID IID_IConnectionPoint;
    
struct IConnectionPoint: public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetConnectionInterface( 
            /* [out] */ IID __RPC_FAR *pIID) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetConnectionPointContainer( 
            /* [out] */ IConnectionPointContainer __RPC_FAR *__RPC_FAR *ppCPC) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ IUnknown __RPC_FAR *pUnkSink,
            /* [out] */ DWORD __RPC_FAR *pdwCookie) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ DWORD dwCookie) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE EnumConnections( 
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum) = 0;
};

typedef IConnectionPoint __RPC_FAR *PCONNECTIONPOINT;

typedef IConnectionPoint __RPC_FAR *LPCONNECTIONPOINT;


// ============================================================================
// IConnectionPointContainer
EXTERN_C const IID IID_IConnectionPointContainer;

struct IConnectionPointContainer: public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE EnumConnectionPoints( 
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE FindConnectionPoint( 
            /* [in] */ REFIID riid,
            /* [out] */ IConnectionPoint __RPC_FAR *__RPC_FAR *ppCP) = 0;
        
};
    
typedef IConnectionPointContainer __RPC_FAR *PCONNECTIONPOINTCONTAINER;

typedef IConnectionPointContainer __RPC_FAR *LPCONNECTIONPOINTCONTAINER;

// ============================================================================
// IEnumConnections
typedef struct  tagCONNECTDATA
    {
    IUnknown __RPC_FAR *pUnk;
    DWORD dwCookie;
    }	CONNECTDATA;

typedef struct tagCONNECTDATA __RPC_FAR *PCONNECTDATA;

typedef struct tagCONNECTDATA __RPC_FAR *LPCONNECTDATA;
EXTERN_C const IID IID_IEnumConnections;

struct IEnumConnections : public IUnknown
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTDATA rgcd,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cConnections) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum) = 0;
};

// ============================================================================
// IEnumConnectionPoints
struct IEnumConnectionPoints : public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cConnections,
            /* [length_is][size_is][out] */ LPCONNECTIONPOINT __RPC_FAR *ppCP,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cConnections) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum) = 0;
};

// ============================================================================
// IPersistMemory

#ifndef __IPersistMemory_INTERFACE_DEFINED__
#define __IPersistMemory_INTERFACE_DEFINED__

typedef IPersistMemory __RPC_FAR *LPPERSISTMEMORY;

EXTERN_C const IID IID_IPersistMemory;

struct IPersistMemory : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [size_is][in] */ LPVOID pMem,
            /* [in] */ ULONG cbSize) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Save( 
            /* [size_is][out] */ LPVOID pMem,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ ULONG cbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ ULONG __RPC_FAR *pCbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
    };    
#endif

// ============================================================================
// IPersistStreamInit

#ifndef __IPersistStreamInit_INTERFACE_DEFINED__
#define __IPersistStreamInit_INTERFACE_DEFINED__

typedef IPersistStreamInit __RPC_FAR *LPPERSISTSTREAMINIT;

EXTERN_C const IID IID_IPersistStreamInit;

struct IPersistStreamInit : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ LPSTREAM pStm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ LPSTREAM pStm,
            /* [in] */ BOOL fClearDirty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pCbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
    };
#endif

#endif
