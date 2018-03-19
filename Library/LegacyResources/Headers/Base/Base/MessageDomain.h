//==============================================================================================
//	FILENAME	:	MessageDomain.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2005-03-11
//	Copyright (C) MicroStrategy Incorporated 2005
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_MessageDomain_h
#define MBase_MessageDomain_h

#include "Base.h"

namespace MBase
{
	/**
	 * Message domain describes which API a message belongs to.
	 * 
	 * IMPORTANT:
	 *
	 * Given a message ID (such as 123), and a message domain, it must be
	 * absolutely non-ambiguous what this message is and what it means.
	 * This is why for example MESSAGE_DOMAIN_ERRNO has a different value
	 * on different platforms. See MessageDomain.cpp.
	 *
	 * Note: for each new domain, you should add a new Bundle in the Resource project.
	 */
	typedef unsigned short MessageDomain;

	// This value is reserved for special uses.
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_UNKNOWN;

	// For error codes returned by errno.
	// This is an alias for a platform-specific domain.
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_ERRNO;

#if defined(WIN32)
	// For error codes returned by Win32 APIs (via ::GetLastError() call)
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_WIN32_API;

	// For standard (structured) exceptions
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_WIN32_EXCEPTION;
#else
	// For error messages returned by dl* functions (via ::dlerror())
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_UNIX_DLERROR;

	// For UNIX signals.
	// This is an alias for a platform-specific domain.
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_UNIX_SIGNAL;
#endif // WIN32

	// For MicroStrategy server messages
	DLL_BASE_EXIM extern const MessageDomain MESSAGE_DOMAIN_SERVER;

	/**
	 * Convert a MessageDomain to a string.
	 */
	DLL_BASE_EXIM const char* ToString(MBase::MessageDomain iDomain);

	/**
	 * Convert a string to a MessageDomain.
	 */
	DLL_BASE_EXIM MBase::MessageDomain ToMessageDomain(const char* ipDomain);
}

#endif // MBase_MessageDomain_h
