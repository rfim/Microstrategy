// File:	objbase.h
// Date:	August 14, 2001
// Author:	Liqun Jin
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file replaces the objbase.h file from ATL on UNIX
// File only declares similar symbols and classes to the corresponding VC++ file, which is used by Intelligence Server

#ifndef __OBJBASE_H__
#define __OBJBASE_H__

#include "MsiCOM/MsiCOM/Msi_ole2.h"
#include "PDCHeader/PDCrpc.h"

#include "Base/Base/GUID.h"
// Make the GUID operators part of the global namespace (as they are in COM)
using MBase::operator ==;
using MBase::operator !=;
using MBase::IsEqualGUID;

// COM Memory Allocation
WINOLEAPI_(LPVOID) DLL_MSICOM_EXIM CoTaskMemAlloc(ULONG cb);
WINOLEAPI_(LPVOID) DLL_MSICOM_EXIM CoTaskMemRealloc(LPVOID pv, ULONG cb);
WINOLEAPI_(void)   DLL_MSICOM_EXIM CoTaskMemFree(LPVOID pv);

// Marshaling
WINOLEAPI DLL_MSICOM_EXIM CoGetMarshalSizeMax(ULONG *pulSize, REFIID riid, LPUNKNOWN pUnk,
                    DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags);
WINOLEAPI DLL_MSICOM_EXIM CoMarshalInterface(LPSTREAM pStm, REFIID riid, LPUNKNOWN pUnk,
                    DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags);
WINOLEAPI DLL_MSICOM_EXIM CoUnmarshalInterface(LPSTREAM pStm, REFIID riid, LPVOID FAR* ppv);
WINOLEAPI DLL_MSICOM_EXIM CoMarshalHresult(LPSTREAM pstm, HRESULT hresult);
WINOLEAPI DLL_MSICOM_EXIM CoUnmarshalHresult(LPSTREAM pstm, HRESULT FAR * phresult);
WINOLEAPI DLL_MSICOM_EXIM CoReleaseMarshalData(LPSTREAM pStm);
WINOLEAPI DLL_MSICOM_EXIM CoDisconnectObject(LPUNKNOWN pUnk, DWORD dwReserved);
WINOLEAPI DLL_MSICOM_EXIM CoLockObjectExternal(LPUNKNOWN pUnk, BOOL fLock, BOOL fLastUnlockReleases);
WINOLEAPI DLL_MSICOM_EXIM CoGetStandardMarshal(REFIID riid, LPUNKNOWN pUnk,
                    DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags,
                    LPMARSHAL FAR* ppMarshal);

WINOLEAPI DLL_MSICOM_EXIM CoGetStdMarshalEx(LPUNKNOWN pUnkOuter, DWORD smexflags,
                            LPUNKNOWN FAR* ppUnkInner);
WINOLEAPI DLL_MSICOM_EXIM CoGetStaticMarshal(IUnknown *pUnkControl,
                             ULONG        cItfs,
                             IID        **arIIDs,
                             UUID       **arIPIDs,
                             DWORD        dwBindingFlags,
                             ULONG        cBindings,
                             LPUNKNOWN   *pBindings,
                             IUnknown   **ppUnkInner);

/* flags for CoGetStdMarshalEx */
typedef enum tagSTDMSHLFLAGS
{
    SMEXF_SERVER     = 0x01,       // server side aggregated std marshaler
    SMEXF_HANDLER    = 0x02        // client side (handler) agg std marshaler
} STDMSHLFLAGS;


WINOLEAPI_(BOOL) DLL_MSICOM_EXIM CoIsHandlerConnected(LPUNKNOWN pUnk);
WINOLEAPI_(BOOL) DLL_MSICOM_EXIM CoHasStrongExternalConnections(LPUNKNOWN pUnk);

// Apartment model inter-thread interface passing helpers
WINOLEAPI DLL_MSICOM_EXIM CoMarshalInterThreadInterfaceInStream(REFIID riid, LPUNKNOWN pUnk,
                    LPSTREAM *ppStm);

WINOLEAPI DLL_MSICOM_EXIM CoGetInterfaceAndReleaseStream(LPSTREAM pStm, REFIID iid,
                    LPVOID FAR* ppv);

WINOLEAPI DLL_MSICOM_EXIM CoCreateFreeThreadedMarshaler(LPUNKNOWN  punkOuter,
                    LPUNKNOWN *ppunkMarshal);


////////////////////////////////
// added by yma, 8/29/01
WINOLEAPI DLL_MSICOM_EXIM StringFromCLSID(REFCLSID rclsid, LPOLESTR FAR* lplpsz);
WINOLEAPI DLL_MSICOM_EXIM CLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid);
WINOLEAPI DLL_MSICOM_EXIM StringFromIID(REFIID rclsid, LPOLESTR FAR* lplpsz);
WINOLEAPI DLL_MSICOM_EXIM IIDFromString(LPOLESTR lpsz, LPIID lpiid);
WINOLEAPI_(BOOL) DLL_MSICOM_EXIM CoIsOle1Class(REFCLSID rclsid);
WINOLEAPI DLL_MSICOM_EXIM ProgIDFromCLSID (REFCLSID clsid, LPOLESTR FAR* lplpszProgID);
WINOLEAPI DLL_MSICOM_EXIM CLSIDFromProgID (LPCOLESTR lpszProgID, LPCLSID lpclsid);
WINOLEAPI_(int) DLL_MSICOM_EXIM StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cbMax);

WINOLEAPI DLL_MSICOM_EXIM CoCreateGuid(GUID FAR *pguid);

#ifndef HUGEP
#define HUGEP
#endif




#endif
