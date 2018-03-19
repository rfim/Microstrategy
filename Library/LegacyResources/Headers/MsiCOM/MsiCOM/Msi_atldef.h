// File:	atldef.h
// Date:	11th July 2001
// Author:	Will Hurwood
// Modified by: Liqun Jin
//
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file replaces the atldef.h file from ATL on UNIX
// File declares similar symbols and classes to the corresponding VC++ file

#ifndef __ATLDEF_H__
#define __ATLDEF_H__


// Disable some ATL macros
#define ATLASSERT(expr)
#define ATLTRY(x)	x;

#define ATL_NO_VTABLE

// These symbols are used for functions that might be internal, or in
// a separate library.  Here we define under assumption that they are
// in a different library.
#define ATLAPI		extern "C" HRESULT
#define ATLAPI_(x)	extern "C" x
#define ATLINLINE

// Gulp - does UNIX compiler support packing?
#define _ATL_PACKING 8
#define offsetofclass(base, derived) ((DWORD_PTR)(static_cast<base*>((derived*)_ATL_PACKING))-_ATL_PACKING)

#ifdef offsetof
#undef offsetof
#endif

#define offsetof(s,m)   (size_t)&(((s *)0)->m)

/////////////////////////////////////////////////////////////////////////////
// Master version numbers

#define _ATL     1      // Active Template Library
#define _ATL_VER 0x0300 // Active Template Library version 3.0


/////////////////////////////////////////////////////////////////////////////
// Threading

#ifndef _ATL_SINGLE_THREADED
#ifndef _ATL_APARTMENT_THREADED
#ifndef _ATL_FREE_THREADED
#define _ATL_FREE_THREADED
#endif
#endif
#endif


#endif // __ATLDEF_H__
