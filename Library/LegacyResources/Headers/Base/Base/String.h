//==============================================================================================
//	FILENAME	:	BaseString.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_String_h
#define MBase_String_h

#include "Allocator.h"

#include "PDCHeader/PDCstring"


// This used to be named String.h, but that conflicted with string.h
// on case-insensitive file systems.


namespace MBase 
{
	/**
	 * MBase::String is just a typedef of std::wstring using MBase::Allocator, so that it 
	 * can be used with MBase::Buffer.
	 */
	typedef std::basic_string< WCHAR, std::char_traits< WCHAR >, Allocator< WCHAR > > String;
}

#endif // MBase_String_h
