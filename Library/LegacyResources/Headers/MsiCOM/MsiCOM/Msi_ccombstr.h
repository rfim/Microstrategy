// File:	ccombstr.h
// Date:	8/20/2001
// Author:	Yuling Ma
//
// Copyright (C) 2001 MicroStrategy Incorporated
// All rights reserved

// This header file is for defining the CComBstr related structures, symbols and functions.
// On NT platform, the these structures, symbols and functions are defined in the following
// header files: atlbase.h
#ifndef _CCOMBSTR_H_
#define _CCOMBSTR_H_

#include "PDCHeader/PDCwtypes.h"
#include "PDCHeader/PDCwchar.h"
#include "PDCHeader/PDCmemory.h"
#include "PDCHeader/PDCmalloc.h"
#include "PDCHeader/PDCwctype.h"

#include "Msi_objbase.h"
#include "Msi_bstr.h"
#include "Msi_objidl.h"
#include "Msi_winuser.h"

/////////////////////////////
// define in ATLCONV.h
/////////////////////////////

// TODO: remove this symbol from source code
#define USES_CONVERSION

/////////////////////////////////////////////////////////////////////////////
// CComBSTR
class CComBSTR
{
public:
	BSTR m_str;
	CComBSTR()
	{
		m_str = NULL;
	}
	/*explicit*/ CComBSTR(int nSize)
	{
		m_str = ::SysAllocStringLen(NULL, nSize);
	}
	/*explicit*/ CComBSTR(int nSize, LPCOLESTR sz)
	{
		m_str = ::SysAllocStringLen(sz, nSize);
	}
	/*explicit*/ CComBSTR(LPCOLESTR pSrc)
	{
		m_str = ::SysAllocString(pSrc);
	}
	/*explicit*/ CComBSTR(const CComBSTR& src)
	{
		m_str = src.Copy();
	}
	/*explicit*/ CComBSTR(REFGUID src)
	{
		LPOLESTR szGuid;
		StringFromCLSID(src, &szGuid);
		m_str = ::SysAllocString(szGuid);
		CoTaskMemFree(szGuid);
		
	}
	CComBSTR& operator=(const CComBSTR& src)
	{
		if (m_str != src.m_str)
		{
			if (m_str)
				::SysFreeString(m_str);
			m_str = src.Copy();
		}
		return *this;
	}

	CComBSTR& operator=(LPCOLESTR pSrc)
	{
		::SysFreeString(m_str);
		m_str = ::SysAllocString(pSrc);
		return *this;
	}

	~CComBSTR()
	{
		::SysFreeString(m_str);
	}
	unsigned int Length() const
	{
		return (m_str == NULL)? 0 : SysStringLen(m_str);
	}
	operator BSTR() const
	{
		return m_str;
	}
	BSTR* operator&()
	{
		return &m_str;
	}
	BSTR Copy() const
	{
		return ::SysAllocStringLen(m_str, ::SysStringLen(m_str));
	}
	HRESULT CopyTo(BSTR* pbstr)
	{
		ATLASSERT(pbstr != NULL);
		if (pbstr == NULL)
			return E_POINTER;
		*pbstr = ::SysAllocStringLen(m_str, ::SysStringLen(m_str));
		if (*pbstr == NULL)
			return E_OUTOFMEMORY;
		return S_OK;
	}
	void Attach(BSTR src)
	{
		ATLASSERT(m_str == NULL);
		m_str = src;
	}
	BSTR Detach()
	{
		BSTR s = m_str;
		m_str = NULL;
		return s;
	}
	void Empty()
	{
		::SysFreeString(m_str);
		m_str = NULL;
	}
	bool operator!() const
	{
		return (m_str == NULL);
	}
	HRESULT Append(const CComBSTR& bstrSrc)
	{
		return Append(bstrSrc.m_str, SysStringLen(bstrSrc.m_str));
	}
	HRESULT Append(LPCOLESTR lpsz)
	{
#ifdef NO_WCHAR_SUPPORT
		return Append(lpsz, ::strlen(lpsz));
#else
		return Append(lpsz, ::wcslen(lpsz));
#endif
	}
	
	// a BSTR is just a LPCOLESTR so we need a special version to signify
	// that we are appending a BSTR
	HRESULT AppendBSTR(BSTR p)
	{
		return Append(p, SysStringLen(p));
	}
	HRESULT Append(LPCOLESTR lpsz, size_t nLen)
	{
		int n1 = Length();
		BSTR b;
		b = ::SysAllocStringLen(NULL, (UINT)(n1+nLen));
		if (b == NULL)
			return E_OUTOFMEMORY;
		memmove(b, m_str, n1*sizeof(OLECHAR));
		memmove(b+n1, lpsz, nLen*sizeof(OLECHAR));
		b[n1+nLen] = 0;
		SysFreeString(m_str);
		m_str = b;
		return S_OK;
	}
	HRESULT ToLower()
	{
		if (m_str != NULL)
		{
#ifdef NO_WCHAR_SUPPORT
			LPTSTR psz = CharLowerA(m_str);
#else
			LPTSTR psz = CharLowerW(m_str);
#endif
			if (psz == NULL)
				return E_OUTOFMEMORY;
			BSTR b = ::SysAllocString(psz);
			if (psz == NULL)
				return E_OUTOFMEMORY;
			SysFreeString(m_str);
			m_str = b;
		}
		return S_OK;
	}
	HRESULT ToUpper()
	{
		if (m_str != NULL)
		{
#ifdef NO_WCHAR_SUPPORT
			LPTSTR psz = CharLowerA(m_str);
#else
			LPTSTR psz = CharLowerW(m_str);
#endif
			if (psz == NULL)
				return E_OUTOFMEMORY;
			BSTR b = ::SysAllocString(psz);
			if (psz == NULL)
				return E_OUTOFMEMORY;
			SysFreeString(m_str);
			m_str = b;
		}
		return S_OK;
	}

	CComBSTR& operator+=(const CComBSTR& bstrSrc)
	{
		AppendBSTR(bstrSrc.m_str);
		return *this;
	}
	bool operator<(BSTR bstrSrc) const
	{
		if (bstrSrc == NULL && m_str == NULL)
			return false;
		if (bstrSrc != NULL && m_str != NULL)
#ifdef NO_WCHAR_SUPPORT
			return strcmp(m_str, bstrSrc) < 0;
#else
			return wcscmp(m_str, bstrSrc) < 0;
#endif
		return m_str == NULL;
	}
	bool operator==(BSTR bstrSrc) const
	{
		if (bstrSrc == NULL && m_str == NULL)
			return true;
		if (bstrSrc != NULL && m_str != NULL)
#ifdef NO_WCHAR_SUPPORT
			return strcmp(m_str, bstrSrc) == 0;
#else
			return wcscmp(m_str, bstrSrc) == 0;
#endif
		return false;
	}
	bool operator!=(BSTR bstrSrc) const
	{
		return !(*this == bstrSrc);
	}

	HRESULT WriteToStream(IStream* pStream)
	{
		ATLASSERT(pStream != NULL);
		ULONG cb;
		ULONG cbStrLen = m_str ? SysStringByteLen(m_str)+sizeof(OLECHAR) : 0;
		HRESULT hr = pStream->Write((void*) &cbStrLen, sizeof(cbStrLen), &cb);
		if (FAILED(hr))
			return hr;
		return cbStrLen ? pStream->Write((void*) m_str, cbStrLen, &cb) : S_OK;
	}
	HRESULT ReadFromStream(IStream* pStream)
	{
		ATLASSERT(pStream != NULL);
		ATLASSERT(m_str == NULL); // should be empty
		ULONG cbStrLen = 0;
		HRESULT hr = pStream->Read((void*) &cbStrLen, sizeof(cbStrLen), NULL);
		if ((hr == S_OK) && (cbStrLen != 0))
		{
			//subtract size for terminating NULL which we wrote out
			//since SysAllocStringByteLen overallocates for the NULL
            //msun, 4-23-2015, temporarily avoid the warnning message by explicitly converting ULONG to UINT. Need to refine in the future.
			m_str = SysAllocStringByteLen(NULL, (UINT)cbStrLen-sizeof(OLECHAR));
			if (m_str == NULL)
				hr = E_OUTOFMEMORY;
			else
				hr = pStream->Read((void*) m_str, cbStrLen, NULL);
		}
		if (hr == S_FALSE)
			hr = E_FAIL;
		return hr;
	}
};

#endif /* _CCOMBSTR_H_ */



