//==============================================================================================
//	FILENAME	:	LastError.h
//	AUTHOR		:	vovechkin
//	CREATION	:	Nov 30, 2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_LastError_h
#define MBase_LastError_h

#include "Base.h"
#include "ReturnString.h"

#include "../Defines/BasicTypes.h"

////////////////////////////////////////////////////////////////////////////
//
//   #####  ###### #####  #####  ######  #####  ####  ##### ###### #####
//   #    # #      #    # #    # #      #      #    #   #   #      #    #
//   #    # #####  #    # #    # #####  #      #    #   #   #####  #    #
//   #    # #      #####  #####  #      #      ######   #   #      #    #
//   #    # #      #      #   #  #      #      #    #   #   #      #    #
//   #####  ###### #      #    # ######  ##### #    #   #   ###### #####
//
////////////////////////////////////////////////////////////////////////////
// 2005-04-15 ovechkin
// Do not use these methods, because they are ambiguous on Windows vs UNIX.
//
namespace MBase
{
	typedef unsigned Int32 ErrorCode;

	/**
	 * Insead use:
	 *	errno -- on Windows, UNIX, or 
	 *	::GetLastError() -- on Windows
	 */
	DLL_BASE_EXIM ErrorCode GetLastError() throw();

	/**
	 * Instead use:
	 *	MBase::GetStandardCLibraryErrorDescriptionOrNULL -- for error codes returned by errno, or 
	 *	MBase::GetWin32APIErrorDescriptionOrNULL -- for error codes returned by ::GetLastError()
	 */
	DLL_BASE_EXIM ReturnWString GetSystemErrorDescription(ErrorCode iErrorCode) throw();

	/**
	 * Do not use this one at all!
	 */
	DLL_BASE_EXIM bool IsSystemError(ErrorCode iErrorCode) throw();
}

#endif // MBase_LastError_h
