//==============================================================================================
//	FILENAME	:	FragmentedString.h
//	AUTHOR		:	Xingyan Fan
//	CREATION	:	2008-12-29
//	Copyright (C) MicroStrategy, Inc. 2008
//  All rights reserved
//==============================================================================================

#ifndef MBase_FragmentedString_h
#define MBase_FragmentedString_h

#include "PDCHeader/PDCwchar.h"
//#include "Base/Base/Buffer.h"
#include "Base/Base/Allocator.h"
#include "Base/Defines/BasicTypes.h"
#include "Base.h"
#include "Base/Defines/StrongPtr.h"
//#include "PDCHeader/PDCwtypes.h"

typedef long HRESULT;

namespace MBase 
{
	template<class T>
	class DeleteFS
	{
	public:
		void operator()(T* ipData) throw()
		{
			if (ipData!=NULL)
			{
				ipData->Delete();
			}
		}
	};

	class FragmentedString
	{
	public:
		//Constructors
		
		// construct a fragmented string based on the input string ipStr
		// iSize decides the initial block size, it will copy the whole ipStr whatever the iSize is.
		// use MBase::Allocator<wchar_t> as default allocator
		FragmentedString(WCHAR* ipStr, size_t iSize, const MBase::Allocator<WCHAR> &iAlloc = MBase::Allocator<WCHAR>());

		// if there is no input paremeter iSize, it will allocate memory on page boundary and copy the whole ipStr
		FragmentedString(WCHAR* ipStr, const MBase::Allocator<WCHAR> &iAlloc = MBase::Allocator<WCHAR>());

		// destructor
		~FragmentedString();
		
	/**
		Append an array of characters to the fragmented string.
		The callers supplies a C-style array pointer and a length.
		If the length were 0 then nothing would be appended.
		Since the length of the array is passed explicitly, there is no need to end the array with '\0'.
		If the caller does find it convenient to end the string with L'\0'. then the caller should NOT
		include the end of string marker in the length count.
		@param ipChars A non-NULL pointer to an array of characters.
		@param iLength The length of the array, not including an optional L'\0'..
	*/
		// return S_OK if success, return S_FALSE if error, return DSSCOM_E_XML_BUFFER_EXCEPTION if allocate throw
		HRESULT AppendChars(WCHAR* ipChars, unsigned Int32 iLength);

	/**
		Append a single character to the end of the fragmented string.
		@param iChar The character to be appended.
	*/
		HRESULT AppendChar(WCHAR iChar);

	/**
		Append a wide-char string of characters to the end of the fragmented string.
		For this method the fragmented string class will identify the end of the string by looking
		for the final '\0' character.
		@param ipString A non-NULL pointer to an array of string characters ending with L'\0'.
	*/
		HRESULT AppendString(WCHAR* ipString);

	/**
		Return the length of the fragmented string in wide characters.
		The number returned does not include any end of string marker.
		@param opLength The current length of the string.
	*/
		HRESULT get_Length(unsigned Int32* opLength);

	/**
		Instruct the fragmented string to copy its internal data into the given string buffer.
		The caller should have called get_Length to determine the correct length.
		The characters are copied as a C-style array of wide characters.  No L'\0' character will
		be written.
		@param iLength The number of characters in the passed in buffer.
		@param opString A pointer to an array with room for the given number of characters.
	*/
		HRESULT CopyString(unsigned Int32 iLength, WCHAR* opString);

		// return total memory allocated size in bytes
		HRESULT get_TotalAllocatedSize(unsigned Int32* opSize);

		// return the total number of allocations to construct a whole string
		HRESULT get_TotalAllocNumer(unsigned Int32* opNum);

		// return the initial size passed-in to the constrcutor (in wide characters)
		// return 0 if there is no passed-in initial size
		HRESULT get_InitSize(unsigned Int32* opInit);

		void Delete();

	private:
		HRESULT hAppendChars(WCHAR* ipChars, unsigned Int32 iLength, bool ibThrow);

		// please check if HEADER_SIZE is multiple of sizeof(wchar_t) if you modify BlockHeader
		struct BlockHeader
		{
		unsigned Int32 mBlockSize; //block size, include the header
		void *mNext;  // to point to the next block
		};

		// HEADER_SIZE should be in multiple of sizeof(wchar_t) because the header is allocated with allocator<wchar_t>. 
		// This is not an issue right now. 
		// But if we make changes of BlockHeader, we need to check HEADER_SIZE.
		#define HEADER_SIZE sizeof(BlockHeader)

		void *mHeader;  //pointer of the fragmented string header
		MBase::Allocator<WCHAR> mAlloc;
		// size_t mPageSize;  // system page size
		size_t mAdjustSize;  // block size needs to be on boundary of mAdjustSize

		BlockHeader *mCurHdr;  //current block
		unsigned Int32 mSpaceUsed;  // Bytes used in the current block, include the header	
		unsigned Int32 mTotalLength; // total length of the string
		unsigned Int32 mTotalAllocatedSize; // total allocated size
		unsigned Int32 mTotalAllocNum;
		unsigned Int32 mInitSize;
		
		// helper functions
		WCHAR* GetCurrentPosition();
		size_t AdjustBoundary(size_t iSize);
	};

	// return NULL if exception happens. (for example, allocate throw)
	DLL_BASE_EXIM FragmentedString* CreateFragmentedString(WCHAR* ipStr, size_t iSize, const MBase::Allocator<WCHAR> &iAlloc = MBase::Allocator<WCHAR>());
	DLL_BASE_EXIM FragmentedString* CreateFragmentedString(WCHAR* ipStr, const MBase::Allocator<WCHAR> &iAlloc = MBase::Allocator<WCHAR>());

	typedef MBase::StrongPtr<FragmentedString, DeleteFS<FragmentedString> > FragmentedStringPtr;

}

#endif 