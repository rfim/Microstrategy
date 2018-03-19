//==============================================================================================
//	FILENAME	:	StringConverter.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StringConverter_h
#define MBase_StringConverter_h

#include "../Defines/BasicTypes.h"
#include "Language.h"
#include "ReturnString.h"

namespace MBase
{
	/**
	 * Converts a wide character into its zero padded hexadecimal string form.
	 *
	 * Format example:
	 * L'A' will be converted to L"0041"
	 */
	ReturnWString DLL_BASE_EXIM WideChar2String(WCHAR iWideChar);

	/**
	 * Converts a binary sequence into its zero padded hexadecimal string form.
	 * @param: ipBin A pointer pointing to the head of the binary sequence.
	 * @param: iSize The length of the binary sequence(in bytes).
	 *
	 * For example:
	 * 0010 0001 0000 0011 1111 1110 (iSize = 3) will be converted to L"2103FE"
	 */
	DLL_BASE_EXIM ReturnWString Binary2String(void* ipBin, Int32 iSize);

	/**
	 * Converts a binary sequence into its zero padded octaldecimal string form.
	 * @param: ipBin A pointer pointing to the head of the binary sequence.
	 * @param: iSize The length of the binary sequence(in bytes).
	 *
	 * For example:
	 * 0010 0001 0000 0011 1111 1110 (iSize = 3) will be converted to L"\\041\\003\\376"
	 */
	DLL_BASE_EXIM ReturnWString Binary2OctString(void* ipBin, Int32 iSize);

	/**
	 * Converts an int into a decimal string
	 */
	DLL_BASE_EXIM ReturnWString ToDecimalWString(int iValue);

	/**
	 * Converts an int into a decimal string
	 */
	DLL_BASE_EXIM ReturnString ToDecimalString(int iValue);

	/**
	 * Converts an int into an upper-case hex string
	 */
	DLL_BASE_EXIM ReturnWString ToHexUpperWString(int iValue);

	/**
	 * Converts an int into an upper-case hex string
	 */
	DLL_BASE_EXIM ReturnString ToHexUpperString(int iValue);

	/**
	 * Converts an int into a lower-case hex string
	 */
	DLL_BASE_EXIM ReturnWString ToHexLowerWString(int iValue);

	/**
	 * Converts an int into a lower-case hex string
	 */
	DLL_BASE_EXIM ReturnString ToHexLowerString(int iValue);

#ifndef NO_WCHAR_SUPPORT
	/**
	 * Converts a decimal number into a 32-bit integer
	 */
	DLL_BASE_EXIM int DecimalToInt(const WCHAR* ipStr);
#endif
	/**
	 * Converts a decimal number into a 32-bit integer
	 */
	DLL_BASE_EXIM int DecimalToInt(const char* ipStr);

	/**
	 * Converts a string in the following format to double
	 * 	123.456, +1234.456, -1234.456, -.456, or -456.
	 *  123.456, +1234.456, -1.2344e56, -.45E-6
	 */

	DLL_BASE_EXIM double LocaleNeutralStringToDouble(const WCHAR* ipValueString);

	/**
	 * Convert a string into a 32-bit unsigned integer.
	 *
	 * @param ipStr a hex number in the range [0 .. 0xFFFFFFFE] without any prefix.
	 * @param iIgnoreTrailingCharacters whether to ignore the unparsed tail of the input string.
	 *
	 * @throw MBase::StringConversionException in case of an error.
	 */
#ifndef NO_WCHAR_SUPPORT
	DLL_BASE_EXIM unsigned int HexToUnsignedInt(const WCHAR* ipStr, bool iIgnoreTrailingCharacters = false);
#endif
	DLL_BASE_EXIM unsigned int HexToUnsignedInt(const char* ipStr, bool iIgnoreTrailingCharacters = false);

	/**
	 * Return a string describing a Standard C Library error, as returned by errno
	 */
	DLL_BASE_EXIM ReturnWString GetStandardCLibraryErrorDescriptionOrNULL(int iErrorCode) throw();

	/**
	 * Return a a Standard C Library error symbol (such as EPERM) or NULL.
	 */
	DLL_BASE_EXIM ReturnString GetStandardCLibraryErrorSymbolOrNULL(int iErrorCode) throw();

	/**
	 * Return a a Standard C Library error symbol (such as EPERM)
	 * or a string formatted as decimal integer.
	 */
	DLL_BASE_EXIM ReturnString GetStandardCLibraryErrorSymbolOrDecimalString(int iErrorCode) throw();

#ifdef WIN32
	/**
	 * Return a string describing a Win32 API error, as returned by ::GetLastError()
	 */
	DLL_BASE_EXIM ReturnWString GetWin32APIErrorDescriptionOrNULL(unsigned Int32 iErrorCode, LanguageID iLanguageID) throw();

	/**
	 * Return a symbol of a Win32 standard exception, i.e. "EXCEPTION_ACCESS_VIOLATION".
	 */
	DLL_BASE_EXIM ReturnString GetWin32ExceptionSymbolOrNULL(unsigned int iExceptionCode) throw();
	DLL_BASE_EXIM ReturnString GetWin32ExceptionSymbolOrHexUpperString(unsigned int iExceptionCode) throw();
#else
	/**
	 * Return a string describing a signal in the current locale, or NULL if the description is not available.
	 */
	DLL_BASE_EXIM ReturnWString GetSignalDescriptionOrNULL(int iSignalNumber) throw();

	/**
	 * Return a string with the signal name, such as HUP, INT, ILL, KILL, SEGV, and so on
	 * or NULL of the signal number is invalid.
	 */
	DLL_BASE_EXIM ReturnString GetSignalNameOrNULL(int iSignalNumber) throw();

	/**
	 * Return a string with the signal name, such as HUP, INT, ILL, KILL, SEGV, and so on
	 * or a string formatted as decimal integer.
	 */
	DLL_BASE_EXIM ReturnString GetSignalNameOrDecimalString(int iSignalNumber) throw();
	/**
	 * Converts a decimal number into a 64-bit integer
	 */
	DLL_BASE_EXIM Int64 DecimalToInt64(const wchar_t* ipStr);

	/**
	 * Converts a decimal number into a 64-bit integer
	 */
	DLL_BASE_EXIM Int64 DecimalToInt64(const char* ipStr);

#endif
}

#endif // MBase_StringConverter_h
