//==============================================================================================
//	FILENAME	:	PDCrpc.h
//	AUTHOR		:	yma
//	CREATION	:	7/19/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCrpc_h
#define PDCrpc_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <rpc.h>
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	//==============================================================================================
	//	FILENAME	:	rpc.h
	//	AUTHOR		:	Will Hurwood
	//	CREATION	:	7/17/2001
	//	Copyright (C) 2001 MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	// This header file stands in for the standard 'rpc.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// We need this file so we can include MIDL generated output without modification
	// The MIDL generated header files include "rpc.h" so we need to have a file.
	// However since the COM port does not need to support remote procedure call
	// all we do is define some symbols used in the MIDL output

	// This file is intended to include basic definitions needed by any rpc application.
	#ifndef __RPC_H__
	#define __RPC_H__

	// Stub out these commonly used symbols
	#define __RPC_FAR
	#define __RPC_API
	#define __RPC_USER
	#define __RPC_STUB
	#define  RPC_ENTRY

	#include "PDCrpcdce.h"

	// Some RPC platforms don't define DECLSPEC_IMPORT
	#if !defined(DECLSPEC_IMPORT)
	#if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
	#define DECLSPEC_IMPORT __declspec(dllimport)
	#else
	#define DECLSPEC_IMPORT
	#endif
	#endif
	#endif /* __RPC_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCrpc_h
