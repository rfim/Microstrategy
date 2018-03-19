//==============================================================================================
//	FILENAME	:	PDCwtypes.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwtypes_h
#define PDCwtypes_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
	#ifndef STRICT
	#define STRICT // be type-safe
	#endif

//	#ifndef _WIN32_WINNT
//	#define _WIN32_WINNT 0x0500 // for Windows 2000+ APIs
//	#endif

	#include "ProtectedSource/DisableRareWin32API.h"
	#include <wtypes.h>
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	//==============================================================================================
	//	FILENAME	:	wtypes.h
	//	AUTHOR		:	Will Hurwood
	//	CREATION	:	7/10/2001
	//	Copyright (C) MicroStrategy Incorporated 2001
	//	All rights reserved
	//==============================================================================================
	// This header file stands in for the standard 'wtypes.h' header file used in VC++
	// We make minimal effort to declare symbols and functions in the same file as
	// used in Windows.

	// We use this header file to declare fundemental types used in COM interfaces.

	#ifndef __WTYPES_H__
	#define __WTYPES_H__

	#include "PDCwchar.h"
	#include "PDCrpc.h"
#ifdef WIN32
	#include "../../../../3rdParty/BaseTsd.h"
#endif
	////////////////////////////////////////////////////////////////////////////////////
	// Some hash defines often seen in Windows header files
	#ifndef NULL
	#define NULL 			0
	#endif

	#ifndef FALSE
	#define FALSE			0
	#endif

	#ifndef TRUE
	#define TRUE			1
	#endif

	// Including these macros makes it easier to copy over interface definitions
	#define EXTERN_C		extern "C"
	//#define interface		struct

	// These symbols appear to use compiler extensions
	// Easiest just to #define them away
	#ifndef FAR
	#define FAR
	#endif

	#define __RPC_FAR

	#ifdef WIN32
	#define STDMETHODCALLTYPE		__stdcall
	#else
	#define STDMETHODCALLTYPE		
	#endif

	#define STDAPICALLTYPE			
	#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
	#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method
	#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
	#define STDAPI_(type)           EXTERN_C type STDAPICALLTYPE
	#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
	#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE
	#define PURE                    = 0
	#define THIS_
	#define THIS                    void
	#define DECLARE_INTERFACE(iface)    interface iface
	#define DECLARE_INTERFACE_(iface, baseiface)    interface iface : public baseiface


	////////////////////////////////////////////////////////////////////////////////////
	// Standard datatypes

	typedef int INT;

	#ifndef _BOOL_
	#define _BOOL_
	#ifndef __LP64__
	typedef signed char BOOL;
	#else
	typedef bool BOOL; // yanzli: support arm64
	#endif
	#endif

	
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int UINT;

	#if defined(__sparcv9) || defined(__64BIT__)
	typedef int LONG;
    typedef long LONGLONG;
#ifndef DWORD
	typedef unsigned int DWORD;
#endif
	#else
	typedef long LONG;
    typedef long long LONGLONG;
	typedef unsigned long DWORD;
	#endif

#ifndef LPDWORD
	typedef DWORD __RPC_FAR *LPDWORD;
#endif
	typedef DWORD ULONG;
	typedef ULONG FAR *PULONG;

	#ifndef _HRESULT_
	#define _HRESULT_
	typedef LONG HRESULT;
	#endif
	typedef LONG *LPLONG;
	typedef LONG *PLONG;
	typedef void *LPVOID;
	typedef UINT   FAR *PUINT;
	#undef far
	#undef near
	#define far
	#define near
	typedef BYTE near           *PBYTE;
	typedef BYTE far            *LPBYTE;
#ifndef NEAR
	#define NEAR                near
#endif

	typedef short SHORT;  
	typedef void __RPC_FAR *PVOID;
	typedef float FLOAT;
	typedef double DOUBLE;
	typedef short VARIANT_BOOL;
	typedef LONG SCODE;
	typedef double DATE;
	typedef unsigned short USHORT;
	#ifndef VOID
		typedef void VOID;
	#endif
//01/17/06 nprabhudessai: added for MSICOM
#ifdef WIN32
	#if (!defined(_MAC)) && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
	#define pascal __stdcall
	#else
	#define pascal
	#endif
	#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
	#define PASCAL      __stdcall
	#else
	#define PASCAL      pascal
	#endif
	
	typedef BOOL near           *PBOOL;
	typedef BOOL far            *LPBOOL;
	
	typedef struct _MEMORYSTATUS {
		DWORD dwLength;
		DWORD dwMemoryLoad;
		SIZE_T dwTotalPhys;
		SIZE_T dwAvailPhys;
		SIZE_T dwTotalPageFile;
		SIZE_T dwAvailPageFile;
		SIZE_T dwTotalVirtual;
		SIZE_T dwAvailVirtual;
	} MEMORYSTATUS, *LPMEMORYSTATUS;
#endif	

	#ifdef WIN32
		typedef __int64 INT64;
	#else
		typedef long long INT64;
	#endif

	typedef INT64 hyper;
	typedef INT64 MIDL_uhyper;

	typedef hyper LONGLONG;
	typedef MIDL_uhyper ULONGLONG;
	typedef MIDL_uhyper DWORDLONG;

	typedef PVOID PSID;

	typedef union  _LARGE_INTEGER
	{
		struct {
	#ifdef WIN32
			DWORD LowPart;
			DWORD HighPart;
	#else
			DWORD HighPart;
			DWORD LowPart;
	#endif
		} l;
		struct {
	#ifdef WIN32
			DWORD LowPart;
			DWORD HighPart;
	#else
			DWORD HighPart;
			DWORD LowPart;
	#endif
		} u;
		LONGLONG QuadPart;
	}	LARGE_INTEGER;

	typedef union _ULARGE_INTEGER {
		struct {
	#ifdef WIN32
			DWORD LowPart;
			DWORD HighPart;
	#else
			DWORD HighPart;
			DWORD LowPart;
	#endif
		} l;
		struct {
	#ifdef WIN32
			DWORD LowPart;
			DWORD HighPart;
	#else
			DWORD HighPart;
			DWORD LowPart;
	#endif
		} u;
		ULONGLONG QuadPart;
	} ULARGE_INTEGER;

	#ifndef _FILETIME_
	#define _FILETIME_
	typedef struct  _FILETIME
	{
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	}	FILETIME, *PFILETIME, *LPFILETIME;;
	#endif //_FILETIME_

	#ifndef _tagCY_DEFINED
	#define _tagCY_DEFINED
	#define _CY_DEFINED

	/* real definition that makes the C++ compiler happy */
	typedef union tagCY {
		struct  {
			unsigned long Lo;
			long      Hi;
		} _Parts;
		LONGLONG int64;
	} CY;

	#endif /* _tagCY_DEFINED */
	typedef CY __RPC_FAR *LPCY;


	/* real definition that makes the C++ compiler happy */
	typedef struct tagDEC {
		USHORT wReserved;
		union tagParts1 {
			struct  {
				BYTE scale;
				BYTE sign;
			}_Parts1;
			USHORT signscale;
		};
		ULONG Hi32;
		union tagParts2 {
			struct  {
				ULONG Lo32;
				ULONG Mid32;
			}_Parts2;
			ULONGLONG Lo64;
		};
	} DECIMAL;

	#ifndef _LCID_DEFINED
	typedef DWORD LCID;
	#ifndef WIN32
	typedef DWORD LCTYPE;
	#endif
	# define _LCID_DEFINED
	#endif


	////////////////////////////////////////////////////////////////////////////////////
	// Windows handles

	// We do not attempt to be strict about kinds of handles
	typedef void *HANDLE;
	#define DECLARE_HANDLE(name) typedef HANDLE name
	typedef HANDLE *PHANDLE;

	DECLARE_HANDLE(HINSTANCE);
	typedef HINSTANCE HMODULE; // HMODULEs can be used in place of HINSTANCEs

	////////////////////////////////////////////////////////////////////////////////////
	// Global unique identifiers

	// In various places we use a different type with the same definition
	typedef GUID *LPGUID;
	#define REFGUID             const GUID &
	typedef GUID IID;
	typedef IID *LPIID;
	#define REFIID              const IID &
	#define IID_NULL            GUID_NULL
	#define IsEqualIID(riid1, riid2) InlineIsEqualGUID(riid1, riid2)
	typedef GUID CLSID;
	typedef CLSID *LPCLSID;
	#define CLSID_NULL          GUID_NULL
	#define IsEqualCLSID(rclsid1, rclsid2) InlineIsEqualGUID(rclsid1, rclsid2)
	#define REFCLSID            const CLSID &
	//#define IsEqualGUID(rguid1, rguid2) InlineIsEqualGUID(rguid1, rguid2)

	// Define symbols to stop MIDL generated files trying to redefine these types
	#define __IID_DEFINED__
	#define CLSID_DEFINED

	/////////////////////////////////////////////////////////////////
	// BSTR related datatypes
	/////////////////////////////////////////////////////////////////
	typedef char CHAR;

	#ifndef _WCHAR_DEFINED
	#define _WCHAR_DEFINED

	//#ifndef _BSTR_BINARY
#ifdef NO_WCHAR_SUPPORT
	typedef char WCHAR;
	#define _T(x) x
#else	
	typedef wchar_t WCHAR;
	#define _T(x) L##x
#endif
	//typedef unsigned short WCHAR;
	typedef WCHAR TCHAR;
	typedef WCHAR *LPWSTR, *PWSTR;

	//nprabhudessai 1/18/06 changes for MSICOM build
	typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
	/*
	#else
	typedef std::wstring BSTR;
	typedef WCHAR TCHAR;
	*/
	//#endif // _BSTR_BINARY

	#endif // !_WCHAR_DEFINED
	typedef /* [string] */ CHAR __RPC_FAR *LPSTR;

	typedef /* [string] */ const CHAR __RPC_FAR *LPCSTR;

	typedef /* [string] */ const WCHAR __RPC_FAR *LPCWSTR;

	typedef /* [string] */ TCHAR __RPC_FAR *LPTSTR;

	typedef /* [string] */ WCHAR __RPC_FAR *LPWSTR;

	typedef /* [string] */ const TCHAR __RPC_FAR *LPCTSTR;

	typedef CHAR *PSTR;
	typedef unsigned char *PUCHAR;

	/*
	#ifndef _WCHAR_T_DEFINED
	typedef unsigned short wchar_t;
	#define _WCHAR_T_DEFINED
	#endif
	*/




	typedef WCHAR OLECHAR;

	typedef /* [string] */ OLECHAR __RPC_FAR *LPOLESTR;

	typedef /* [string] */ const OLECHAR __RPC_FAR *LPCOLESTR;

	#define OLESTR(str) L##str




	typedef /* [wire_marshal] */ OLECHAR __RPC_FAR *BSTR;

	typedef /* [wire_marshal] */ void __RPC_FAR *HGLOBAL;

	#define VARIANT_TRUE ((VARIANT_BOOL)0xffff)
	#define VARIANT_FALSE ((VARIANT_BOOL)0)

	#define CONST const
	typedef CONST void     *LPCVOID;

	DECLARE_HANDLE            (HWND);


	#ifdef WIN32

	#define CALLBACK    __stdcall

	// take out of Unix build because it is already there.
	/*
	 * Structure used in select() call, taken from the BSD file sys/time.h.
	 */
	struct timeval 
	{
			long    tv_sec;         /* seconds */
			long    tv_usec;        /* and microseconds */
	};
	#else

	#define CALLBACK

	#endif
	
	typedef 
	enum tagMSHCTX
		{	MSHCTX_LOCAL	= 0,
		MSHCTX_NOSHAREDMEM	= 1,
		MSHCTX_DIFFERENTMACHINE	= 2,
		MSHCTX_INPROC	= 3
		}	MSHCTX;

	typedef 
	enum tagMSHLFLAGS
		{	MSHLFLAGS_NORMAL	= 0,
		MSHLFLAGS_TABLESTRONG	= 1,
		MSHLFLAGS_TABLEWEAK	= 2,
		MSHLFLAGS_NOPING	= 4
		}	MSHLFLAGS;
	typedef 

	enum tagSTATFLAG
		{	STATFLAG_DEFAULT	= 0,
		STATFLAG_NONAME	= 1,
		STATFLAG_NOOPEN	= 2
		}	STATFLAG;

	typedef void __RPC_FAR *HRGN;
	#define LF_FACESIZE         32

	typedef void __RPC_FAR *HDC;
	typedef void __RPC_FAR *HBITMAP;
	typedef /* [wire_marshal] */ void __RPC_FAR *HWND;

	typedef /* [wire_marshal] */ void __RPC_FAR *HMENU;

	typedef /* [wire_marshal] */ void __RPC_FAR *HACCEL;

	typedef /* [wire_marshal] */ void __RPC_FAR *HBRUSH;

	typedef /* [wire_marshal] */ void __RPC_FAR *HFONT;

	typedef /* [wire_marshal] */ void __RPC_FAR *HDC;

	typedef /* [wire_marshal] */ void __RPC_FAR *HICON;

	typedef bool BOOLEAN;

	#ifndef _COLORREF_DEFINED
	#define _COLORREF_DEFINED
	typedef DWORD COLORREF;

	#endif // !_COLORREF_DEFINED
	#ifndef _LPCOLORREF_DEFINED
	#define _LPCOLORREF_DEFINED
	typedef DWORD __RPC_FAR *LPCOLORREF;

	#endif // !_LPCOLORREF_DEFINED
	typedef HANDLE __RPC_FAR *LPHANDLE;

	#ifndef _SECURITY_ATTRIBUTES_
	#define _SECURITY_ATTRIBUTES_
	typedef struct  _SECURITY_ATTRIBUTES
		{
		DWORD nLength;
		/* [size_is] */ LPVOID lpSecurityDescriptor;
		BOOL bInheritHandle;
		}	SECURITY_ATTRIBUTES;

	typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *PSECURITY_ATTRIBUTES;

	#ifndef LPSECURITY_ATTRIBUTES
	typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *LPSECURITY_ATTRIBUTES;
	#endif // LPSECURITY_ATTRIBUTES

	#endif // !_SECURITY_ATTRIBUTES_


	/////////////////////////////////////////////////////////////////////////////////////
	// 10/07/2003 yuwen
	// Add TEXTMETRIC definition for MsiCOM and also UNIX
	#ifndef WIN32
	// for unix only, define a TEXTMETRIC class to replace the class with the same name on NT. 
	// with two member variables used in the pagination engine
	class TEXTMETRIC {
	public:
		long tmHeight; 
		long tmAscent; 
	};
	#else // WIN32 with MsiCOM
	typedef struct tagTEXTMETRICW
	{
		LONG tmHeight;
		LONG tmAscent;
		LONG tmDescent;
		LONG tmInternalLeading;
		LONG tmExternalLeading;
		LONG tmAveCharWidth;
		LONG tmMaxCharWidth;
		LONG tmWeight;
		LONG tmOverhang;
		LONG tmDigitizedAspectX;
		LONG tmDigitizedAspectY;
		WCHAR tmFirstChar;
		WCHAR tmLastChar;
		WCHAR tmDefaultChar;
		WCHAR tmBreakChar;
		BYTE tmItalic;
		BYTE tmUnderlined;
		BYTE tmStruckOut;
		BYTE tmPitchAndFamily;
		BYTE tmCharSet;
	}	TEXTMETRICW;
	typedef TEXTMETRICW TEXTMETRIC;
	#endif

	#ifdef WIN32
	#ifndef _SYSTEMTIME_
	#define _SYSTEMTIME_
	typedef struct  _SYSTEMTIME
		{
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
		}	SYSTEMTIME;

	typedef struct _SYSTEMTIME __RPC_FAR *PSYSTEMTIME;
	typedef struct _SYSTEMTIME __RPC_FAR *LPSYSTEMTIME;
	#endif // !_SYSTEMTIME


	//01/17/06 following definitions added : nprabhudessai, Required for MSICOM
	#define VER_PLATFORM_WIN32s             0
	#define VER_PLATFORM_WIN32_WINDOWS      1
	#define VER_PLATFORM_WIN32_NT           2
	
	//copied from windef.h
	typedef unsigned char UCHAR;
	typedef UCHAR *PUCHAR;

	
	typedef struct _OSVERSIONINFOA {
			DWORD dwOSVersionInfoSize;
			DWORD dwMajorVersion;
			DWORD dwMinorVersion;
			DWORD dwBuildNumber;
			DWORD dwPlatformId;
			CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
		} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

		typedef struct _OSVERSIONINFOW {
			DWORD dwOSVersionInfoSize;
			DWORD dwMajorVersion;
			DWORD dwMinorVersion;
			DWORD dwBuildNumber;
			DWORD dwPlatformId;
			WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
		} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
		#ifdef UNICODE
		typedef OSVERSIONINFOW OSVERSIONINFO;
		typedef POSVERSIONINFOW POSVERSIONINFO;
		typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
		#else
		typedef OSVERSIONINFOA OSVERSIONINFO;
		typedef POSVERSIONINFOA POSVERSIONINFO;
		typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
		#endif // UNICODE

		typedef struct _OSVERSIONINFOEXA {
			DWORD dwOSVersionInfoSize;
			DWORD dwMajorVersion;
			DWORD dwMinorVersion;
			DWORD dwBuildNumber;
			DWORD dwPlatformId;
			CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
			WORD   wServicePackMajor;
			WORD   wServicePackMinor;
			WORD   wSuiteMask;
			BYTE  wProductType;
			BYTE  wReserved;
		} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;
		typedef struct _OSVERSIONINFOEXW {
			DWORD dwOSVersionInfoSize;
			DWORD dwMajorVersion;
			DWORD dwMinorVersion;
			DWORD dwBuildNumber;
			DWORD dwPlatformId;
			WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
			WORD   wServicePackMajor;
			WORD   wServicePackMinor;
			WORD   wSuiteMask;
			BYTE  wProductType;
			BYTE  wReserved;
		} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW, RTL_OSVERSIONINFOEXW, *PRTL_OSVERSIONINFOEXW;
		#ifdef UNICODE
		typedef OSVERSIONINFOEXW OSVERSIONINFOEX;
		typedef POSVERSIONINFOEXW POSVERSIONINFOEX;
		typedef LPOSVERSIONINFOEXW LPOSVERSIONINFOEX;
		#else
		typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
		typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
		typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
		#endif // UNICODE

		//end of changes 01/17/06 nprabhudessai




	#endif // WIN32

	#endif /* __WTYPES_H__ */
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwtypes_h
