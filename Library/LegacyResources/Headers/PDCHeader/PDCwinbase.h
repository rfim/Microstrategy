//==============================================================================================
//	FILENAME	:	PDCwinbase.h
//	AUTHOR		:	Perl Script
//	CREATION	:	6/13/02
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#ifndef PDCwinbase_h
#define PDCwinbase_h

// this must be the *first* file included
#include "ProtectedSource/Prolog.h"

#if defined(WIN32) && !defined(_USE_MSI_COM)
//	#ifndef _WIN32_WINNT
//	#define _WIN32_WINNT 0x0500 // for Windows 2000+ APIs
//	#endif

	#include <winbase.h>
	#include "ProtectedSource/UndefineWin32APIAliases.h"
#else
	/************************************************************************
	*                                                                       *
	*   winbase.h -- This module defines the 32-Bit Windows Base APIs       *
	*                                                                       *
	*   Copyright 1990 - 1998 Microsoft Corp. All rights reserved.       *
	*                                                                       *
	************************************************************************/
	#ifndef _WINBASE_
	#define _WINBASE_

#ifdef WIN32
	#include "../../../../3rdParty/BaseTsd.h"
#endif

	#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
	#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
	#define FORMAT_MESSAGE_FROM_STRING     0x00000400
	#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
	#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
	#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
	#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

	#define WAIT_TIMEOUT                        STATUS_TIMEOUT
	#define WAIT_IO_COMPLETION                  STATUS_USER_APC
	#define STILL_ACTIVE                        STATUS_PENDING
	#define EXCEPTION_ACCESS_VIOLATION          STATUS_ACCESS_VIOLATION
	#define EXCEPTION_DATATYPE_MISALIGNMENT     STATUS_DATATYPE_MISALIGNMENT
	#define EXCEPTION_BREAKPOINT                STATUS_BREAKPOINT
	#define EXCEPTION_SINGLE_STEP               STATUS_SINGLE_STEP
	#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     STATUS_ARRAY_BOUNDS_EXCEEDED
	#define EXCEPTION_FLT_DENORMAL_OPERAND      STATUS_FLOAT_DENORMAL_OPERAND
	#define EXCEPTION_FLT_DIVIDE_BY_ZERO        STATUS_FLOAT_DIVIDE_BY_ZERO
	#define EXCEPTION_FLT_INEXACT_RESULT        STATUS_FLOAT_INEXACT_RESULT
	#define EXCEPTION_FLT_INVALID_OPERATION     STATUS_FLOAT_INVALID_OPERATION
	#define EXCEPTION_FLT_OVERFLOW              STATUS_FLOAT_OVERFLOW
	#define EXCEPTION_FLT_STACK_CHECK           STATUS_FLOAT_STACK_CHECK
	#define EXCEPTION_FLT_UNDERFLOW             STATUS_FLOAT_UNDERFLOW
	#define EXCEPTION_INT_DIVIDE_BY_ZERO        STATUS_INTEGER_DIVIDE_BY_ZERO
	#define EXCEPTION_INT_OVERFLOW              STATUS_INTEGER_OVERFLOW
	#define EXCEPTION_PRIV_INSTRUCTION          STATUS_PRIVILEGED_INSTRUCTION
	#define EXCEPTION_IN_PAGE_ERROR             STATUS_IN_PAGE_ERROR
	#define EXCEPTION_ILLEGAL_INSTRUCTION       STATUS_ILLEGAL_INSTRUCTION
	#define EXCEPTION_NONCONTINUABLE_EXCEPTION  STATUS_NONCONTINUABLE_EXCEPTION
	#define EXCEPTION_STACK_OVERFLOW            STATUS_STACK_OVERFLOW
	#define EXCEPTION_INVALID_DISPOSITION       STATUS_INVALID_DISPOSITION
	#define EXCEPTION_GUARD_PAGE                STATUS_GUARD_PAGE_VIOLATION
	#define EXCEPTION_INVALID_HANDLE            STATUS_INVALID_HANDLE
	#define CONTROL_C_EXIT                      STATUS_CONTROL_C_EXIT

	#define FILE_MAP_COPY       SECTION_QUERY
	#define FILE_MAP_WRITE      SECTION_MAP_WRITE
	#define FILE_MAP_READ       SECTION_MAP_READ
	#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS
	#define FILE_MAP_EXECUTE    SECTION_MAP_EXECUTE_EXPLICIT    // not included in FILE_MAP_ALL_ACCESS

	#ifndef MAX_PATH
	#define MAX_PATH 260
	#endif

	#define IGNORE              0       // Ignore signal
	#define INFINITE            0xFFFFFFFF  // Infinite timeout


	#ifdef WIN32
	//kxiao add
	#define FILE_FLAG_WRITE_THROUGH         0x80000000
	#define FILE_FLAG_OVERLAPPED            0x40000000
	#define FILE_FLAG_NO_BUFFERING          0x20000000
	#define FILE_FLAG_RANDOM_ACCESS         0x10000000
	#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000
	#define FILE_FLAG_DELETE_ON_CLOSE       0x04000000
	#define FILE_FLAG_BACKUP_SEMANTICS      0x02000000
	#define FILE_FLAG_POSIX_SEMANTICS       0x01000000
	#define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
	#define FILE_FLAG_OPEN_NO_RECALL        0x00100000
	#define FILE_FLAG_FIRST_PIPE_INSTANCE   0x00080000

	//1/19/06 nprabhudessai
	#define CREATE_NEW          1
	#define CREATE_ALWAYS       2
	#define OPEN_EXISTING       3
	#define OPEN_ALWAYS         4
	#define TRUNCATE_EXISTING   5

	typedef struct _WIN32_FIND_DATAA {
		DWORD dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD nFileSizeHigh;
		DWORD nFileSizeLow;
		DWORD dwReserved0;
		DWORD dwReserved1;
		CHAR   cFileName[ MAX_PATH ];
		CHAR   cAlternateFileName[ 14 ];
	#ifdef _MAC
		DWORD dwFileType;
		DWORD dwCreatorType;
		WORD  wFinderFlags;
	#endif
	} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

	typedef struct _WIN32_FIND_DATAW {
		DWORD dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD nFileSizeHigh;
		DWORD nFileSizeLow;
		DWORD dwReserved0;
		DWORD dwReserved1;
		WCHAR  cFileName[ MAX_PATH ];
		WCHAR  cAlternateFileName[ 14 ];
	} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;

	typedef LPWIN32_FIND_DATAW LPWIN32_FIND_DATA;
	typedef WIN32_FIND_DATAW WIN32_FIND_DATA;

	typedef struct _MEMORYSTATUSEX {
		DWORD dwLength;
		DWORD dwMemoryLoad;
		DWORDLONG ullTotalPhys;
		DWORDLONG ullAvailPhys;
		DWORDLONG ullTotalPageFile;
		DWORDLONG ullAvailPageFile;
		DWORDLONG ullTotalVirtual;
		DWORDLONG ullAvailVirtual;
		DWORDLONG ullAvailExtendedVirtual;
	} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;
	#define STD_INPUT_HANDLE    (DWORD)-10
	#define STD_OUTPUT_HANDLE   (DWORD)-11
	#define STD_ERROR_HANDLE      (DWORD)-12

	#define OPEN_EXISTING       3

#ifdef WIN64
	#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#else
	#define INVALID_HANDLE_VALUE (HANDLE)-1
#endif

	#ifndef WINAPI
	#define WINAPI __stdcall
	#endif

	// declare prototypes of this kernel32.dll functions for Windows only
	extern "C" UINT WINAPI GetWindowsDirectoryW(
	  LPTSTR lpBuffer,  // buffer for Windows directory
	  UINT uSize        // size of directory buffer
	);

	// declare prototypes of this kernel32.dll functions for Windows only
	extern "C" UINT WINAPI GetSystemWindowsDirectoryW(
	  LPTSTR lpBuffer,  // buffer for Windows directory
	  UINT uSize        // size of directory buffer
	);

	// declare prototypes of this kernel32.dll functions for Windows only
	extern "C" DWORD WINAPI GetModuleFileNameW(
	  HMODULE hModule,
	  LPTSTR lpFilename,
   	  DWORD nSize
	);

	// declare prototypes of this kernel32.dll functions for Windows only
	extern "C" DWORD WINAPI GetLastError(void);

	extern "C" BOOL WINAPI IsBadWritePtr(
	  VOID* lp, // memory address
	  UINT ucb   // size of memory block
	);

	extern "C" BOOL WINAPI IsBadReadPtr(
	  CONST VOID* lp, // memory address
	  UINT ucb   // size of memory block
	);

	extern "C" HANDLE WINAPI RegisterEventSourceW(
	  LPCTSTR lpUNCServerName,  // server name
	  LPCTSTR lpSourceName      // source name
	);

	extern "C" BOOL WINAPI ReportEventW(
	  HANDLE hEventLog,    // handle to event log
	  WORD wType,          // event type
	  WORD wCategory,      // event category
	  DWORD dwEventID,     // event identifier
	  PSID lpUserSid,      // user security identifier
	  WORD wNumStrings,    // number of strings to merge
	  DWORD dwDataSize,    // size of binary data
	  LPCTSTR *lpStrings,  // array of strings to merge
	  LPVOID lpRawData     // binary data buffer
	);

	extern "C" BOOL WINAPI DeregisterEventSource(
	  HANDLE hEventLog   // handle to event log
	);


	#define InterlockedExchange _InterlockedExchange
	extern "C" LONG WINAPI _InterlockedExchange(
	  LONG volatile * Target, // value to exchange
	  LONG Value              // new value
	);

	extern "C" VOID WINAPI Sleep(
	  DWORD dwMilliseconds   // sleep time
	);

	extern "C" BOOL WINAPI SetStdHandle(
	  DWORD nStdHandle,  // input, output, or error device
	  HANDLE hHandle     // handle
	);

	extern "C" HANDLE WINAPI CreateFileW(
	  LPCTSTR lpFileName,                         // file name
	  DWORD dwDesiredAccess,                      // access mode
	  DWORD dwShareMode,                          // share mode
	  LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
	  DWORD dwCreationDisposition,                // how to create
	  DWORD dwFlagsAndAttributes,                 // file attributes
	  HANDLE hTemplateFile                        // handle to template file
	);

	//kxiao add for security subsytem  on MSICOM Windows
	extern "C"	HANDLE
	WINAPI
	GetCurrentProcess(
		VOID
		);

	extern "C"
	HANDLE
	WINAPI
	GetCurrentThread(
		VOID
	   );

	extern "C"
	BOOL
	WINAPI
	OpenProcessToken (
		HANDLE ProcessHandle,
		DWORD DesiredAccess,
		PHANDLE TokenHandle
		);

	extern "C"
	HANDLE
	WINAPI
	OpenProcess(
		DWORD dwDesiredAccess,
		BOOL bInheritHandle,
		DWORD dwProcessId
		);

	extern "C"
	DWORD
	WINAPI
	GetLengthSid (
		PSID pSid
		);

		extern "C"
	BOOL
	WINAPI
	OpenThreadToken (
		HANDLE ThreadHandle,
		DWORD DesiredAccess,
		BOOL OpenAsSelf,
		PHANDLE TokenHandle
		);


	extern "C"
	BOOL
	WINAPI
	GetTokenInformation (
		HANDLE TokenHandle,
		TOKEN_INFORMATION_CLASS TokenInformationClass,
		LPVOID TokenInformation,
		DWORD TokenInformationLength,
		PDWORD ReturnLength
		);

	extern "C"
	BOOL
	WINAPI
	GetProcessAffinityMask(
		__in  HANDLE hProcess,
		__out PDWORD_PTR lpProcessAffinityMask,
		__out PDWORD_PTR lpSystemAffinityMask
		);

	extern "C"
	BOOL
	WINAPI
	SetProcessAffinityMask(
		__in HANDLE hProcess,
		__in DWORD_PTR dwProcessAffinityMask
		);


	extern "C"
	BOOL
	WINAPI
	QueryPerformanceFrequency(
		__out LARGE_INTEGER *lpFrequency
		);

	extern "C"
	BOOL
	WINAPI
	QueryPerformanceCounter(
		__out LARGE_INTEGER *lpPerformanceCount
		);

	//01/17/06 Added following definitions for MSICOM build nprabhudessai
		extern "C"
		BOOL
		WINAPI
		GetVersionExA(
			__inout LPOSVERSIONINFOA lpVersionInformation
			);


		extern "C"
		BOOL
		WINAPI
		GetVersionExW(
			__inout LPOSVERSIONINFOW lpVersionInformation
			);
		#ifdef UNICODE
		#define GetVersionEx  GetVersionExW
		#else
		#define GetVersionEx  GetVersionExA
		#endif // !UNICODE

#ifndef FARPROC
typedef int (FAR WINAPI *FARPROC)();
#endif

		extern "C"
		FARPROC
		WINAPI
		GetProcAddress (
			__in HMODULE hModule,
			__in LPCSTR lpProcName
			);

		extern "C"
		VOID
		WINAPI
		GlobalMemoryStatus(
			__out LPMEMORYSTATUS lpBuffer
			);

		extern "C"
		__out
		HMODULE
		WINAPI
		GetModuleHandleW(
			__in_opt LPCWSTR lpModuleName
			);
		#ifdef UNICODE
		#define GetModuleHandle  GetModuleHandleW
		#else
		#define GetModuleHandle  GetModuleHandleA
		#endif // !UNICODE

		typedef struct _STARTUPINFOA {
			DWORD   cb;
			LPSTR   lpReserved;
			LPSTR   lpDesktop;
			LPSTR   lpTitle;
			DWORD   dwX;
			DWORD   dwY;
			DWORD   dwXSize;
			DWORD   dwYSize;
			DWORD   dwXCountChars;
			DWORD   dwYCountChars;
			DWORD   dwFillAttribute;
			DWORD   dwFlags;
			WORD    wShowWindow;
			WORD    cbReserved2;
			LPBYTE  lpReserved2;
			HANDLE  hStdInput;
			HANDLE  hStdOutput;
			HANDLE  hStdError;
		} STARTUPINFOA, *LPSTARTUPINFOA;
		typedef struct _STARTUPINFOW {
			DWORD   cb;
			LPWSTR  lpReserved;
			LPWSTR  lpDesktop;
			LPWSTR  lpTitle;
			DWORD   dwX;
			DWORD   dwY;
			DWORD   dwXSize;
			DWORD   dwYSize;
			DWORD   dwXCountChars;
			DWORD   dwYCountChars;
			DWORD   dwFillAttribute;
			DWORD   dwFlags;
			WORD    wShowWindow;
			WORD    cbReserved2;
			LPBYTE  lpReserved2;
			HANDLE  hStdInput;
			HANDLE  hStdOutput;
			HANDLE  hStdError;
		} STARTUPINFOW, *LPSTARTUPINFOW;
		#ifdef UNICODE
		typedef STARTUPINFOW STARTUPINFO;
		typedef LPSTARTUPINFOW LPSTARTUPINFO;
		#else
		typedef STARTUPINFOA STARTUPINFO;
		typedef LPSTARTUPINFOA LPSTARTUPINFO;
		#endif // UNICODE


		typedef struct _PROCESS_INFORMATION {
			HANDLE hProcess;
			HANDLE hThread;
			DWORD dwProcessId;
			DWORD dwThreadId;
		} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;


		extern "C"
		BOOL
		WINAPI
		CreateProcessA(
			__in_opt    LPCSTR lpApplicationName,
			__inout_opt LPSTR lpCommandLine,
			__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
			__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
			__in        BOOL bInheritHandles,
			__in        DWORD dwCreationFlags,
			__in_opt    LPVOID lpEnvironment,
			__in_opt    LPCSTR lpCurrentDirectory,
			__in        LPSTARTUPINFOA lpStartupInfo,
			__out       LPPROCESS_INFORMATION lpProcessInformation
			);
		extern "C"
		BOOL
		WINAPI
		CreateProcessW(
			__in_opt    LPCWSTR lpApplicationName,
			__inout_opt LPWSTR lpCommandLine,
			__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
			__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
			__in        BOOL bInheritHandles,
			__in        DWORD dwCreationFlags,
			__in_opt    LPVOID lpEnvironment,
			__in_opt    LPCWSTR lpCurrentDirectory,
			__in        LPSTARTUPINFOW lpStartupInfo,
			__out       LPPROCESS_INFORMATION lpProcessInformation
			);
		#ifdef UNICODE
		#define CreateProcess  CreateProcessW
		#else
		#define CreateProcess  CreateProcessA
		#endif // !UNICODE

/*
		extern "C"
		VOID
		WINAPI
		OutputDebugStringA(
			__in LPCSTR lpOutputString
			);
		extern "C"
		VOID
		WINAPI
		OutputDebugStringW(
			__in LPCWSTR lpOutputString
			);
		#ifdef UNICODE
		#define OutputDebugString  OutputDebugStringW
		#else
		#define OutputDebugString  OutputDebugStringA
		#endif // !UNICODE
*/
		//
		//  File structures
		//
		typedef struct _OVERLAPPED {
			ULONG_PTR Internal;
			ULONG_PTR InternalHigh;
			union {
				struct {
					DWORD Offset;
					DWORD OffsetHigh;
				};

				PVOID Pointer;
			};

			HANDLE  hEvent;
		} OVERLAPPED, *LPOVERLAPPED;

		//required for MSIXML project
		extern "C"
		VOID
		WINAPI
		DebugBreak(
			VOID
			);

		extern "C"
		BOOL
		WINAPI
		CloseHandle(
			__in HANDLE hObject
			);

		extern "C"
		BOOL
		WINAPI
		WriteFile(
			__in        HANDLE hFile,
			__in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
			__in        DWORD nNumberOfBytesToWrite,
			__out_opt   LPDWORD lpNumberOfBytesWritten,
			__inout_opt LPOVERLAPPED lpOverlapped
			);

		extern "C"
		BOOL
		WINAPI
		ReadFile(
			__in        HANDLE hFile,
			__out_bcount_part(nNumberOfBytesToRead, *lpNumberOfBytesRead) LPVOID lpBuffer,
			__in        DWORD nNumberOfBytesToRead,
			__out_opt   LPDWORD lpNumberOfBytesRead,
			__inout_opt LPOVERLAPPED lpOverlapped
			);

		extern "C"
		int
		WINAPI
		lstrlenA(
			__in LPCSTR lpString
			);

		extern "C"
		int
		WINAPI
		lstrlenW(
			__in LPCWSTR lpString
			);

		#ifdef UNICODE
		#define lstrlen  lstrlenW
		#else
		#define lstrlen  lstrlenA
		#endif // !UNICODE

		extern "C"
		__out
		LPSTR
		WINAPI
		lstrcpynA(
			__out_ecount(iMaxLength) LPSTR lpString1,
			__in LPCSTR lpString2,
			__in int iMaxLength
			);


		extern "C"
		__out
		LPWSTR
		WINAPI
		lstrcpynW(
			__out_ecount(iMaxLength) LPWSTR lpString1,
			__in LPCWSTR lpString2,
			__in int iMaxLength
			);
		#ifdef UNICODE
		#define lstrcpyn  lstrcpynW
		#else
		#define lstrcpyn  lstrcpynA
		#endif // !UNICODE



		extern "C"
		__out
		LPSTR
		WINAPI
		lstrcpyA(
			__out LPSTR lpString1,
			__in  LPCSTR lpString2
			);


		extern "C"
		__out
		LPWSTR
		WINAPI
		lstrcpyW(
			__out LPWSTR lpString1,
			__in  LPCWSTR lpString2
			);
		#ifdef UNICODE
		#define lstrcpy  lstrcpyW
		#else
		#define lstrcpy  lstrcpyA
		#endif // !UNICODE

		extern "C"
		int
		WINAPI
		lstrcmpiA(
			__in LPCSTR lpString1,
			__in LPCSTR lpString2
			);


		extern "C"
		int
		WINAPI
		lstrcmpiW(
			__in LPCWSTR lpString1,
			__in LPCWSTR lpString2
			);
		#ifdef UNICODE
		#define lstrcmpi  lstrcmpiW
		#else
		#define lstrcmpi  lstrcmpiA
		#endif // !UNICODE


		#if !defined(WINAPIV)
		#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
		#define WINAPIV     __cdecl
		#else
		#define WINAPIV
		#endif
		#endif


		extern "C"
		int
		WINAPIV
		wsprintfA(
			__out LPSTR,
			__in __format_string LPCSTR,
			...);


		extern "C"
		int
		WINAPIV
		wsprintfW(
			__out LPWSTR,
			__in __format_string LPCWSTR,
			...);
		#ifdef UNICODE
		#define wsprintf  wsprintfW
		#else
		#define wsprintf  wsprintfA
		#endif // !UNICODE

		// for MSInst2 project MSICOM configuration

		#if !defined(MIDL_PASS)
		extern "C"
		DWORD
		WINAPI
		FormatMessageA(
			DWORD dwFlags,
			LPCVOID lpSource,
			DWORD dwMessageId,
			DWORD dwLanguageId,
			LPSTR lpBuffer,
			DWORD nSize,
			va_list *Arguments
			);

		extern "C"
		DWORD
		WINAPI
		FormatMessageW(
			DWORD dwFlags,
			LPCVOID lpSource,
			DWORD dwMessageId,
			DWORD dwLanguageId,
			LPWSTR lpBuffer,
			DWORD nSize,
			va_list *Arguments
			);
		#ifdef UNICODE
		#define FormatMessage  FormatMessageW
		#else
		#define FormatMessage  FormatMessageA
		#endif // !UNICODE

		#if defined(_M_CEE)
		#undef FormatMessage
		__inline
		DWORD
		FormatMessage(
			DWORD dwFlags,
			LPCVOID lpSource,
			DWORD dwMessageId,
			DWORD dwLanguageId,
			LPTSTR lpBuffer,
			DWORD nSize,
			va_list *Arguments
			)
		{
		#ifdef UNICODE
			return FormatMessageW(
		#else
			return FormatMessageA(
		#endif
				dwFlags,
				lpSource,
				dwMessageId,
				dwLanguageId,
				lpBuffer,
				nSize,
				Arguments
			);
		}
		#endif 	/* _M_CEE */
		#endif	/* MIDL_PASS */



		extern "C"
		HANDLE
		WINAPI
		LocalFree(
			__deref HANDLE hMem
			);

		//end of changes nprabhudessai: 01/17/06

		// 2/13/2006 qxue: added the following function declarations for Windows MSICOM build
		#ifndef _FILETIME_
		#define _FILETIME_
		typedef struct _FILETIME {
			DWORD dwLowDateTime;
			DWORD dwHighDateTime;
		} FILETIME, *PFILETIME, *LPFILETIME;
		#endif

		extern "C"
		LPVOID
		WINAPI
		VirtualAlloc(
			__in_opt LPVOID lpAddress,
			__in     SIZE_T dwSize,
			__in     DWORD flAllocationType,
			__in     DWORD flProtect
			);

		extern "C"
		SIZE_T
		WINAPI
		VirtualQuery(
			__in_opt LPCVOID lpAddress,
			__out_bcount_part(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer,
			__in     SIZE_T dwLength
			);

		extern "C"
		BOOL
		WINAPI
		GetProcessWorkingSetSize(
			__in  HANDLE hProcess,
			__out PSIZE_T lpMinimumWorkingSetSize,
			__out PSIZE_T lpMaximumWorkingSetSize
			);

		extern "C"
		DWORD
		WINAPI
		GetProcessHeaps(
			__in DWORD NumberOfHeaps,
			__out_ecount_part(NumberOfHeaps, return) PHANDLE ProcessHeaps
			);

		extern "C"
		__out
		HANDLE
		WINAPI
		GetStdHandle(
			__in DWORD nStdHandle
			);

		extern "C"
		BOOL
		WINAPI
		SetStdHandle(
			__in DWORD nStdHandle,
			__in HANDLE hHandle
			);

		extern "C"
		BOOL
		WINAPI
		GetThreadTimes(
			__in  HANDLE hThread,
			__out LPFILETIME lpCreationTime,
			__out LPFILETIME lpExitTime,
			__out LPFILETIME lpKernelTime,
			__out LPFILETIME lpUserTime
			);

		/* Global Memory Flags */
		#define GMEM_FIXED          0x0000
		#define GMEM_MOVEABLE       0x0002
		#define GMEM_NOCOMPACT      0x0010
		#define GMEM_NODISCARD      0x0020
		#define GMEM_ZEROINIT       0x0040
		#define GMEM_MODIFY         0x0080
		#define GMEM_DISCARDABLE    0x0100
		#define GMEM_NOT_BANKED     0x1000
		#define GMEM_SHARE          0x2000
		#define GMEM_DDESHARE       0x2000
		#define GMEM_NOTIFY         0x4000
		#define GMEM_LOWER          GMEM_NOT_BANKED
		#define GMEM_VALID_FLAGS    0x7F72
		#define GMEM_INVALID_HANDLE 0x8000

		#define GHND                (GMEM_MOVEABLE | GMEM_ZEROINIT)
		#define GPTR                (GMEM_FIXED | GMEM_ZEROINIT)

		extern "C"
		__out
		HGLOBAL
		WINAPI
		GlobalFree(
			__deref HGLOBAL hMem
			);

		extern "C"
		__out
		HGLOBAL
		WINAPI
		GlobalAlloc (
			__in UINT uFlags,
			__in SIZE_T dwBytes
			);

		extern "C"
		DWORD
		WINAPI
		GetModuleFileNameA(
			__in_opt HMODULE hModule,
			__out_ecount_part(nSize, return + 1) char* lpFilename,
			__in     DWORD nSize
			);

		extern "C"
		DWORD
		WINAPI
		GetModuleFileNameW(
			__in_opt HMODULE hModule,
			__out_ecount_part(nSize, return + 1) wchar_t* lpFilename,
			__in     DWORD nSize
			);
		#ifdef UNICODE
		#define GetModuleFileName  GetModuleFileNameW
		#else
		#define GetModuleFileName  GetModuleFileNameA
		#endif // !UNICODE

		extern "C"
		__out
		HANDLE
		WINAPI
		GetCurrentProcess(
			VOID
			);

		extern "C"
		DWORD
		WINAPI
		GetCurrentProcessId(
			VOID
			);

		extern "C"
		DWORD
		WINAPI
		GetCurrentThreadId(
			VOID
			);

		extern "C"
		__out
		HMODULE
		WINAPI
		LoadLibraryA(
			__in LPCSTR lpLibFileName
			);
		extern "C"
		__out
		HMODULE
		WINAPI
		LoadLibraryW(
			__in LPCWSTR lpLibFileName
			);
		#ifdef UNICODE
		#define LoadLibrary  LoadLibraryW
		#else
		#define LoadLibrary  LoadLibraryA
		#endif // !UNICODE
		extern "C"
		BOOL
		WINAPI
		FreeLibrary (
			__in HMODULE hLibModule
			);
		extern "C"
		__out
		HANDLE
		WINAPI
		CreateMutexA(
			__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
			__in     BOOL bInitialOwner,
			__in_opt LPCSTR lpName
			);
		extern "C"
		__out
		HANDLE
		WINAPI
		CreateMutexW(
			__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
			__in     BOOL bInitialOwner,
			__in_opt LPCWSTR lpName
			);
		#ifdef UNICODE
		#define CreateMutex  CreateMutexW
		#else
		#define CreateMutex  CreateMutexA
		#endif // !UNICODE

		extern "C"
		BOOL
		WINAPI
		ReleaseMutex(
			__in HANDLE hMutex
			);

		extern "C"
		BOOL
		WINAPI
		SetEvent(
			__in HANDLE hEvent
			);

		extern "C"
		DWORD
		WINAPI
		WaitForSingleObject(
			__in HANDLE hHandle,
			__in DWORD dwMilliseconds
			);

			extern "C"
			__out
			HANDLE
			WINAPI
			CreateFileMappingA(
				__in     HANDLE hFile,
				__in_opt LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
				__in     DWORD flProtect,
				__in     DWORD dwMaximumSizeHigh,
				__in     DWORD dwMaximumSizeLow,
				__in_opt LPCSTR lpName
				);
			extern "C"
			__out
			HANDLE
			WINAPI
			CreateFileMappingW(
				__in     HANDLE hFile,
				__in_opt LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
				__in     DWORD flProtect,
				__in     DWORD dwMaximumSizeHigh,
				__in     DWORD dwMaximumSizeLow,
				__in_opt LPCWSTR lpName
				);
			#ifdef UNICODE
			#define CreateFileMapping  CreateFileMappingW
			#else
			#define CreateFileMapping  CreateFileMappingA
			#endif // !UNICODE


			extern "C"
			__out
			HANDLE
			WINAPI
			OpenFileMappingA(
				__in DWORD dwDesiredAccess,
				__in BOOL bInheritHandle,
				__in LPCSTR lpName
				);

			extern "C"
			__out
			HANDLE
			WINAPI
			OpenFileMappingW(
				__in DWORD dwDesiredAccess,
				__in BOOL bInheritHandle,
				__in LPCWSTR lpName
				);
			#ifdef UNICODE
			#define OpenFileMapping  OpenFileMappingW
			#else
			#define OpenFileMapping  OpenFileMappingA
			#endif // !UNICODE


			extern "C"
			__out
			LPVOID
			WINAPI
			MapViewOfFile(
				__in HANDLE hFileMappingObject,
				__in DWORD dwDesiredAccess,
				__in DWORD dwFileOffsetHigh,
				__in DWORD dwFileOffsetLow,
				__in SIZE_T dwNumberOfBytesToMap
				);

			extern "C"
			__out
			BOOL
			WINAPI
			UnmapViewOfFile(
				__in LPCVOID lpBaseAddress
				);

			extern "C"
			DWORD
			WINAPI
			WaitForMultipleObjects(
				__in DWORD nCount,
				__in_ecount(nCount) CONST HANDLE *lpHandles,
				__in BOOL bWaitAll,
				__in DWORD dwMilliseconds
				);

			extern "C"
			BOOL
			WINAPI
			LocalFileTimeToFileTime(
				__in  CONST FILETIME *lpLocalFileTime,
				__out LPFILETIME lpFileTime
				);

			extern "C"
			BOOL
			WINAPI
			SystemTimeToFileTime(
				__in  CONST SYSTEMTIME *lpSystemTime,
				__out LPFILETIME lpFileTime
				);

			#define WAIT_FAILED ((DWORD)0xFFFFFFFF)
			#define WAIT_OBJECT_0       ((STATUS_WAIT_0 ) + 0 )

			extern "C"
			BOOL
			WINAPI
			WritePrivateProfileStringA(
				__in_opt LPCSTR lpAppName,
				__in_opt LPCSTR lpKeyName,
				__in_opt LPCSTR lpString,
				__in_opt LPCSTR lpFileName
				);

			extern "C"
			BOOL
			WINAPI
			WritePrivateProfileStringW(
				__in_opt LPCWSTR lpAppName,
				__in_opt LPCWSTR lpKeyName,
				__in_opt LPCWSTR lpString,
				__in_opt LPCWSTR lpFileName
				);
#ifdef UNICODE
#define WritePrivateProfileString  WritePrivateProfileStringW
#else
#define WritePrivateProfileString  WritePrivateProfileStringA
#endif // !UNICODE


			extern "C"
			UINT
			WINAPI
			GetPrivateProfileIntA(
				__in     LPCSTR lpAppName,
				__in     LPCSTR lpKeyName,
				__in     INT nDefault,
				__in_opt LPCSTR lpFileName
				);
			extern "C"
			UINT
			WINAPI
			GetPrivateProfileIntW(
				__in     LPCWSTR lpAppName,
				__in     LPCWSTR lpKeyName,
				__in     INT nDefault,
				__in_opt LPCWSTR lpFileName
				);
			#ifdef UNICODE
			#define GetPrivateProfileInt  GetPrivateProfileIntW
			#else
			#define GetPrivateProfileInt  GetPrivateProfileIntA
			#endif // !UNICODE

			extern "C"
			DWORD
			WINAPI
			GetPrivateProfileStringA(
				__in_opt LPCSTR lpAppName,
				__in_opt LPCSTR lpKeyName,
				__in_opt LPCSTR lpDefault,
				__out_ecount_part_opt(nSize, return + 1) LPSTR lpReturnedString,
				__in     DWORD nSize,
				__in_opt LPCSTR lpFileName
				);
			extern "C"
			DWORD
			WINAPI
			GetPrivateProfileStringW(
				__in_opt LPCWSTR lpAppName,
				__in_opt LPCWSTR lpKeyName,
				__in_opt LPCWSTR lpDefault,
				__out_ecount_part_opt(nSize, return + 1) LPWSTR lpReturnedString,
				__in     DWORD nSize,
				__in_opt LPCWSTR lpFileName
				);
			#ifdef UNICODE
			#define GetPrivateProfileString  GetPrivateProfileStringW
			#else
			#define GetPrivateProfileString  GetPrivateProfileStringA
			#endif // !UNICODE

			extern "C"
			BOOL
			WINAPI
			LookupPrivilegeValueA(
				__in_opt LPCSTR lpSystemName,
				__in     LPCSTR lpName,
				__out    PLUID   lpLuid
				);
			extern "C"
			BOOL
			WINAPI
			LookupPrivilegeValueW(
				__in_opt LPCWSTR lpSystemName,
				__in     LPCWSTR lpName,
				__out    PLUID   lpLuid
				);
			#ifdef UNICODE
			#define LookupPrivilegeValue  LookupPrivilegeValueW
			#else
			#define LookupPrivilegeValue  LookupPrivilegeValueA
			#endif // !UNICODE


			extern "C"
			BOOL
			WINAPI
			AdjustTokenPrivileges (
				__in      HANDLE TokenHandle,
				__in      BOOL DisableAllPrivileges,
				__in_opt  PTOKEN_PRIVILEGES NewState,
				__in      DWORD BufferLength,
				__out_bcount_part_opt(BufferLength, *ReturnLength) PTOKEN_PRIVILEGES PreviousState,
				__out_opt PDWORD ReturnLength
				);

			extern "C"
			VOID
			WINAPI
			InitializeCriticalSection(
				__out CRITICAL_SECTION* lpCriticalSection
				);

			extern "C"
			VOID
			WINAPI
			EnterCriticalSection(
				__inout CRITICAL_SECTION* lpCriticalSection
				);

			extern "C"
			VOID
			WINAPI
			LeaveCriticalSection(
				__inout CRITICAL_SECTION* lpCriticalSection
				);

			#if (_WIN32_WINNT >= 0x0403)
			extern "C"
			BOOL
			WINAPI
			InitializeCriticalSectionAndSpinCount(
				__out CRITICAL_SECTION* lpCriticalSection,
				__in  DWORD dwSpinCount
				);

			extern "C"
			DWORD
			WINAPI
			SetCriticalSectionSpinCount(
				__inout CRITICAL_SECTION* lpCriticalSection,
				__in    DWORD dwSpinCount
				);
			#endif

			#if(_WIN32_WINNT >= 0x0400)
			extern "C"
			BOOL
			WINAPI
			TryEnterCriticalSection(
				__inout CRITICAL_SECTION* lpCriticalSection
				);
			#endif /* _WIN32_WINNT >= 0x0400 */

			extern "C"
			VOID
			WINAPI
			DeleteCriticalSection(
				__inout CRITICAL_SECTION* lpCriticalSection
				);


	#endif // WIN32


	#endif // _WINBASE_
#endif // WIN32 && !_USE_MSI_COM

// this must be the *last* file included
#include "ProtectedSource/Epilog.h"

#endif // PDCwinbase_h
