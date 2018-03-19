// File:	oaidl.h
// Date:	10th July 2001
// Author:	Will Hurwood
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'oaidl.h' header file used in VC++
// We make minimal effort to declare symbols and functions in the same file as
// used in Windows.

// We use this file to declare standard COM interfaces used in our C++ project
// In Windows these interfaces are split over many files, but hopefully we will
// only need a few of them, and so we can survive with a single file.

// Note that since we are not trying to maintain connectivity with any third party
// code we do not have to make the interfaces identical if we do not use all of
// their functionality.  In particular there is no need to implement IDispatch.

#ifndef __OAIDL_H__
#define __OAIDL_H__

#include "PDCHeader/PDCrpc.h"
#include "PDCHeader/PDCwtypes.h"

#include "Synch/Synch/AtomicLong.h"

///////////////////////////////////////////////////////////////////////////////////
// Interfaces implemented correctly

// added by yma, 8/29/01
#ifndef __IUnknown_FWD_DEFINED__
#define __IUnknown_FWD_DEFINED__
typedef struct IUnknown IUnknown;
#endif 	/* __IUnknown_FWD_DEFINED__ */

#ifndef __IUnknown_INTERFACE_DEFINED__
#define __IUnknown_INTERFACE_DEFINED__
// IUnknown - The root interface of all COM interfaces
EXTERN_C const IID IID_IUnknown;
struct IUnknown 
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void ** ppvObject) = 0;
        
    virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;
        
    virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
};

typedef /* [unique] */ IUnknown __RPC_FAR *LPUNKNOWN;

#endif // __IUnknown_INTERFACE_DEFINED__

// IClassFactory - interface implemented by COM class objects
EXTERN_C const IID IID_IClassFactory;
struct IClassFactory : public IUnknown 
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance(
	/* [unique][in] */ IUnknown __RPC_FAR *pUnkOuter,
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer(
	/* [in] */ BOOL fLock) = 0;
};

typedef /* [unique] */ IClassFactory __RPC_FAR *LPCLASSFACTORY;


///////////////////////////////////////////////////////////////////////////////////
// Interfaces implemented wrongly

// The interfaces included here are referenced in the Intelligence Server code
// but are never invoked.  So we need to declare the interfaces, but we do not
// give them the correct definition.

// IDispatch - Automation interface
// Since the Server does not use late binding there is no need to implement this
// interface in the UNIX port
// We declare it to be an extension of IUnknown (so that casts still work)

// SAFEARRAY related definition
typedef struct  tagSAFEARRAYBOUND
    {
    ULONG cElements;
    LONG lLbound;
    }	SAFEARRAYBOUND;

typedef struct tagSAFEARRAYBOUND __RPC_FAR *LPSAFEARRAYBOUND;

typedef struct  tagSAFEARRAY
    {
    USHORT cDims;
    USHORT fFeatures;
    ULONG cbElements;
	MSynch::AtomicLong cLocks;
    PVOID pvData;
    SAFEARRAYBOUND rgsabound[ 1 ];
    }	SAFEARRAY;

typedef /* [wire_marshal] */ SAFEARRAY __RPC_FAR *LPSAFEARRAY;

#define	FADF_AUTO	( 0x1 )

#define	FADF_STATIC	( 0x2 )

#define	FADF_EMBEDDED	( 0x4 )

#define	FADF_FIXEDSIZE	( 0x10 )

#define	FADF_RECORD	( 0x20 )

#define	FADF_HAVEIID	( 0x40 )

#define	FADF_HAVEVARTYPE	( 0x80 )

#define	FADF_BSTR	( 0x100 )

#define	FADF_UNKNOWN	( 0x200 )

#define	FADF_DISPATCH	( 0x400 )

#define	FADF_VARIANT	( 0x800 )

#define	FADF_RESERVED	( 0xf008 )


struct IDispatch;

#include "MsiCOM/MsiCOM/Msi_variant.h"
///////////////////////////////////////////////////////////////////////////////
// IEnumVARIANT
EXTERN_C const IID IID_IEnumVARIANT;

struct IEnumVARIANT : public IUnknown
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ VARIANT __RPC_FAR *rgVar,
            /* [out] */ ULONG __RPC_FAR *pCeltFetched) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumVARIANT __RPC_FAR *__RPC_FAR *ppEnum) = 0;    
};

typedef /* [unique] */ IEnumVARIANT __RPC_FAR *LPENUMVARIANT;


///////////////////////////////////////////////////////////////////////////////
// IErrorInfo
EXTERN_C const IID IID_IErrorInfo;

struct IErrorInfo : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [out] */ GUID __RPC_FAR *pGUID) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetSource( 
            /* [out] */ BSTR __RPC_FAR *pBstrSource) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR __RPC_FAR *pBstrDescription) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetHelpFile( 
            /* [out] */ BSTR __RPC_FAR *pBstrHelpFile) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE GetHelpContext( 
            /* [out] */ DWORD __RPC_FAR *pdwHelpContext) = 0;    
};

typedef /* [unique] */ IErrorInfo __RPC_FAR *LPERRORINFO;

///////////////////////////////////////////////////////////////////////////////
// ICreateErrorInfo

EXTERN_C const IID IID_ICreateErrorInfo;

struct ICreateErrorInfo : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetGUID( 
            /* [in] */ REFGUID rguid) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE SetSource( 
            /* [in] */ LPOLESTR szSource) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ LPOLESTR szDescription) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE SetHelpFile( 
            /* [in] */ LPOLESTR szHelpFile) = 0;
        
    virtual HRESULT STDMETHODCALLTYPE SetHelpContext( 
            /* [in] */ DWORD dwHelpContext) = 0;
};

typedef /* [unique] */ ICreateErrorInfo __RPC_FAR *LPCREATEERRORINFO;


///////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo
EXTERN_C const IID IID_ISupportErrorInfo;

struct ISupportErrorInfo : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo( 
            /* [in] */ REFIID riid) = 0;    
};

typedef /* [unique] */ ISupportErrorInfo __RPC_FAR *LPSUPPORTERRORINFO;


/////////////////////////////////////////////////////////////
// 9/7/01 YMA 
// SAFEARRAY

typedef struct  tagIDLDESC
    {
    ULONG dwReserved;
    USHORT wIDLFlags;
    }	IDLDESC;

typedef struct tagIDLDESC __RPC_FAR *LPIDLDESC;

typedef LONG DISPID;

typedef DISPID MEMBERID;

typedef DWORD HREFTYPE;

typedef /* [v1_enum] */ 
enum tagTYPEKIND
    {	TKIND_ENUM	= 0,
	TKIND_RECORD	= TKIND_ENUM + 1,
	TKIND_MODULE	= TKIND_RECORD + 1,
	TKIND_INTERFACE	= TKIND_MODULE + 1,
	TKIND_DISPATCH	= TKIND_INTERFACE + 1,
	TKIND_COCLASS	= TKIND_DISPATCH + 1,
	TKIND_ALIAS	= TKIND_COCLASS + 1,
	TKIND_UNION	= TKIND_ALIAS + 1,
	TKIND_MAX	= TKIND_UNION + 1
    }	TYPEKIND;

typedef struct  tagTYPEDESC
    {
    /* [switch_is][switch_type] */ union 
        {
        /* [case()] */ struct tagTYPEDESC __RPC_FAR *lptdesc;
        /* [case()] */ struct tagARRAYDESC __RPC_FAR *lpadesc;
        /* [case()] */ HREFTYPE hreftype;
        /* [default] */  /* Empty union arm */ 
        }	;
    VARTYPE vt;
    }	TYPEDESC;


typedef struct  tagTYPEATTR
    {
    GUID guid;
    LCID lcid;
    DWORD dwReserved;
    MEMBERID memidConstructor;
    MEMBERID memidDestructor;
    LPOLESTR lpstrSchema;
    ULONG cbSizeInstance;
    TYPEKIND typekind;
    WORD cFuncs;
    WORD cVars;
    WORD cImplTypes;
    WORD cbSizeVft;
    WORD cbAlignment;
    WORD wTypeFlags;
    WORD wMajorVerNum;
    WORD wMinorVerNum;
    TYPEDESC tdescAlias;
    IDLDESC idldescType;
    }	TYPEATTR;

typedef struct tagTYPEATTR __RPC_FAR *LPTYPEATTR;

#ifndef __ITypeInfo_FWD_DEFINED__
#define __ITypeInfo_FWD_DEFINED__
typedef struct ITypeInfo ITypeInfo;
#endif 	/* __ITypeInfo_FWD_DEFINED__ */

#ifndef __IRecordInfo_FWD_DEFINED__
#define __IRecordInfo_FWD_DEFINED__
typedef struct IRecordInfo IRecordInfo;
#endif 	/* __IRecordInfo_FWD_DEFINED__ */

#ifndef __ITypeComp_FWD_DEFINED__
#define __ITypeComp_FWD_DEFINED__
typedef struct ITypeComp ITypeComp;
#endif 	/* __ITypeComp_FWD_DEFINED__ */

#ifndef __ITypeLib_FWD_DEFINED__
#define __ITypeLib_FWD_DEFINED__
typedef struct ITypeLib ITypeLib;
#endif 	/* __ITypeLib_FWD_DEFINED__ */

/////////////////////////////////////////
// ITypeComp

typedef struct  tagPARAMDESCEX
    {
    ULONG cBytes;
    VARIANTARG varDefaultValue;
    }	PARAMDESCEX;

typedef struct tagPARAMDESCEX __RPC_FAR *LPPARAMDESCEX;

typedef struct  tagPARAMDESC
    {
    LPPARAMDESCEX pparamdescex;
    USHORT wParamFlags;
    }	PARAMDESC;

typedef struct tagPARAMDESC __RPC_FAR *LPPARAMDESC;

#if 0
/* the following is what MIDL knows how to remote */
typedef struct  tagELEMDESC
    {
    TYPEDESC tdesc;
    PARAMDESC paramdesc;
    }	ELEMDESC;

#else /* 0 */
typedef struct tagELEMDESC {
    TYPEDESC tdesc;             /* the type of the element */
    union {
        IDLDESC idldesc;        /* info for remoting the element */
        PARAMDESC paramdesc;    /* info about the parameter */
    };
} ELEMDESC, * LPELEMDESC;
#endif /* 0 */

typedef /* [v1_enum] */ 
enum tagVARKIND
    {	VAR_PERINSTANCE	= 0,
	VAR_STATIC	= VAR_PERINSTANCE + 1,
	VAR_CONST	= VAR_STATIC + 1,
	VAR_DISPATCH	= VAR_CONST + 1
    }	VARKIND;

typedef struct  tagVARDESC
    {
    MEMBERID memid;
    LPOLESTR lpstrSchema;
    /* [switch_is][switch_type] */ union 
        {
        /* [case()] */ ULONG oInst;
        /* [case()] */ VARIANT __RPC_FAR *lpvarValue;
        }	;
    ELEMDESC elemdescVar;
    WORD wVarFlags;
    VARKIND varkind;
    }	VARDESC;

typedef struct tagVARDESC __RPC_FAR *LPVARDESC;

#if 0
/* the following is what MIDL knows how to remote */
typedef struct  tagEXCEPINFO
    {
    WORD wCode;
    WORD wReserved;
    BSTR bstrSource;
    BSTR bstrDescription;
    BSTR bstrHelpFile;
    DWORD dwHelpContext;
    ULONG pvReserved;
    ULONG pfnDeferredFillIn;
    SCODE scode;
    }	EXCEPINFO;

#else /* 0 */
typedef struct tagEXCEPINFO {
    WORD  wCode;
    WORD  wReserved;
    BSTR  bstrSource;
    BSTR  bstrDescription;
    BSTR  bstrHelpFile;
    DWORD dwHelpContext;
    PVOID pvReserved;
    HRESULT (/*__stdcall*/ *pfnDeferredFillIn)(struct tagEXCEPINFO *);
    SCODE scode;
} EXCEPINFO, * LPEXCEPINFO;
#endif /* 0 */

typedef /* [v1_enum] */
enum tagCALLCONV
    {	CC_FASTCALL	= 0,
	CC_CDECL	= 1,
	CC_MSCPASCAL	= CC_CDECL + 1,
	CC_PASCAL	= CC_MSCPASCAL,
	CC_MACPASCAL	= CC_PASCAL + 1,
	CC_STDCALL	= CC_MACPASCAL + 1,
	CC_FPFASTCALL	= CC_STDCALL + 1,
	CC_SYSCALL	= CC_FPFASTCALL + 1,
	CC_MPWCDECL	= CC_SYSCALL + 1,
	CC_MPWPASCAL	= CC_MPWCDECL + 1,
	CC_MAX	= CC_MPWPASCAL + 1
    }	CALLCONV;

typedef /* [v1_enum] */ 
enum tagFUNCKIND
    {	FUNC_VIRTUAL	= 0,
	FUNC_PUREVIRTUAL	= FUNC_VIRTUAL + 1,
	FUNC_NONVIRTUAL	= FUNC_PUREVIRTUAL + 1,
	FUNC_STATIC	= FUNC_NONVIRTUAL + 1,
	FUNC_DISPATCH	= FUNC_STATIC + 1
    }	FUNCKIND;

typedef /* [v1_enum] */ 
enum tagINVOKEKIND
    {	INVOKE_FUNC	= 1,
	INVOKE_PROPERTYGET	= 2,
	INVOKE_PROPERTYPUT	= 4,
	INVOKE_PROPERTYPUTREF	= 8
    }	INVOKEKIND;

typedef struct  tagFUNCDESC
    {
    MEMBERID memid;
    /* [size_is] */ SCODE __RPC_FAR *lprgscode;
    /* [size_is] */ ELEMDESC __RPC_FAR *lprgelemdescParam;
    FUNCKIND funckind;
    INVOKEKIND invkind;
    CALLCONV callconv;
    SHORT cParams;
    SHORT cParamsOpt;
    SHORT oVft;
    SHORT cScodes;
    ELEMDESC elemdescFunc;
    WORD wFuncFlags;
    }	FUNCDESC;

typedef struct tagFUNCDESC __RPC_FAR *LPFUNCDESC;

typedef /* [v1_enum] */ 
enum tagDESCKIND
    {	DESCKIND_NONE	= 0,
	DESCKIND_FUNCDESC	= DESCKIND_NONE + 1,
	DESCKIND_VARDESC	= DESCKIND_FUNCDESC + 1,
	DESCKIND_TYPECOMP	= DESCKIND_VARDESC + 1,
	DESCKIND_IMPLICITAPPOBJ	= DESCKIND_TYPECOMP + 1,
	DESCKIND_MAX	= DESCKIND_IMPLICITAPPOBJ + 1
    }	DESCKIND;

typedef union tagBINDPTR
    {
    FUNCDESC __RPC_FAR *lpfuncdesc;
    VARDESC __RPC_FAR *lpvardesc;
    ITypeComp __RPC_FAR *lptcomp;
    }	BINDPTR;

typedef union tagBINDPTR __RPC_FAR *LPBINDPTR;

typedef struct  tagDISPPARAMS
    {
    /* [size_is] */ VARIANTARG __RPC_FAR *rgvarg;
    /* [size_is] */ DISPID __RPC_FAR *rgdispidNamedArgs;
    UINT cArgs;
    UINT cNamedArgs;
    }	DISPPARAMS;

typedef /* [unique] */ ITypeComp __RPC_FAR *LPTYPECOMP;

EXTERN_C const IID IID_ITypeComp;

struct    ITypeComp : public IUnknown
{
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Bind( 
            /* [in] */ LPOLESTR szName,
            /* [in] */ ULONG lHashVal,
            /* [in] */ WORD wFlags,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo,
            /* [out] */ DESCKIND __RPC_FAR *pDescKind,
            /* [out] */ BINDPTR __RPC_FAR *pBindPtr) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindType( 
            /* [in] */ LPOLESTR szName,
            /* [in] */ ULONG lHashVal,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo,
            /* [out] */ ITypeComp __RPC_FAR *__RPC_FAR *ppTComp) = 0;
        
};

// added by yma, 8/29/01
#ifndef __IDispatch_FWD_DEFINED__
#define __IDispatch_FWD_DEFINED__
typedef struct IDispatch IDispatch;
#endif 	/* __IDispatch_FWD_DEFINED__ */

EXTERN_C const IID IID_IDispatch;
struct IDispatch : public IUnknown
{
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr) = 0;
};

typedef /* [unique] */ IDispatch __RPC_FAR *LPDISPATCH;

//////////////////////////////////////////////
// ITypeLib


typedef /* [v1_enum] */ 
enum tagSYSKIND
    {	SYS_WIN16	= 0,
	SYS_WIN32	= SYS_WIN16 + 1,
	SYS_MAC	= SYS_WIN32 + 1
    }	SYSKIND;

typedef /* [v1_enum] */ 
enum tagLIBFLAGS
    {	LIBFLAG_FRESTRICTED	= 0x1,
	LIBFLAG_FCONTROL	= 0x2,
	LIBFLAG_FHIDDEN	= 0x4,
	LIBFLAG_FHASDISKIMAGE	= 0x8
    }	LIBFLAGS;

typedef /* [unique] */ ITypeLib __RPC_FAR *LPTYPELIB;

typedef struct  tagTLIBATTR
    {
    GUID guid;
    LCID lcid;
    SYSKIND syskind;
    WORD wMajorVerNum;
    WORD wMinorVerNum;
    WORD wLibFlags;
    }	TLIBATTR;

typedef struct tagTLIBATTR __RPC_FAR *LPTLIBATTR;

EXTERN_C const IID IID_ITypeLib;

struct    ITypeLib : public IUnknown
{
    public:
        virtual /* [local] */ UINT STDMETHODCALLTYPE GetTypeInfoCount( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
            /* [in] */ UINT index,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfoType( 
            /* [in] */ UINT index,
            /* [out] */ TYPEKIND __RPC_FAR *pTKind) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfoOfGuid( 
            /* [in] */ REFGUID guid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTinfo) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetLibAttr( 
            /* [out] */ TLIBATTR __RPC_FAR *__RPC_FAR *ppTLibAttr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeComp( 
            /* [out] */ ITypeComp __RPC_FAR *__RPC_FAR *ppTComp) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDocumentation( 
            /* [in] */ INT index,
            /* [out] */ BSTR __RPC_FAR *pBstrName,
            /* [out] */ BSTR __RPC_FAR *pBstrDocString,
            /* [out] */ DWORD __RPC_FAR *pdwHelpContext,
            /* [out] */ BSTR __RPC_FAR *pBstrHelpFile) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE IsName( 
            /* [out][in] */ LPOLESTR szNameBuf,
            /* [in] */ ULONG lHashVal,
            /* [out] */ BOOL __RPC_FAR *pfName) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE FindName( 
            /* [out][in] */ LPOLESTR szNameBuf,
            /* [in] */ ULONG lHashVal,
            /* [length_is][size_is][out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo,
            /* [length_is][size_is][out] */ MEMBERID __RPC_FAR *rgMemId,
            /* [out][in] */ USHORT __RPC_FAR *pcFound) = 0;
        
        virtual /* [local] */ void STDMETHODCALLTYPE ReleaseTLibAttr( 
            /* [in] */ TLIBATTR __RPC_FAR *pTLibAttr) = 0;
        
};


/////////////////////////////////////
// ITypeInfo
typedef /* [unique] */ ITypeInfo __RPC_FAR *LPTYPEINFO;

EXTERN_C const IID IID_ITypeInfo;

struct    ITypeInfo : public IUnknown
{
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetTypeAttr( 
            /* [out] */ TYPEATTR __RPC_FAR *__RPC_FAR *ppTypeAttr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeComp( 
            /* [out] */ ITypeComp __RPC_FAR *__RPC_FAR *ppTComp) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetFuncDesc( 
            /* [in] */ UINT index,
            /* [out] */ FUNCDESC __RPC_FAR *__RPC_FAR *ppFuncDesc) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetVarDesc( 
            /* [in] */ UINT index,
            /* [out] */ VARDESC __RPC_FAR *__RPC_FAR *ppVarDesc) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetNames( 
            /* [in] */ MEMBERID memid,
            /* [length_is][size_is][out] */ BSTR __RPC_FAR *rgBstrNames,
            /* [in] */ UINT cMaxNames,
            /* [out] */ UINT __RPC_FAR *pcNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRefTypeOfImplType( 
            /* [in] */ UINT index,
            /* [out] */ HREFTYPE __RPC_FAR *pRefType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetImplTypeFlags( 
            /* [in] */ UINT index,
            /* [out] */ INT __RPC_FAR *pImplTypeFlags) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [size_is][out] */ MEMBERID __RPC_FAR *pMemId) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ PVOID pvInstance,
            /* [in] */ MEMBERID memid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDocumentation( 
            /* [in] */ MEMBERID memid,
            /* [out] */ BSTR __RPC_FAR *pBstrName,
            /* [out] */ BSTR __RPC_FAR *pBstrDocString,
            /* [out] */ DWORD __RPC_FAR *pdwHelpContext,
            /* [out] */ BSTR __RPC_FAR *pBstrHelpFile) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDllEntry( 
            /* [in] */ MEMBERID memid,
            /* [in] */ INVOKEKIND invKind,
            /* [out] */ BSTR __RPC_FAR *pBstrDllName,
            /* [out] */ BSTR __RPC_FAR *pBstrName,
            /* [out] */ WORD __RPC_FAR *pwOrdinal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRefTypeInfo( 
            /* [in] */ HREFTYPE hRefType,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE AddressOfMember( 
            /* [in] */ MEMBERID memid,
            /* [in] */ INVOKEKIND invKind,
            /* [out] */ PVOID __RPC_FAR *ppv) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ PVOID __RPC_FAR *ppvObj) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMops( 
            /* [in] */ MEMBERID memid,
            /* [out] */ BSTR __RPC_FAR *pBstrMops) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetContainingTypeLib( 
            /* [out] */ ITypeLib __RPC_FAR *__RPC_FAR *ppTLib,
            /* [out] */ UINT __RPC_FAR *pIndex) = 0;
        
        virtual /* [local] */ void STDMETHODCALLTYPE ReleaseTypeAttr( 
            /* [in] */ TYPEATTR __RPC_FAR *pTypeAttr) = 0;
        
        virtual /* [local] */ void STDMETHODCALLTYPE ReleaseFuncDesc( 
            /* [in] */ FUNCDESC __RPC_FAR *pFuncDesc) = 0;
        
        virtual /* [local] */ void STDMETHODCALLTYPE ReleaseVarDesc( 
            /* [in] */ VARDESC __RPC_FAR *pVarDesc) = 0;
        
};


typedef /* [unique] */ IRecordInfo __RPC_FAR *LPRECORDINFO;


EXTERN_C const IID IID_IRecordInfo;

struct    IRecordInfo : public IUnknown
{
	public:
        virtual HRESULT STDMETHODCALLTYPE RecordInit( 
            /* [out] */ PVOID pvNew) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecordClear( 
            /* [in] */ PVOID pvExisting) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecordCopy( 
            /* [in] */ PVOID pvExisting,
            /* [out] */ PVOID pvNew) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGuid( 
            /* [out] */ GUID __RPC_FAR *pguid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ ULONG __RPC_FAR *pcbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTypeInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetField( 
            /* [in] */ PVOID pvData,
            /* [in] */ LPCOLESTR szFieldName,
            /* [out] */ VARIANT __RPC_FAR *pvarField) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFieldNoCopy( 
            /* [in] */ PVOID pvData,
            /* [in] */ LPCOLESTR szFieldName,
            /* [out] */ VARIANT __RPC_FAR *pvarField,
            /* [out] */ PVOID __RPC_FAR *ppvDataCArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutField( 
            /* [in] */ ULONG wFlags,
            /* [out][in] */ PVOID pvData,
            /* [in] */ LPCOLESTR szFieldName,
            /* [in] */ VARIANT __RPC_FAR *pvarField) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutFieldNoCopy( 
            /* [in] */ ULONG wFlags,
            /* [out][in] */ PVOID pvData,
            /* [in] */ LPCOLESTR szFieldName,
            /* [in] */ VARIANT __RPC_FAR *pvarField) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFieldNames( 
            /* [out][in] */ ULONG __RPC_FAR *pcNames,
            /* [length_is][size_is][out] */ BSTR __RPC_FAR *rgBstrNames) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsMatchingType( 
            /* [in] */ IRecordInfo __RPC_FAR *pRecordInfo) = 0;
        
        virtual PVOID STDMETHODCALLTYPE RecordCreate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecordCreateCopy( 
            /* [in] */ PVOID pvSource,
            /* [out] */ PVOID __RPC_FAR *ppvDest) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecordDestroy( 
            /* [in] */ PVOID pvRecord) = 0;
        
};



#include "MsiCOM/MsiCOM/Msi_objidl.h"

#endif /* __OAIDL_H__ */
