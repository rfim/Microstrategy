//==============================================================================================
//	FILENAME	:	UTFConverter.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2001-11-18
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_UTFConverter_h
#define MBase_UTFConverter_h

#include "Base.h"
#include "ReturnString.h"
#include "PDCHeader/PDCwchar.h"
#include "Base/Defines/BasicTypes.h"

#include "Base/Interfaces/StringConversionException.h"
#include "Base/Interfaces/BaseMemoryContract.h"

namespace MBase
{
	/**
	 * Converts a Unicode string from UTF-8 to platform-specific "wide characters" (wchar_t),
	 * which carry UCS-2 Unicode values.
	 *
	 * @param ipUTF8 A Unicode string encoded using UTF-8.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception when an illegal character is found.
	 *
	 * @return A Unicode string of platform dependent wchar_t
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see WideCharToUTF8( const WCHAR*, bool )
	 */
	DLL_BASE_EXIM ReturnWString UTF8ToWideChar( 
		const char* ipUTF8, 
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Checks a single byte in a UTF-8 string and returns true if it is the leading byte (the first
	 * byte in a series of bytes that makes up one character)
	 *
	 * @param iUTF8 A single byte in a UTF-8 string 
	 *
	 * @return true when the input byte is a leading byte

	 * @see UTF8ToWideChar( const char*, bool ), UTF8TotalBytes
	 */
	DLL_BASE_EXIM bool UTF8IsLeadingByte(char iUTF8);

	/**
	 * Checks the lead byte in a UTF-8 string and returns the number of bytes in this one character
	 *
	 * @param iUTF8 A single byte in a UTF-8 string 
	 *
	 * @return number of bytes in this one character
	 * @throw StringConversionException when input character is not the leading byte or is
	 *		an invalid octet
	 * @see UTF8ToWideChar( const char*, bool ), UTF8IsLeadingByte
	 */
	DLL_BASE_EXIM unsigned Int32 UTF8TotalBytes(char iUTF8);

	/**
	 * Converts a Unicode string from platform-specific "wide characters" (wchar_t) to UTF-8.
	 *
	 * @param ipWide A wide character Unicode string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception
	 *
	 * @return A Unicode string as UTF-8.
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see UTF8ToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnString WideCharToUTF8( 
		const WCHAR* ipWide, 
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a Unicode string from platform-specific "wide characters" (wchar_t) to UTF-8. This differs
	 * from the above method in that the passed in string need not be null terminated. The passed out
	 * string will be null terminated
	 *
	 * @param ipWide A wide character Unicode string.
	 * @param iLength The number of characters to convert
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception
	 *
	 * @return A Unicode string as UTF-8.
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see UTF8ToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnString WideCharToUTF8UsingLength( 
		const WCHAR* ipWide,
		unsigned Int32 iLength,
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a Unicode string from platform-specific "wide characters" (wchar_t) to UTF-8.
	 *
	 * @param ipWide A wide character Unicode string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception
	 * @param irMemContract a memory contract used to govern the memory allocation
	 *
	 * @return A Unicode string as UTF-8.
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see UTF8ToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnString WideCharToUTF8Governed( 
		const WCHAR* ipWide, 
		bool ibUseSubstitutionCharacter,
		MBase::MemoryContractPtr& irMemContract);

	DLL_BASE_EXIM ReturnString WideCharToUTF8UsingLengthGoverned( 
		const WCHAR* ipWide,
		unsigned Int32 iLength,
		MBase::MemoryContractPtr& irMemContract,
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a Unicode string from UTF-16 to platform-specific "wide characters" (wchar_t),
	 * which carry UCS-2 Unicode values.
	 *
	 * @param ipUTF16 A Unicode string encoded using UTF-16.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception when an illegal character is found.
	 *
	 * @return A Unicode string of platform dependent wchar_t
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see WideCharToUTF16( const WCHAR*, bool )
	 */
	DLL_BASE_EXIM ReturnWString UTF16ToWideChar( 
		const unsigned short* ipUTF16, 
		bool ibUseSubstitutionCharacter = false );

	/**
	 * Converts a Unicode string from platform-specific "wide characters" (wchar_t) to UTF-16.
	 *
	 * @param ipWide A wide character Unicode string.
	 * @param ibUseSubstitutionCharacter when true, replaces invalid characters with U+FFFD
	 *		otherwise throws an exception
	 *
	 * @return A Unicode string as UTF-16.
	 * @throw StringConversionException when an invalid character found and ibUseSubstitutionCharacter == false
	 *
	 * @see StringConversionException
	 * @see UTF16ToWideChar( const char*, bool )
	 */
	DLL_BASE_EXIM ReturnUString WideCharToUTF16( 
		const WCHAR* ipWide, 
		bool ibUseSubstitutionCharacter = false );

	/**
	 * A Unicode BOM (Byte Order Mark)
	 *
	 * @see <URL:http://www.unicode.org/unicode/faq/utf_bom.html>
	 */
	DLL_BASE_EXIM extern const WCHAR UNICODE_BYTE_ORDER_MARK[];
}

#endif // MBase_UTFConverter_h
