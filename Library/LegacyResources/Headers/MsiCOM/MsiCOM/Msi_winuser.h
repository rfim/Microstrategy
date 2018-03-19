// File:	winuser.h
// Date:	8/29/2001
// Author:	Yuling Ma
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved
#ifndef _WINUSER_H_
#define _WINUSER_H_

#include "PDCHeader/PDCwindows.h"

#include "MsiCOM.h"

#ifdef NO_WCHAR_SUPPORT
LPSTR DLL_MSICOM_EXIM WINAPI CharLowerA(LPSTR lpsz);

LPSTR DLL_MSICOM_EXIM WINAPI CharUpperA(LPSTR lpsz);
#else
LPWSTR DLL_MSICOM_EXIM WINAPI CharLowerW(LPWSTR lpsz);

LPWSTR DLL_MSICOM_EXIM WINAPI CharUpperW(LPWSTR lpsz);
#endif
#endif /* _WINUSER_H_ */
