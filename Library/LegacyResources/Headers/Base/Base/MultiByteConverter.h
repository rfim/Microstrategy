//==============================================================================================
//	FILENAME	:	MultiByteConverter.h
//	AUTHOR		:	vovechkin
//	CREATION	:	Nov 18, 2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_MultiByteConverter_h
#define MBase_MultiByteConverter_h

#include "Base.h"
#include "ReturnString.h"
#include "PDCHeader/PDCwchar.h"
#include "Base/Defines/BasicTypes.h"

#include "Base/Interfaces/StringConversionException.h"

/**
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 * Use the *MultiByte* functions with extreme caution.
 * If you are not absolutely sure - use *ASCII* or *UTF8* instead,
 * whichever is more appropriate.
 *
 * The problems with *MultiByte* conversion are:
 *
 *	1) the multi-byte encoding is determined by locale -- set externally
 *	   to the program.
 *
 *	2) many multi-byte encodings are limited to a small subset of the whole
 *	   range of Unicode values. So Unicode->MultiByte conversion may be lossy.
 *
 * Therefore, no internal code shall use *MultiByte* conversions.
 * If you do, everything may work perfectly within your environment,
 * but will fall apart with an unusual combination of locale and
 * international characters in a converted string.
 *
 * Only console I/O functions that must localized may use *MultiByte* conversions.
 *
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */
namespace MBase
{
	/**
	 * Converts a multi-byte string (assumed to be encoded accordingly to the current locale settings)
	 * to "wide characters" (wchar_t).
	 *
	 * @param ipMultiByte A multi-byte string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with '?'
	 *		otherwise throws an exception when an illegal character is found.
	 *
	 * @return A converted wide character string
	 * @throw StringConversionException when an invalid character found
	 *		and ibUseSubstitutionCharacter is false.
	 *
	 * @see StringConversionException
	 * @see WideCharToMultiByte( const wchar_t*, bool )
	 * @see ASCIIToWideChar( const char*, bool )
	 * @see UTF8ToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnWString MultiByteToWideChar(
		const char* ipMultiByte,
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a wide character string to a multi-byte string in a locale-dependent encoding.
	 *
	 * @param ipWide A wide character string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with '?'
	 *		otherwise throws an exception.
	 *
	 * @return A multi-byte string.
	 * @throw StringConversionException when an character cannot be converted is found
	 *		and ibUseSubstitutionCharacter is false.
	 *
	 * @see StringConversionException
	 * @see MultiByteToWideChar( const char*, bool )
	 * @see WideCharToASCII( const wchar_t*, bool )
	 * @see WideCharToUTF8( const wchar_t*, bool )
	 */
	DLL_BASE_EXIM ReturnString WideCharToMultiByte(
		const WCHAR* ipWide,
		bool ibUseSubstitutionCharacter = false );
}

#endif // MBase_MultiByteConverter_h
