// File:	oleauto.h
// Date:	01/08/2001
// Author:	Yuling Ma
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'oleauto.h' header file used in VC++
// We make minimal effort to declare symbols and functions in the same file as
// used in Windows.


#if !defined( _OLEAUTO_H_ )
#define _OLEAUTO_H_

#include "MsiCOM/MsiCOM/MsiCOM.h"
#include "PDCHeader/PDCwtypes.h"
#include "MsiCOM/MsiCOM/Msi_bstr.h"
#include "MsiCOM/MsiCOM/Msi_oaidl.h"

//  Definition of the OLE Automation APIs, and macros.

//#ifdef _OLEAUT32_
/*
#define WINOLEAUTAPI        STDAPI
#define WINOLEAUTAPI_(type) STDAPI_(type)
*/
/*#else
#define WINOLEAUTAPI        EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define WINOLEAUTAPI_(type) EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
#endif
*/

/*---------------------------------------------------------------------*/
/*                            BSTR API                                 */
/*---------------------------------------------------------------------*/
/*
WINOLEAUTAPI_(BSTR) SysAllocString(const OLECHAR *);
WINOLEAUTAPI_(INT)  SysReAllocString(BSTR *, const OLECHAR *);
WINOLEAUTAPI_(BSTR) SysAllocStringLen(const OLECHAR *, UINT);
WINOLEAUTAPI_(INT)  SysReAllocStringLen(BSTR *, const OLECHAR *, UINT);
WINOLEAUTAPI_(void) SysFreeString(BSTR);
WINOLEAUTAPI_(UINT) SysStringLen(BSTR);

#ifdef _WIN32
WINOLEAUTAPI_(UINT) SysStringByteLen(BSTR bstr);
WINOLEAUTAPI_(BSTR) SysAllocStringByteLen(LPCSTR psz, UINT len);
#endif

*/

/* Flags for IDispatch::Invoke */
#define DISPATCH_METHOD         0x1
#define DISPATCH_PROPERTYGET    0x2
#define DISPATCH_PROPERTYPUT    0x4
#define DISPATCH_PROPERTYPUTREF 0x8

// ============================================================================
// ErrorInfo API

WINOLEAUTAPI DLL_MSICOM_EXIM SetErrorInfo(ULONG dwReserved, IErrorInfo * perrinfo);
WINOLEAUTAPI DLL_MSICOM_EXIM GetErrorInfo(ULONG dwReserved, IErrorInfo ** pperrinfo);
WINOLEAUTAPI DLL_MSICOM_EXIM CreateErrorInfo(ICreateErrorInfo ** pperrinfo);

/*---------------------------------------------------------------------*/
/*                          SafeArray API                              */
/*---------------------------------------------------------------------*/

WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayAllocDescriptor(UINT cDims, SAFEARRAY ** ppsaOut);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayAllocDescriptorEx(VARTYPE vt, UINT cDims, SAFEARRAY ** ppsaOut);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayAllocData(SAFEARRAY * psa);
EXTERN_C DLL_MSICOM_EXIM SAFEARRAY * STDAPICALLTYPE  SafeArrayCreate(VARTYPE vt, UINT cDims, SAFEARRAYBOUND * rgsabound);
EXTERN_C DLL_MSICOM_EXIM SAFEARRAY * STDAPICALLTYPE SafeArrayCreateEx(VARTYPE vt, UINT cDims, SAFEARRAYBOUND * rgsabound, PVOID pvExtra);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayCopyData(SAFEARRAY *psaSource, SAFEARRAY *psaTarget);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayDestroyDescriptor(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayDestroyData(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayDestroy(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayRedim(SAFEARRAY * psa, SAFEARRAYBOUND * psaboundNew);
WINOLEAUTAPI_(UINT) DLL_MSICOM_EXIM SafeArrayGetDim(SAFEARRAY * psa);
WINOLEAUTAPI_(UINT) DLL_MSICOM_EXIM SafeArrayGetElemsize(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetUBound(SAFEARRAY * psa, UINT nDim, LONG * plUbound);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetLBound(SAFEARRAY * psa, UINT nDim, LONG * plLbound);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayLock(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayUnlock(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayAccessData(SAFEARRAY * psa, void HUGEP** ppvData);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayUnaccessData(SAFEARRAY * psa);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetElement(SAFEARRAY * psa, LONG * rgIndices, void * pv);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayPutElement(SAFEARRAY * psa, LONG * rgIndices, void * pv);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayCopy(SAFEARRAY * psa, SAFEARRAY ** ppsaOut);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayPtrOfIndex(SAFEARRAY * psa, LONG * rgIndices, void ** ppvData);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArraySetRecordInfo(SAFEARRAY * psa, IRecordInfo * prinfo);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetRecordInfo(SAFEARRAY * psa, IRecordInfo ** prinfo);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArraySetIID(SAFEARRAY * psa, REFGUID guid);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetIID(SAFEARRAY * psa, GUID * pguid);
WINOLEAUTAPI DLL_MSICOM_EXIM SafeArrayGetVartype(SAFEARRAY * psa, VARTYPE * pvt);
EXTERN_C DLL_MSICOM_EXIM SAFEARRAY * STDAPICALLTYPE SafeArrayCreateVector(VARTYPE vt, LONG lLbound, ULONG cElements);
EXTERN_C DLL_MSICOM_EXIM SAFEARRAY * STDAPICALLTYPE SafeArrayCreateVectorEx(VARTYPE vt, LONG lLbound, ULONG cElements, PVOID pvExtra);
EXTERN_C DLL_MSICOM_EXIM SAFEARRAY * STDAPICALLTYPE MSTRSafeArrayCreateVector(VARTYPE vt, LONG lLbound, ULONG cElements);
WINOLEAUTAPI DLL_MSICOM_EXIM MSTRSafeArrayDestroy(SAFEARRAY * psa);

#endif /* _OLEAUTO_H_ */
