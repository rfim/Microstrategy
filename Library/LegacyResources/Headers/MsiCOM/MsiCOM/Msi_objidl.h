// File:	objidl.h
// Date:	August 27, 2001
// Author:	Liqun Jin
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'objidl.h' header file used in VC++
#ifndef __OBJIDL_H__
#define __OBJIDL_H__

#include "Msi_oaidl.h"

#ifdef WIN64
	#include "../../../../3rdParty/BaseTsd.h"
#else
	typedef unsigned long SIZE_T;
#endif

// ============================================================================
// ISequentialStream
EXTERN_C const IID IID_ISequentialStream;

struct ISequentialStream: public IUnknown
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [length_is][size_is][out] */ void __RPC_FAR *pv,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG __RPC_FAR *pcbRead) = 0;
        
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [size_is][in] */ const void __RPC_FAR *pv,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG __RPC_FAR *pcbWritten) = 0;
        
};

// ============================================================================
// IStream
typedef struct  tagSTATSTG
{
    LPOLESTR pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
}	STATSTG;

typedef enum tagSTGTY
{	
	STGTY_STORAGE	= 1,
	STGTY_STREAM	= 2,
	STGTY_LOCKBYTES	= 3,
	STGTY_PROPERTY	= 4
}	STGTY;

typedef enum tagSTREAM_SEEK
{	
	STREAM_SEEK_SET	= 0,
	STREAM_SEEK_CUR	= 1,
	STREAM_SEEK_END	= 2
}	STREAM_SEEK;

typedef enum tagLOCKTYPE
{	
	LOCK_WRITE	= 1,
	LOCK_EXCLUSIVE	= 2,
	LOCK_ONLYONCE	= 4
}	LOCKTYPE;

EXTERN_C const IID IID_IStream;

struct IStream : public ISequentialStream
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ LARGE_INTEGER dlibMove,
            /* [in] */ DWORD dwOrigin,
            /* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE SetSize( 
            /* [in] */ ULARGE_INTEGER libNewSize) = 0;
        
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
            /* [unique][in] */ IStream __RPC_FAR *pstm,
            /* [in] */ ULARGE_INTEGER cb,
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbRead,
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbWritten) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Commit( 
            /* [in] */ DWORD grfCommitFlags) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Revert( void) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
            /* [in] */ ULARGE_INTEGER libOffset,
            /* [in] */ ULARGE_INTEGER cb,
            /* [in] */ DWORD dwLockType) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
            /* [in] */ ULARGE_INTEGER libOffset,
            /* [in] */ ULARGE_INTEGER cb,
            /* [in] */ DWORD dwLockType) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Stat( 
            /* [out] */ STATSTG __RPC_FAR *pstatstg,
            /* [in] */ DWORD grfStatFlag) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) = 0;
        
};

typedef /* [unique] */ IStream __RPC_FAR *LPSTREAM;


// ============================================================================
// IMarshal
EXTERN_C const IID IID_IMarshal;

struct IMarshal : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetUnmarshalClass( 
            /* [in] */ REFIID riid,
            /* [unique][in] */ void __RPC_FAR *pv,
            /* [in] */ DWORD dwDestContext,
            /* [unique][in] */ void __RPC_FAR *pvDestContext,
            /* [in] */ DWORD mshlflags,
            /* [out] */ CLSID __RPC_FAR *pCid) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetMarshalSizeMax( 
            /* [in] */ REFIID riid,
            /* [unique][in] */ void __RPC_FAR *pv,
            /* [in] */ DWORD dwDestContext,
            /* [unique][in] */ void __RPC_FAR *pvDestContext,
            /* [in] */ DWORD mshlflags,
            /* [out] */ DWORD __RPC_FAR *pSize) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE MarshalInterface( 
            /* [unique][in] */ IStream __RPC_FAR *pStm,
            /* [in] */ REFIID riid,
            /* [unique][in] */ void __RPC_FAR *pv,
            /* [in] */ DWORD dwDestContext,
            /* [unique][in] */ void __RPC_FAR *pvDestContext,
            /* [in] */ DWORD mshlflags) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE UnmarshalInterface( 
            /* [unique][in] */ IStream __RPC_FAR *pStm,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE ReleaseMarshalData( 
            /* [unique][in] */ IStream __RPC_FAR *pStm) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE DisconnectObject( 
            /* [in] */ DWORD dwReserved) = 0;
        
};

typedef /* [unique] */ IMarshal __RPC_FAR *LPMARSHAL;

//==================================================================
// added by yma, 8/28/01

#ifndef __IPersist_FWD_DEFINED__
#define __IPersist_FWD_DEFINED__
typedef struct IPersist IPersist;
#endif 	/* __IPersist_FWD_DEFINED__ */

#ifndef __IPersistStream_FWD_DEFINED__
#define __IPersistStream_FWD_DEFINED__
typedef struct IPersistStream IPersistStream;
#endif 	/* __IPersistStream_FWD_DEFINED__ */

//===================================================================
//IPersist

EXTERN_C const IID IID_IPersist;

struct IPersist : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetClassID( 
            /* [out] */ CLSID __RPC_FAR *pClassID) = 0;
        
    };

//=================================================================
// IPersistStream
EXTERN_C const IID IID_IPersistStream;

struct IPersistStream : public IPersist
{
public:
        virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [unique][in] */ IStream __RPC_FAR *pStm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [unique][in] */ IStream __RPC_FAR *pStm,
            /* [in] */ BOOL fClearDirty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
            /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbSize) = 0;
        
};

//=================================================================
// IMallocSpy
EXTERN_C const IID IID_IMallocSpy;
    
struct IMallocSpy : public IUnknown
{
public:
    virtual SIZE_T STDMETHODCALLTYPE PreAlloc( 
        /* [in] */ SIZE_T cbRequest) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PostAlloc( 
        /* [in] */ void __RPC_FAR *pActual) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreFree( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void STDMETHODCALLTYPE PostFree( 
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual SIZE_T STDMETHODCALLTYPE PreRealloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ SIZE_T cbRequest,
        /* [out] */ void __RPC_FAR *__RPC_FAR *ppNewRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PostRealloc( 
        /* [in] */ void __RPC_FAR *pActual,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreGetSize( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual SIZE_T STDMETHODCALLTYPE PostGetSize( 
        /* [in] */ SIZE_T cbActual,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreDidAlloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual int STDMETHODCALLTYPE PostDidAlloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed,
        /* [in] */ int fActual) = 0;
    
    virtual void STDMETHODCALLTYPE PreHeapMinimize( void) = 0;
    
    virtual void STDMETHODCALLTYPE PostHeapMinimize( void) = 0;
    
};
    
#endif // __OBJIDL_H__
