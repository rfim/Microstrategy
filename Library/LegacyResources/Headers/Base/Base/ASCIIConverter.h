//==============================================================================================
//	FILENAME	:	ASCIIConverter.h
//	AUTHOR		:	vovechkin
//	CREATION	:	Nov 18, 2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ASCIIConverter_h
#define MBase_ASCIIConverter_h

#include "Base.h"
#include "ReturnString.h"
#include "PDCHeader/PDCwchar.h"

#include "Base/Interfaces/StringConversionException.h"

namespace MBase
{
	/**
	 * Converts an ASCII string to "wide characters" (wchar_t). Only ASCII values
	 * in the range 0..127 are allowed.
	 *
	 * @param ipASCII An ASCII string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with '?'
	 *		otherwise throws an exception when an illegal character is found.
	 *
	 * @return A converted wide character string
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see WideCharToASCII( const wchar_t*, bool )
	 */
	DLL_BASE_EXIM ReturnWString ASCIIToWideChar( 
		const char* ipASCII, 
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a wide character string to an ASCII string. Only ASCII values
	 * in the range 0..127 are allowed.
	 *
	 * @param ipWide A wide character string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with '?'
	 *		otherwise throws an exception
	 *
	 * @return An ASCII string.
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter is false.
	 *
	 * @see StringConversionException
	 * @see ASCIIToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnString WideCharToASCII( 
		const wchar_t* ipWide, 
		bool ibUseSubstitutionCharacter = false );
}

#endif // MBase_ASCIIConverter_h
