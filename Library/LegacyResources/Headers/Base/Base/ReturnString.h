//==============================================================================================
//	FILENAME	:	ReturnString.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_ReturnString_h
#define MBase_ReturnString_h

#ifdef NO_WCHAR_SUPPORT
	#include "PDCHeader/PDCwtypes.h"
#else
	#include "PDCHeader/PDCwchar.h"
#endif

#include <string>

#include "Base.h"
#include "Base/Defines/NoCopy.h"

namespace MBase
{
	/**
	 * Use ReturnString when you want to return a transient string.
	 * This means that there is no live string in the object
	 * (and therefore you may not return a const char*)
	 * We unfortunately have to leave the constructor
	 * public because we want this guy to be constructed anywhere.
	 * Its size, therefore, may never change.
	 * ReturnString may be used with a NULL.
	 * IMPORTANT: ReturnString constructor may throw!!! If the user function has
	 * altered the state, it will break the strong guarantee (but it will not break
	 * the basic guarantee).
	 */
	class DLL_BASE_EXIM ReturnString : public NoCopy
	{
	public:
		typedef char CharType;

		/// ReturnString will never throw if ipcString is NULL
		/// @param ipcString is a null-terminated string
		ReturnString(const char* ipcString);

		/**
		 * Preallocate a string of iLength characters (not including the terminating null-character)
		 * and initialize it with contents of ipString.
		 *
		 * @param ipcString is a string (not necessarily null-terminated), may be NULL
		 * @param iLength is the string length (in characters)
		 */
		ReturnString(size_t iLength, const char* ipString);

		/// ReturnString will never throw
#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined (__hpux) || defined (sun)
		// 2002-09-24 vovechkin
		// VisualAge compiler does not like non-const constructors
		// so, we have to cheat and do a const_cast inside
		ReturnString(const ReturnString& irReturnString) throw();
#else
		ReturnString(ReturnString& irReturnString) throw();
#endif // __IBMCPP__

		~ReturnString() throw();

		/**
		 * GetLength returns the length of the string (not including the terminating null-character).
		 * If the ReturnString is NULL, the returned value is 0.
		 */
		size_t GetLength() const;

		const char* c_str() const throw()
		{
			return mpString;
		}

	private:

		char* mpString;
	};

	/**
	 * ReturnWString is the wide-character version.
	 */
	class DLL_BASE_EXIM ReturnWString : public NoCopy
	{
	public:
		typedef WCHAR CharType;

		/// ReturnWString will never throw if ipcString is NULL
		/// @param ipcString is a null-terminated string
		ReturnWString(const WCHAR* ipcString);

		/**
		 * Preallocate a string of iLength characters (not including the terminating null-character)
		 * and initialize it with contents of ipString.
		 *
		 * @param ipcString is a string (not necessarily null-terminated), may be NULL
		 * @param iLength is the string length (in characters)
		 */
		ReturnWString(size_t iLength, const WCHAR* ipString);

		/// ReturnWString will never throw
#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined(__hpux) || defined (sun)
		// 2002-09-24 vovechkin
		// VisualAge compiler does not like non-const constructors
		// so, we have to cheat and do a const_cast inside
		ReturnWString(const ReturnWString& irReturnString) throw();
#else
		ReturnWString(ReturnWString& irReturnString) throw();
#endif // __IBMCPP__

		~ReturnWString() throw();

		/**
		 * GetLength returns the length of the string (not including the terminating null-character).
		 * If the ReturnWString is NULL, the returned value is 0.
		 */
		size_t GetLength() const;

		const WCHAR* c_str() const throw()
		{
			return mpString;
		}

	private:

		WCHAR* mpString;
	};

	/**
	 * ReturnUString is the unsigned short version of ReturnString,
	 * which may be used for UTF-16.
	 */
	class DLL_BASE_EXIM ReturnUString : public NoCopy
	{
	public:
		typedef unsigned short CharType;

		/// ReturnUString will never throw if ipcString is NULL
		/// @param ipcString is a null-terminated string
		ReturnUString(const unsigned short* ipcString);

		/**
		 * Preallocate a string of iLength characters (not including the terminating null-character)
		 * and initialize it with contents of ipString.
		 *
		 * @param ipcString is a string (not necessarily null-terminated), may be NULL
		 * @param iLength is the string length (in characters)
		 */
		ReturnUString(size_t iLength, const unsigned short* ipString);

		/// ReturnUString will never throw
#if defined(__IBMCPP__) || defined(WIN64) || defined(__GNUG__) || defined(__hpux) || defined (sun)
		// 2002-09-24 vovechkin
		// VisualAge compiler does not like non-const constructors
		// so, we have to cheat and do a const_cast inside
		ReturnUString(const ReturnUString& irReturnString) throw();
#else
		ReturnUString(ReturnUString& irReturnString) throw();
#endif // __IBMCPP__

		~ReturnUString() throw();

		/**
		 * GetLength returns the length of the string (not including the terminating null-character).
		 * If the ReturnUString is NULL, the returned value is 0.
		 */
		size_t GetLength() const;

		const unsigned short* c_str() const throw()
		{
			return mpString;
		}

	private:

		unsigned short* mpString;
	};
}

#endif // MBase_ReturnString_h
