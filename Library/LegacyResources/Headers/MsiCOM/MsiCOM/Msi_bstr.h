// File:	bstr.h
// Date:	08/20/2001
// Author:	Yuling Ma
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file stands in for the standard 'bstr.h' header file used in VC++
// We make minimal effort to declare symbols and functions in the same file as
// used in Windows.


#if !defined( _BSTR_H_ )
#define _BSTR_H_

#include "MsiCOM/MsiCOM/MsiCOM.h"
#include "PDCHeader/PDCwtypes.h"

//  Definition of the OLE Automation APIs, and macros.

#define WINOLEAUTAPI        STDAPI
#define WINOLEAUTAPI_(type) STDAPI_(type)


/*---------------------------------------------------------------------*/
/*                            BSTR API                                 */
/*---------------------------------------------------------------------*/

WINOLEAUTAPI_(BSTR) DLL_MSICOM_EXIM SysAllocString(const OLECHAR *);
WINOLEAUTAPI_(INT)  DLL_MSICOM_EXIM SysReAllocString(BSTR *, const OLECHAR *);
WINOLEAUTAPI_(BSTR) DLL_MSICOM_EXIM SysAllocStringLen(const OLECHAR *, UINT);
WINOLEAUTAPI_(INT)  DLL_MSICOM_EXIM SysReAllocStringLen(BSTR *, const OLECHAR *, UINT);
WINOLEAUTAPI_(void) DLL_MSICOM_EXIM SysFreeString(BSTR);
WINOLEAUTAPI_(UINT) DLL_MSICOM_EXIM SysStringLen(BSTR);

WINOLEAUTAPI_(UINT) DLL_MSICOM_EXIM SysStringByteLen(BSTR bstr);
WINOLEAUTAPI_(BSTR) DLL_MSICOM_EXIM SysAllocStringByteLen(LPCSTR psz, UINT len);



#endif /* _BSTR_H_ */
