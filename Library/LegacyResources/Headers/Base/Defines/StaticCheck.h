//==============================================================================================
//	FILENAME	: StaticCheck.h
//	AUTHOR		: Cezary Raczko
//	CREATION	: 4/4/03
//	Copyright (C) MicroStrategy Incorporated 2003
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StaticCheck_h
#define MBase_StaticCheck_h

////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copyright
//     notice appear in all copies and that both that copyright notice and this
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

namespace Base
{
#ifdef __hpux
#else
    template<bool> struct CompileTimeChecker;
    template<> struct CompileTimeChecker<true>
    {
        typedef void type;
    };
#endif
}

/**
 * Macro STATIC_CHECK
 * Invocation: STATIC_CHECK(expr)
 *   @param: expr is a compile-time integral or pointer expression.
*/

#define STATIC_CHECK(expr) typedef char ERROR_5B84D18BADD040F6B55A64F8BCF5A3EA[1][(expr) ? 1 : 0]

#endif // MBase_StaticCheck_h
