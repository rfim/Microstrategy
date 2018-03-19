//==============================================================================================
//	FILENAME	:	PDCwchar.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwchar_h
#define PDCwchar_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#include <wchar.h>

#ifdef NO_WCHAR_SUPPORT
	typedef char WCHAR; 
	#define _T(x) x
	#define _tcslen strlen
	#define _tcscpy strcpy
	#define _tcscmp strcmp
	#define _tcsicmp strcasecmp
#else	
	typedef wchar_t WCHAR;
	#define _T(x) L##x
	#define _tcslen wcslen
	#define _tcscpy wcscpy
	#define _tcscmp wcscmp
	#define _tcsicmp wcsicmp
#endif

// 2004-03-03 vovechkin
// define string operations equivalent on UNIX and Windows
#ifdef WIN32

	inline int wtoi(const wchar_t* ipStr)
	{
		return ::_wtoi(ipStr);
	}

	inline wchar_t* wcstok(wchar_t* ws1, const wchar_t* ws2, wchar_t** ptr)
	{
		// 2004-09-09 vovechkin
		// on Windows, the third parameter is not necessary, because
		// function ::wcstok stores that pointer internally.
		return (ptr != 0) ? ::wcstok_s(ws1, ws2, ptr) : 0;
	}

#elif defined(sun)

	#include <strings.h>
	#include <widec.h>

	inline int stricmp(const char* s1, const char* s2)
	{
		return ::strcasecmp(s1, s2);
	}

	inline int wcsicmp(const wchar_t* s1, const wchar_t* s2)
	{
		return ::wscasecmp(s1, s2);
	}

	inline int strnicmp(const char* s1, const char* s2, int n)
	{
		return ::strncasecmp(s1, s2, n);
	}

	inline int wcsnicmp(const wchar_t* s1, const wchar_t* s2, int n)
	{
		return ::wsncasecmp(s1, s2, n);
	}

	inline wchar_t* wcslwr(wchar_t* string)
	{
		for (wchar_t* lpStr = string; *lpStr != L'\0'; ++lpStr)
		{
			*lpStr = ::towlower(*lpStr);
		}

		return string;
	}

	inline int wtoi(const wchar_t* string)
	{
		return watoi(string);
	}
	
	
#elif defined(linux) 

	#include <cwchar>
	#include <string>
	#include <cwctype>

	inline int stricmp(const char* s1, const char* s2)
	{
		return ::strcasecmp(s1, s2);
	}
#ifndef NO_WCHAR_SUPPORT
	inline int wcsicmp(const wchar_t* s1, const wchar_t* s2)
	{
		return ::wcscasecmp(s1, s2);
	}
#endif
	inline int strnicmp(const char* s1, const char* s2, int n)
	{
		return ::strncasecmp(s1, s2, n);
	}
#ifndef NO_WCHAR_SUPPORT	
	inline int wcsnicmp(const wchar_t* s1, const wchar_t* s2, int n)
	{
		return ::wcsncasecmp(s1, s2, n);
	}
#endif
	inline wchar_t* wcslwr(wchar_t* string)
	{
		for (wchar_t* lpStr = string; *lpStr != L'\0'; ++lpStr)
		{
			*lpStr = ::towlower(*lpStr);
		}

		return string;
	}

	inline int wtoi(const wchar_t* string)
	{
		return wcstol(string, (wchar_t **)0, 10);
	}

#elif defined(_AIX)

// 2004-03-11 vovechkin
// these are defined in '/usr/include/wchar.h'
#ifdef wstrcmp
#undef wstrcmp
#endif // wstrcmp

	#include <strings.h>
	#include <wstring.h>

	inline int stricmp(const char* s1, const char* s2)
	{
		return ::strcasecmp(s1, s2);
	}

	inline int wcsicmp(const wchar_t* s1, const wchar_t* s2)
	{
		return ::wcscasecmp(s1, s2);
	}

	inline int strnicmp(const char* s1, const char* s2, int n)
	{
		return ::strncasecmp(s1, s2, n);
	}

	inline int wcsnicmp(const wchar_t* s1, const wchar_t* s2, int n)
	{
		return ::wcsncasecmp(s1, s2, n);
	}

	inline wchar_t* wcslwr(wchar_t* string)
	{
		for (wchar_t* lpStr = string; *lpStr != L'\0'; ++lpStr)
		{
			*lpStr = ::towlower(*lpStr);
		}

		return string;
	}

	inline int wtoi(const wchar_t* string)
	{
		return watoi(const_cast<wchar_t*>(string));
	}

#elif defined (__hpux)

	#include <cwchar>
	#include <string>
	#include <cwctype>
	#include <alloca.h>

	inline int stricmp(const char* s1, const char* s2)
	{
		return ::strcasecmp(s1, s2);
	}

	inline wchar_t* wcslwr(wchar_t* string)
	{
		for (wchar_t* lpStr = string; *lpStr != L'\0'; ++lpStr)
		{
			*lpStr = ::towlower(*lpStr);
		}

		return string;
	}

	inline int wcsicmp(const wchar_t* s1, const wchar_t* s2)
	{
		//TQMS 964671, xfan, 9/11/2014
		wchar_t* s1_1 = new wchar_t[wcslen(s1) + 1];
		wchar_t* s2_1 = new wchar_t[wcslen(s2) + 1];

		int lRet = -1;
		if ( s1_1 && s2_1 )
		{
			wcslcpy(s1_1, wcslen(s1) + 1, s1);
			wcslcpy(s2_1, wcslen(s2) + 1, s2);
			lRet = ::wcscmp(wcslwr(s1_1), wcslwr(s2_1) );
		}
	
		if(s1_1)
			delete[] s1_1;
		if(s2_1)
			delete[] s2_1;
		return lRet;
	}

	inline int strnicmp(const char* s1, const char* s2, int n)
	{
		return ::strncasecmp(s1, s2, n);
	}

	inline int wcsnicmp(const wchar_t* s1, const wchar_t* s2, int n)
	{
		wchar_t* s1_1 = new wchar_t[wcslen(s1) + 1];
		wchar_t* s2_1 = new wchar_t[wcslen(s2) + 1];

		int lRet = -1;
		if ( s1_1 && s2_1 )
		{
			wcslcpy(s1_1, wcslen(s1) + 1, s1);
			wcslcpy(s2_1, wcslen(s2) + 1, s2);
			lRet = ::wcsncmp(wcslwr(s1_1), wcslwr(s2_1), n);
		}
		
		if(s1_1)
			delete[] s1_1;
		if(s2_1)
			delete[] s2_1;
		return lRet;
	}

	inline int wtoi(const wchar_t* string)
	{
		return wcstol(string, (wchar_t **)0, 10);
	}
#elif defined(_MAC)
//added by ydong
//to support _MAC projects
#include <cwchar>
#include <string>
#include <cwctype>

inline int stricmp(const char* s1, const char* s2)
{
    return ::strcasecmp(s1, s2);
}

inline int wcsicmp(const wchar_t* s1, const wchar_t* s2)
{
    return ::wcscasecmp(s1, s2);
}

inline int strnicmp(const char* s1, const char* s2, int n)
{
    return ::strncasecmp(s1, s2, n);
}

inline int wcsnicmp(const wchar_t* s1, const wchar_t* s2, int n)
{
    return ::wcsncasecmp(s1, s2, n);
}

inline wchar_t* wcslwr(wchar_t* string)
{
    for (wchar_t* lpStr = string; *lpStr != L'\0'; ++lpStr)
    {
        *lpStr = ::towlower(*lpStr);
    }
    
    return string;
}

inline int wtoi(const wchar_t* string)
{
    return (int)wcstol(string, (wchar_t **)0, 10);
}
#else
//#error UNKNOWN PLATFORM
#endif

#if !defined(WIN32) && defined(WIN64)
/**
 * Security enhanced version of basic wide-char string-related functions for non-Windows platform.
 * @author Qiang Xue
 * @version $Revision: $  $Date: $
 */

/**
 * Security enhanced version of wcscat.
 * 
 * Appends strSource to strDestination and terminates the resulting string with a null
 * character. The initial character of strSource overwrites the terminating null character
 * of strDestination. The behavior of wcscat_s is undefined if the source and destination
 * strings overlap.
 *
 * Note that the second parameter is the total size of the buffer, not the remaining size.
 *
 * If strDestination is a null pointer, or is not null-terminated, or if strSource is 
 * a NULL pointer, or if the destination string is too small,  
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in words
 * @param const char* Null-terminated source string buffer
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t wcscat_s(wchar_t *strDestination, size_t sizeInWords, const wchar_t *strSource)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInWords>wcslen(strSource)+wcslen(strDestination),EINVAL);
	::wcscat(strDestination,strSource);
	return 0;
}

/**
 * Security enhanced version of wcscpy.
 * 
 * Copies the contents in the address of strSource, including the terminating null character,
 * to the location specified by strDestination. The destination string must be large enough to
 * hold the source string, including the terminating null character. The behavior of wcscpy_s
 * is undefined if the source and destination strings overlap.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in words
 * @param const char* Null-terminated source string buffer
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t wcscpy_s(wchar_t *strDestination, size_t sizeInWords, const wchar_t *strSource)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInWords>wcslen(strSource),EINVAL);
	::wcscpy(strDestination,strSource);
	return 0;
}

/**
 * Security enhanced version of wcsncat.
 * 
 * Copies up to count characters, including the terminating null character, from the string 
 * pointed to by strSource into the buffer pointed to by strDestination.
 * Once wcsncpy_s() has copied count characters to strDestination, it does not append a 
 * terminating null character. 
 *
 * Note that the second parameter is the total size of the destination buffer, not the remaining size.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string
 * @param size_t Size of the destination string buffer in words
 * @param const char* Null-terminated source string
 * @param size_t Number of characters to append
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t wcsncat_s(wchar_t *strDestination, size_t sizeInWords, const wchar_t *strSource, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(sizeInWords>wcslen(strSource)+(count>wcslen(strSource)?wcslen(strSource):count),EINVAL);
	::wcsncat(strDestination,strSource,count);
	return 0;
}

/**
 * Security enhanced version of wcsncpy.
 * 
 * Copies the initial count characters of strSource to strDestination and returns strDestination.
 * NOTE, Unlike wcsncpy, if count is less than or equal to the length of strSource, a null character 
 * is appended automatically to the copied string, and if count is greater than the length of strSource,
 * the destination string is NOT padded with null characters up to length count. 
 * The behavior of wcsncpy_s is undefined if the source and destination strings overlap.
 *
 * If strDestination or strSource is a null pointer, or if the destination string is too small,
 * it raises assertion error if _DEBUG is defined.
 * If execution is allowed to continue, it returns EINVAL and sets errno to EINVAL.
 *
 * @param char* Null-terminated destination string buffer
 * @param size_t Size of the destination string buffer in bytes
 * @param const char* Null-terminated source string buffer
 * @param size_t Number of characters to copy
 * @return errno_t EINVAL if parameter validation fails, 0 otherwise.
 */
inline errno_t wcsncpy_s(wchar_t *strDestination, size_t sizeInWords, const wchar_t *strSource, size_t count)
{
	_VALIDATE_RETURN_ERRCODE(strDestination!=NULL,EINVAL);
	_VALIDATE_RETURN_ERRCODE(strSource!=NULL,EINVAL);
	size_t len=(count>wcslen(strSource)?wcslen(strSource):count);
	_VALIDATE_RETURN_ERRCODE(sizeInWords>len,EINVAL);
	::wcsncpy(strDestination,strSource,len);
	strDestination[len]=(L'\0');
	return 0;
}

#endif


// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwchar_h
