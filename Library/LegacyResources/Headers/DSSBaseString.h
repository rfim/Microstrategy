/*
 *  String.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/13/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef MBase_CString_h
#define MBase_CString_h

#import <string>

//#include "Allocator.h"

//#include "PDCHeader/PDCstring"

namespace MBase 
{
	/**
	 * MBase::String is just a typedef of std::wstring using MBase::Allocator, so that it 
	 * can be used with MBase::Buffer.
	 */
	//typedef std::basic_string< wchar_t, std::char_traits< wchar_t >, Allocator< wchar_t > > String;
	typedef std::string CString;
}

#endif // MBase_String_h
