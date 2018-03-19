//==============================================================================================
//	FILENAME	:	BaseException.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
//
//   #####  ###### #####  #####  ######  #####  ####  ##### ###### #####
//   #    # #      #    # #    # #      #      #    #   #   #      #    #
//   #    # #####  #    # #    # #####  #      #    #   #   #####  #    #
//   #    # #      #####  #####  #      #      ######   #   #      #    #
//   #    # #      #      #   #  #      #      #    #   #   #      #    #
//   #####  ###### #      #    # ######  ##### #    #   #   ###### #####
//
#ifndef MBase_BaseException_h
#define MBase_BaseException_h

#ifdef __ANDROID__
	#include "Base/Base/BaseString.h"
#else
	#include "PDCHeader/PDCwchar.h" // for wchar_t
	#include "Base/Base/LastError.h"

	#if defined(GetMessage)
	#error Please make sure that you include <winbase.h>/<windows.h> properly (via Common/PDCHeader)
	#endif

	#include "../Base/Message.h"
#endif

namespace MBase 
{
#ifdef __ANDROID__
	typedef String BaseException;
#else
	typedef Message BaseException;
#endif
}

#endif // MBase_BaseException_h
