//==============================================================================================
//	FILENAME	:	PDCwinerror.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwinerror_h
#define PDCwinerror_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#include <winerror.h>
#else
	//==============================================================================================
	//	FILENAME	:	winerror.h
	//	AUTHOR		:	Will Hurwood
	//	CREATION	:	7/10/2001
	//	Copyright (C) MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	// This header file stands in for the standard 'winerror.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// This file contains result codes returned by COM methods
	#ifndef __WINERROR_H__
	#define __WINERROR_H__ 1

	#define S_OK                                   (0x00000000)
	#define S_FALSE                                (0x00000001)

	#define SEVERITY_SUCCESS    0
	#define SEVERITY_ERROR      1

	#define NOERROR             0
	//
	// Generic test for success on any status value (non-negative numbers
	// indicate success).
	//
	#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)

	//
	// and the inverse
	//
	#define FAILED(Status) ((HRESULT)(Status)<0)

	#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

	//
	// Create an HRESULT value from component pieces
	//

	#define MAKE_HRESULT(sev,fac,code) \
		((HRESULT) (((unsigned Int32)(sev)<<31) | ((unsigned Int32)(fac)<<16) | ((unsigned Int32)(code))) )
	#define MAKE_SCODE(sev,fac,code) \
		((SCODE) (((unsigned Int32)(sev)<<31) | ((unsigned Int32)(fac)<<16) | ((unsigned Int32)(code))) )

	//
	// Define the facility codes
	//
	#define FACILITY_WINDOWS                 8
	#define FACILITY_STORAGE                 3
	#define FACILITY_SSPI                    9
	#define FACILITY_SETUPAPI                15
	#define FACILITY_RPC                     1
	#define FACILITY_WIN32                   7
	#define FACILITY_CONTROL                 10
	#define FACILITY_NULL                    0
	#define FACILITY_MSMQ                    14
	#define FACILITY_MEDIASERVER             13
	#define FACILITY_INTERNET                12
	#define FACILITY_ITF                     4
	#define FACILITY_DISPATCH                2
	#define FACILITY_CERT                    11

//
// MessageId: ERROR_INSUFFICIENT_BUFFER
//
// MessageText:
//
//  The data area passed to a system call is too
//  small.
//
#define ERROR_INSUFFICIENT_BUFFER        122L    // dderror
#define ERROR_ALREADY_EXISTS             183L

	//
	// MessageId: E_UNEXPECTED
	//
	// MessageText:
	//
	//  Catestrophic failure
	//
	#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFF)

	//
	// MessageId: E_NOTIMPL
	//
	// MessageText:
	//
	//  Not implemented
	//
	#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001)

	//
	// MessageId: E_OUTOFMEMORY
	//
	// MessageText:
	//
	//  Ran out of memory
	//
	#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000E)

	//
	// MessageId: E_INVALIDARG
	//
	// MessageText:
	//
	//  One or more arguments are invalid
	//
	#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057)

	//
	// MessageId: E_NOINTERFACE
	//
	// MessageText:
	//
	//  No such interface supported
	//
	#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002)

	//
	// MessageId: E_POINTER
	//
	// MessageText:
	//
	//  Invalid pointer
	//
	#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003)

	//
	// MessageId: E_HANDLE
	//
	// MessageText:
	//
	//  Invalid handle
	//
	#define E_HANDLE                         _HRESULT_TYPEDEF_(0x80070006)

	//
	// MessageId: E_ABORT
	//
	// MessageText:
	//
	//  Operation aborted
	//
	#define E_ABORT                          _HRESULT_TYPEDEF_(0x80004004)

	//
	// MessageId: E_FAIL
	//
	// MessageText:
	//
	//  Unspecified error
	//
	#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005)

	//
	// MessageId: E_ACCESSDENIED
	//
	// MessageText:
	//
	//  General access denied error
	//
	#define E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80070005)

	//
	// MessageId: CLASS_E_NOAGGREGATION
	//
	// MessageText:
	//
	//  Class does not support aggregation (or class object is remote)
	//
	#define CLASS_E_NOAGGREGATION            _HRESULT_TYPEDEF_(0x80040110)

	//
	// MessageId: REGDB_E_READREGDB
	//
	// MessageText:
	//
	//  Could not read key from registry
	//
	#define REGDB_E_READREGDB                _HRESULT_TYPEDEF_(0x80040150)

	//
	// MessageId: REGDB_E_WRITEREGDB
	//
	// MessageText:
	//
	//  Could not write key to registry
	//
	#define REGDB_E_WRITEREGDB               _HRESULT_TYPEDEF_(0x80040151)

	//
	// MessageId: REGDB_E_KEYMISSING
	//
	// MessageText:
	//
	//  Could not find the key in the registry
	//
	#define REGDB_E_KEYMISSING               _HRESULT_TYPEDEF_(0x80040152)

	//
	// MessageId: REGDB_E_INVALIDVALUE
	//
	// MessageText:
	//
	//  Invalid value for registry
	//
	#define REGDB_E_INVALIDVALUE             _HRESULT_TYPEDEF_(0x80040153)

	//
	// MessageId: REGDB_E_CLASSNOTREG
	//
	// MessageText:
	//
	//  Class not registered
	//
	#define REGDB_E_CLASSNOTREG              _HRESULT_TYPEDEF_(0x80040154)

	//
	// MessageId: REGDB_E_IIDNOTREG
	//
	// MessageText:
	//
	//  Interface not registered
	//
	#define REGDB_E_IIDNOTREG                _HRESULT_TYPEDEF_(0x80040155)

	//
	// MessageId: CLASS_E_NOAGGREGATION
	//
	// MessageText:
	//
	//  Class does not support aggregation (or class object is remote)
	//
	#define CLASS_E_NOAGGREGATION            _HRESULT_TYPEDEF_(0x80040110)

	//
	// MessageId: CLASS_E_CLASSNOTAVAILABLE
	//
	// MessageText:
	//
	//  ClassFactory cannot supply requested class
	//
	#define CLASS_E_CLASSNOTAVAILABLE        _HRESULT_TYPEDEF_(0x80040111)

	//
	// MessageId: CLASS_E_NOTLICENSED
	//
	// MessageText:
	//
	//  Class is not licensed for use
	//
	#define CLASS_E_NOTLICENSED              _HRESULT_TYPEDEF_(0x80040112)

	//
	// MessageId: DISP_E_BADVARTYPE
	//
	// MessageText:
	//
	//  Bad variable type.
	//
	#define DISP_E_BADVARTYPE                _HRESULT_TYPEDEF_(0x80020008)

	//
	// MessageId: DISP_E_EXCEPTION
	//
	// MessageText:
	//
	//  Exception occurred.
	//
	#define DISP_E_EXCEPTION                 _HRESULT_TYPEDEF_(0x80020009)

	//
	// MessageId: DISP_E_OVERFLOW
	//
	// MessageText:
	//
	//  Out of present range.
	//
	#define DISP_E_OVERFLOW                  _HRESULT_TYPEDEF_(0x8002000A)

	//
	// MessageId: DISP_E_TYPEMISMATCH
	//
	// MessageText:
	//
	//  Type mismatch.
	//
	#define DISP_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80020005)

	//
	// MessageId: DISP_E_ARRAYISLOCKED
	//
	// MessageText:
	//
	//  Memory is locked.
	//
	#define DISP_E_ARRAYISLOCKED             _HRESULT_TYPEDEF_(0x8002000D)

	//
	// MessageId: DISP_E_BADINDEX
	//
	// MessageText:
	//
	//  Invalid index.
	//
	#define DISP_E_BADINDEX                  _HRESULT_TYPEDEF_(0x8002000B)

	//
	// MessageId: DISP_E_PARAMNOTFOUND
	//
	// MessageText:
	//
	//  Parameter not found.
	//
	#define DISP_E_PARAMNOTFOUND             _HRESULT_TYPEDEF_(0x80020004)

	//
	// MessageId: ERROR_DISK_FULL
	//
	// MessageText:
	//
	//  There is not enough space on the disk.
	//
	#define ERROR_DISK_FULL                  112

	//
	// MessageId: STG_E_INVALIDFUNCTION
	//
	// MessageText:
	//
	//  Unable to perform requested operation.
	//
	#define STG_E_INVALIDFUNCTION            _HRESULT_TYPEDEF_(0x80030001)

	//
	// MessageId: STG_E_INVALIDPOINTER
	//
	// MessageText:
	//
	//  Invalid pointer error.
	//
	#define STG_E_INVALIDPOINTER             _HRESULT_TYPEDEF_(0x80030009)

	//
	// MessageId: ERROR_SERVICE_SPECIFIC_ERROR
	//
	// MessageText:
	//
	//  The service has returned a service-specific error code.
	//
	#define ERROR_SERVICE_SPECIFIC_ERROR     1066

	#define ERROR_SUCCESS                    0

	#define NO_ERROR 0                                                // dderror

	//nprabhudessai 1/18/06 : need for MSICOM build
#ifdef WIN32
	//
	// MessageId: ERROR_BUFFER_OVERFLOW
	//
	// MessageText:
	//
	//  The file name is too long.
	//
	#define ERROR_BUFFER_OVERFLOW            111L

	//
	// MessageId: ERROR_NO_TOKEN
	//
	// MessageText:
	//
	//  An attempt was made to reference a token that does not exist.
	//
	#define ERROR_NO_TOKEN                   1008L

	//
	// MessageId: WSAEHOSTUNREACH
	//
	// MessageText:
	//
	//  A socket operation was attempted to an unreachable host.
	//
	#define WSAEHOSTUNREACH                  10065L

	//
	// MessageId: WSAECONNRESET
	//
	// MessageText:
	//
	//  An existing connection was forcibly closed by the remote host.
	//
	#define WSAECONNRESET                    10054L
#endif

	#endif /* __WINERROR_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwinerror_h
