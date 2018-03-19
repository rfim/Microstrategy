// File:	ole2.h
// Date:	10th July 2001
// Author:	Will Hurwood
// Modified by: Liqun Jin
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'ole2.h' header file used in VC++
// We make minimal effort to declare symbols and functions in the same file as
// used in Windows.

// This file is used to declare functions like CoCreateInstance that C++ code
// uses to communicate with COM code.  This in the COM SDK in Windows terminology.

#ifndef __OLE2_H__
#define __OLE2_H__

#include "MsiCOM/MsiCOM/MsiCOM.h"

#include "MsiCOM/MsiCOM/Msi_oaidl.h"
#include "MsiCOM/MsiCOM/Msi_ocidl.h"
#include "PDCHeader/PDCwinerror.h"

// These macros are used in declaration of COM SDK functions
#define WINOLEAPI        STDAPI
#define WINOLEAPI_(type) STDAPI_(type)

#include "MsiCOM/MsiCOM/Msi_objbase.h"

// VC++ extension uses __declspec to associate GUID with class
// We rarely make use of this in our code but standard headers declare this symbol
#ifndef WIN32
#define __declspec(x)
#endif // WIN32




////////////////////////////////////////////////////////////////////////////////////
// Enumerations

// Enumeration used by CoCreateInstance
typedef enum tagCLSCTX {
    CLSCTX_INPROC_SERVER= 0x1,
    CLSCTX_INPROC_HANDLER= 0x2,
    CLSCTX_LOCAL_SERVER= 0x4,
    CLSCTX_INPROC_SERVER16= 0x8,
    CLSCTX_REMOTE_SERVER= 0x10,
    CLSCTX_INPROC_HANDLER16= 0x20,
    CLSCTX_INPROC_SERVERX86= 0x40,
    CLSCTX_INPROC_HANDLERX86= 0x80,
    CLSCTX_ESERVER_HANDLER= 0x100
} CLSCTX;

// COM initialization flags; passed to CoInitialize.
typedef enum tagCOINIT {
    COINIT_APARTMENTTHREADED  = 0x2,      // Apartment model
    COINIT_MULTITHREADED      = 0x0,      // OLE calls objects on any thread.
    COINIT_DISABLE_OLE1DDE    = 0x4,      // Don't use DDE for Ole1 support.
    COINIT_SPEED_OVER_MEMORY  = 0x8      // Trade memory for speed.
} COINIT;


////////////////////////////////////////////////////////////////////////////////////
// Selected COM SDK functions

// Create an instance of a COM object
WINOLEAPI DLL_MSICOM_EXIM CoCreateInstance(REFCLSID rclsid,
			   LPUNKNOWN pUnkOuter,
			   DWORD dwClsContext,
			   REFIID riid,
			   LPVOID FAR* ppv);

// Get the class object for a COM object
WINOLEAPI DLL_MSICOM_EXIM CoGetClassObject(REFCLSID rclsid,
			    DWORD dwClsContext,
			    LPVOID pvReserved,
			    REFIID riid,
			    LPVOID FAR* ppv);

// Start up and shut down COM on a thread
WINOLEAPI DLL_MSICOM_EXIM CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
WINOLEAPI DLL_MSICOM_EXIM CoInitialize(LPVOID pvReserved);
WINOLEAPI_(void) DLL_MSICOM_EXIM CoUninitialize(void);


#endif /* __OLE2_H__ */
