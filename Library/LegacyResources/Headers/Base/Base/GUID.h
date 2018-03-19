//==============================================================================================
//	FILENAME	:	GUID.h
//	AUTHOR		:	Will Hurwood
//	CREATION	:	7/10/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_GUID_h
#define MBase_GUID_h

#include "Base.h"
#ifdef _MAC
#include "ReturnString.h"
#endif
#if !defined(_NO_EXCEPTION)
#include "../Defines/ExceptionImpl.h"
#else
#include <stdio.h>
#endif

/**
 * GUID is the global unique identifier. It is used to uniquely identify any object
 * throughout time and across machines. This file contains only the definition of the
 * GUID data structure, comparison functions and the declaration of GUID_NULL.
 */

#ifndef GUID_DEFINED

#define GUID_DEFINED
#include <string.h>
#include <stdlib.h>
	typedef struct _GUID
	{
		unsigned int Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[ 8 ];
	} 
GUID;

#endif // GUID_DEFINED

// Define the famous GUID_NULL
// {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
extern "C" const ::GUID GUID_NULL;

namespace MBase
{
	/** 
	 * @return true if the GUID is equal to GUID_NULL
	 */
	inline bool IsGUIDNULL(const ::GUID& irGUID)
	{
		return (
			(reinterpret_cast<const int*>(&irGUID))[0] == 0 &&
			(reinterpret_cast<const int*>(&irGUID))[1] == 0 &&
			(reinterpret_cast<const int*>(&irGUID))[2] == 0 &&
			(reinterpret_cast<const int*>(&irGUID))[3] == 0);
	}

	/** 
	 * @return true if both GUIDs are the same
	 */
	inline bool IsEqualGUID(const ::GUID& irGUID1, const ::GUID& irGUID2)
	{
		return (
			(reinterpret_cast<const int*>(&irGUID1))[0] == (reinterpret_cast<const int*>(&irGUID2))[0] &&
			(reinterpret_cast<const int*>(&irGUID1))[1] == (reinterpret_cast<const int*>(&irGUID2))[1] &&
			(reinterpret_cast<const int*>(&irGUID1))[2] == (reinterpret_cast<const int*>(&irGUID2))[2] &&
			(reinterpret_cast<const int*>(&irGUID1))[3] == (reinterpret_cast<const int*>(&irGUID2))[3]);
	}


	/**
	 * CompareGUID returns:
	 *  <0 when irGUID1 < irGUID2
	 *  0 when irGUID1 == irGUID2
	 *  >0 when irGUID1 > irGUID2
	 */
	inline int CompareGUID(const ::GUID &irGUID1, const ::GUID &irGUID2)
	{
		// compare "time_low"
		if( irGUID1.Data1 < irGUID2.Data1 )
		{
			return -1;
		}
		else if( irGUID1.Data1 > irGUID2.Data1 )
		{
			return 1;
		}
		
		// compare "time_mid"
		if( irGUID1.Data2 < irGUID2.Data2 )
		{
			return -1;
		}
		else if( irGUID1.Data2 > irGUID2.Data2 )
		{
			return 1;
		}
		
		// compare "time_hi_and_version"
		if( irGUID1.Data3 < irGUID2.Data3 )
		{
			return -1;
		}
		else if( irGUID1.Data3 > irGUID2.Data3 )
		{
			return 1;
		}
		
		// compare "clock_seq_hi_and_reserved", "clock_seq_low", and "node"
		return ::memcmp( &irGUID1.Data4[0], &irGUID2.Data4[0], sizeof( irGUID1.Data4 ) );
	}

	inline bool operator==(const ::GUID& irGUID1, const ::GUID& irGUID2)
	{
		return MBase::IsEqualGUID(irGUID1, irGUID2);
	}

	inline bool operator!=(const ::GUID& irGUID1, const ::GUID& irGUID2)
	{
		return !MBase::IsEqualGUID(irGUID1, irGUID2);
	}

	/**
	 * Generates a new globally unique value.
	 *
	 * @throw MBase::BaseException in case of an error.
	 */
#ifdef __ANDROID__ // for Android, we use the implementation in GUID.cpp
	DLL_BASE_EXIM void CreateGUID(::GUID& orGUID);
#else
	inline void CreateGUID(::GUID& orGUID)
	{
		unsigned int fTimeLow = 0;
		memmove(reinterpret_cast<unsigned char*> (&orGUID), &fTimeLow, 16);
	}
#endif
	/**
	 * String2Guid converts from the string version to the guid structure version of a GUID.
	 * The string format is proprietary to MicroStrategy.
	 * @return false if the string is not a guid
	 *
	 * Format example:
	 * string "01234567CDEF89ABCE8A4602DF9B5713"
	 * will be converted to
	 * guid { 0x01234567, 0x89ab, 0xcdef, { 0x02, 0x46, 0x8a, 0xce, 0x13, 0x57, 0x9b, 0xdf } }
	 */
	//DLL_BASE_EXIM bool String2Guid(::GUID& orGUID, const WCHAR* ipString);



	////////Added by AE/////////
	inline bool operator<(const ::GUID& irGUID1, const ::GUID& irGUID2)
	{
		return MBase::CompareGUID(irGUID1, irGUID2) < 0;
	}

	inline unsigned char WideChar2Digit(WCHAR iChar)
	{
		switch(iChar)
		{
			case L'0': return 0;
			case L'1': return 1;
			case L'2': return 2;
			case L'3': return 3;
			case L'4': return 4;
			case L'5': return 5;
			case L'6': return 6;
			case L'7': return 7;
			case L'8': return 8;
			case L'9': return 9;
			case L'A': return 10;
			case L'B': return 11;
			case L'C': return 12;
			case L'D': return 13;
			case L'E': return 14;
			case L'F': return 15;
			case L'a': return 10;
			case L'b': return 11;
			case L'c': return 12;
			case L'd': return 13;
			case L'e': return 14;
			case L'f': return 15;
			default:
				return 0xFF;
		}
	}
	
	// StringToT converts a wide character hexadecimal string
	// to an <T>, where <T> is an unsigned integer type (unsigned long or
	// unsigned short).
	// As it is only used internally it doesn't do
	// any sanity checks.
	template<class T>
	inline bool StringToT(T& orT,const WCHAR* i_str)
	{
		T lResult = 0x0;
		orT = 0x0;
		
		for(int i = 0; i < (sizeof(T)*2); ++i)
		{
			unsigned char lDigit = WideChar2Digit( *(i_str + i) );
			
			if(lDigit == 0xFF)
			{
				return false;
			}
			
			lResult = (lResult << 4) + lDigit;
		}
		orT = lResult;
		return true;
	}

	//////Replaced By AE/////////
		/**
	 * String2Guid converts from the string version to the guid structure version of a GUID.
	 * The string format is proprietary to MicroStrategy.
	 * @return false if the string is not a guid
	 *
	 * Format example:
	 * string "01234567CDEF89ABCE8A4602DF9B5713"
	 * will be converted to
	 * guid { 0x01234567, 0x89ab, 0xcdef, { 0x02, 0x46, 0x8a, 0xce, 0x13, 0x57, 0x9b, 0xdf } }
	 */
	inline bool String2Guid(::GUID& orGUID, const WCHAR* ipcString)
	{
		orGUID = GUID_NULL;
		::GUID lTempGuid = GUID_NULL;
		
		if(!ipcString)
		{
			return false;
		}
		
//		if(wcslen(ipcString)!=32)
//		{
//			return false;
//		}
		
#ifdef WIN32
		// This is the original "DSS" format.
		// Keep the original conversion implementation.
		if(!StringToT<unsigned int>(*(reinterpret_cast<unsigned int*>(&lTempGuid)),ipcString))
		{
			return false;
		}
		if(!StringToT<unsigned int>(*(reinterpret_cast<unsigned int*>(&lTempGuid) + 1),ipcString + 8))
		{
			return false;
		}
		if(!StringToT<unsigned int>(*(reinterpret_cast<unsigned int*>(&lTempGuid) + 2),ipcString + 16))
		{
			return false;
		}
		if(!StringToT<unsigned int>(*(reinterpret_cast<unsigned int*>(&lTempGuid) + 3),ipcString + 24))
		{
			return false;
		}
#else
		// For unix, the original conversion implementation doesn't
		// work due to different "endianness".
		// Swap bits around...
		
		// first long is in correct order
		if(!StringToT<unsigned int>(lTempGuid.Data1, ipcString))
		{
			return false;
		}
		
		// second long has the shorts swapped
		{
			if(!StringToT<unsigned short>(lTempGuid.Data3, ipcString + 8))
			{
				return false;
			}
			
			if(!StringToT<unsigned short>(lTempGuid.Data2, ipcString + 12))
			{
				return false;
			}
		}
		
		// third long has each char backward
		{
			for(unsigned int i = 0; i < 4; ++i)
			{
				if(!StringToT<unsigned char>(lTempGuid.Data4[i], ipcString + 22 - i*2))
				{
					return false;
				}
			}
		}
		
		// fourth long has each char backward
		{
			for(unsigned int i = 0; i < 4; ++i)
			{
				if(!StringToT<unsigned char>(lTempGuid.Data4[i + 4], ipcString + 30 - i*2))
				{
					return false;
				}
			}
		}
		
#endif // WIN32
		
		orGUID = lTempGuid;
		return true;
	}

#ifndef NO_WCHAR_SUPPORT
	// If wide char is supported, we should also provide the same functionality
	// for the "char" version
	inline bool String2Guid(::GUID& orGUID, const char* ipString)
	{
		int lMblen = (int)mbstowcs(NULL, ipString, 0);
		wchar_t lWideStr[lMblen + 1];
		mbstowcs(lWideStr, ipString, lMblen + 1);
		// Delegate to the wide-char version
		return String2Guid(orGUID, lWideStr);	
	}
#endif

	/**
	 * Guid2String converts a guid structure into its string form.
	 * The string format is proprietary to MicroStrategy.
	 *
	 * Format example:
	 * guid { 0x01234567, 0x89ab, 0xcdef, { 0x02, 0x46, 0x8a, 0xce, 0x13, 0x57, 0x9b, 0xdf } }
	 * will be converted to
	 * string "01234567CDEF89ABCE8A4602DF9B5713"
	 */
	DLL_BASE_EXIM ReturnWString Guid2String(const ::GUID& irGUID);

	/**
	 * MSString2Guid converts from Microsoft's string version to the guid structure version of a GUID.
	 * The string format is identical to the one used in Microsoft's registry.
	 * @return false if the string is not a guid
	 *
	 * Format example:
	 * string "{01234567-89AB-CDEF-0246-8ACE13579BDF}"
	 * will be converted to
	 * guid { 0x01234567, 0x89ab, 0xcdef, { 0x02, 0x46, 0x8a, 0xce, 0x13, 0x57, 0x9b, 0xdf } }
	 */
	DLL_BASE_EXIM bool MSString2Guid(::GUID& orGuid,const WCHAR* ipString);

	/**
	 * Guid2MSString converts a guid structure into its Microsoft's string form.
	 * The string format is identical to the one used in Microsoft's registry.
	 *
	 * Format example:
	 * guid { 0x01234567, 0x89ab, 0xcdef, { 0x02, 0x46, 0x8a, 0xce, 0x13, 0x57, 0x9b, 0xdf } }
	 * will be converted to
	 * string "{01234567-89AB-CDEF-0246-8ACE13579BDF}"
	 */
	DLL_BASE_EXIM ReturnWString Guid2MSString(const ::GUID& irGuid);

	/**
	 * A user-friendly class that encapsulates GUID functionality.
	 */
	class DLL_BASE_EXIM GUID:
		public ::GUID
	{
	public:

		/// initialize with a new GUID value
		inline GUID()
		{
			CreateGUID(*this);
		}

		/// initialize from a GUID struct
		inline GUID(const ::GUID& irOtherGUID)
		{
			Data1 = irOtherGUID.Data1 ;
			Data2 = irOtherGUID.Data2;
			Data3 = irOtherGUID.Data3;
			memmove(Data4, irOtherGUID.Data4, sizeof(Data4));
		}

		/// initialize from a GUID string
		inline GUID(const WCHAR* ipString)
		{
			bool lResult = (*ipString == L'{') ? MSString2Guid(*this, ipString) : String2Guid(*this, ipString);
			
			if (lResult == false)
			{
#if !defined(_NO_EXCEPTION)
				throw MBase::ExceptionImpl<>("MBase::GUID: cannot parse string.");
#else
			printf("%s\n","MBase::GUID: cannot parse string.");
#endif
			}
		}

		inline ReturnWString ToWString() const
		{
			return Guid2String(*this);
		}

		inline ReturnWString ToMicrosoftWString() const
		{
			return Guid2MSString(*this);
		}

		inline bool operator==(const ::GUID& irOtherGUID) const
		{
			return MBase::IsEqualGUID(*this, irOtherGUID);
		}

		inline bool operator!=(const ::GUID& irOtherGUID) const
		{
			return !MBase::IsEqualGUID(*this, irOtherGUID);
		}

		inline bool operator<(const ::GUID& irOtherGUID) const
		{
			return MBase::CompareGUID(*this, irOtherGUID) < 0;
		}
		
		//////Member Method Added By AE/////////
		inline void Initialize(const WCHAR* ipString)
		{
			bool hr = String2Guid(*this, ipString);
			
			if (hr == false)
			{
#if !defined(_NO_EXCEPTION)
				throw ("MBase::GUID: cannot parse string.");
#else
				printf("%s\n","MBase::GUID: cannot parse string.");
				return;
#endif
			}
		}

		
		
	};




}

#endif // MBase_GUID_h
