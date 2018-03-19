//==============================================================================================
//	FILENAME	:	System.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2003-07-26
//	Copyright (C) MicroStrategy Incorporated 2003
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_System_h
#define MBase_System_h

#include "Base.h"

#include "ReturnString.h"
#include "BaseString.h"

////////////////////////////////////////////////////////////////////////////////////////
// 2003-07-26 vovechkin
// NOTE: this header file should not include system-dependent headers,
// such as "wtypes.h"
////////////////////////////////////////////////////////////////////////////////////////

// 2004-06-30 vovechkin
// use the anonymous namespace for this function, 
// to ensure that on UNIX, each module references
// its own copy of this function.
namespace
{
	inline void MBase_DummyFunction()
	{
		return;
	}
}

namespace MBase
{
	/**
	 * This function outputs a string into the "debug" stream on Windows and
	 * stderr on UNIX.
	 *
	 * The debug string will be terminated with \r\n on Windows and \n on UNIX.
	 */
#ifndef NO_WCHAR_SUPPORT
	DLL_BASE_EXIM void OutputDebugString(const WCHAR* ipDebugString) throw();
#endif
	DLL_BASE_EXIM void OutputDebugString(const char* ipDebugString) throw();

	/**
	 * DEPRECATED!!! Do not use this method
	 * @return environment variable value or NULL if the variable is not set.
	 */
	DLL_BASE_EXIM MBase::ReturnWString GetEnvironmentVariableOrNULL(const WCHAR* ipVariableName);

	/**
	 * @return environment variable value. If the variable is not set it is empty.
	 */
	DLL_BASE_EXIM MBase::ReturnWString ReadEnvironmentVariable(const WCHAR* ipVariableName);

	/**
	 * @return the current process identifier.
	 */
	DLL_BASE_EXIM unsigned Int32 GetCurrentProcessID();

	
	/**
	 * @return the computer/machine/host name.
	 */
	DLL_BASE_EXIM ReturnWString GetComputerName();

	/**
	 * @param pID upon return a unique machine ID based on machine address.
	 * @return 0 if successful 
	 */
	DLL_BASE_EXIM Int32 GetUniqueMachineID();

	/**
	 * @returns the process locale
	 * requires dll to be previously initialized (DllMain)
	 */
	DLL_BASE_EXIM const char* GetProcessLocale() throw();

	/**
	 * @return the current thread identifier.
	 *
	 * IMPORTANT: the returned value is NOT guaranteed to be the underlying OS thread id.
	 */
	DLL_BASE_EXIM unsigned Int32 GetCurrentThreadID();

	/**
	 * Yield execution to another thread.
	 *
	 * This method has the same semantics as ::SwitchToThread on Win32,
	 * and ::sched_yield on UNIX.
	 *
	 * NOTE: Do not use MBase::Sleep(0) instead of MSynch::YieldToAnotherThread.
	 */
	DLL_BASE_EXIM void YieldToAnotherThread();

	/**
	 * Efficiently wait for at least the number of milliseconds.
	 */
	DLL_BASE_EXIM void Sleep(unsigned long inMilliseconds);

	/**
	 * @returns the machine name based on the IP addrerss
	 */
	DLL_BASE_EXIM ReturnWString IPAddress2MachineName(const MBase::String& ipAddr) throw();


	/**
	 * @returns the IP addrerss based on the machine name
	 */
	DLL_BASE_EXIM ReturnWString MachineName2IPAddress(const MBase::String& ipMachineName) throw();


	DLL_BASE_EXIM unsigned short GetNumberOfSystemProcessors() throw();

	/**
	 * @return The module filename that corresponds to the given address.
	 *
	 * @see MBase::GetThisModuleFileName()
	 */
	DLL_BASE_EXIM MBase::ReturnWString GetModuleFileNameByAddressWithinModule(const void* ipAddressWithinModule);

	/**
	 * @return The filename of the module, which invoked GetThisModuleFileName().
	 */
	inline MBase::ReturnWString GetThisModuleFileName()
	{
		// 2004-07-26 yuwen
		// some compilers (GNU C++) do not like a reinterpret_cast of a function pointer.
		return GetModuleFileNameByAddressWithinModule((const void*) (&MBase_DummyFunction));
	}

	/**
	 * Return a string representation of the current login:
	 *  on Windows: domain\username
	 *	on UNIX: username
	 */
	DLL_BASE_EXIM MBase::ReturnWString GetCurrentLogin();

	/**
	 * This method returns a pseudo-random sequence of 
	 * integers in the range of 1 to iMaxValue.
	 */
	DLL_BASE_EXIM unsigned Int32 GetPseudoRandomInt32(unsigned Int32 iMaxValue) throw();

	/**
	* Returns true if the machine is big endian, false otherwise
	*/
	inline bool IsBigEndian()
	{
	// 10/13/2004 yuwen make it more robust. 
	// Note that HP on Itanium64 is big endian; NT on Itanium64 is little endian
#if defined(_BIG_ENDIAN) 
	return true;
#elif defined (_LITTLE_ENDIAN)
	return false;
#elif defined(WIN32) || defined(__i386) || defined(__x86_64)
	return false;
#elif defined(__sparc) || defined(_POWER)
	return true;
#else
#error Unknown Platform
#endif
	}

#ifndef WIN32
	/**
	 * Lookup a user name given a user ID.
	 */
	DLL_BASE_EXIM MBase::ReturnWString UserIDToName(int iUserID);

	/**
	 * Lookup a user ID given a user name.
	 */
	DLL_BASE_EXIM int UserNameToID(const WCHAR* ipUserName);

	/**
	 * Lookup a group name given a group ID.
	 */
	DLL_BASE_EXIM MBase::ReturnWString GroupIDToName(int iGroupID);

	/**
	 * Lookup a group ID given a group name.
	 */
	DLL_BASE_EXIM int GroupNameToID(const WCHAR* ipGroupName);
#endif // WIN32

	/*
			Will return the Virtual Bytes(Commited) of the Iserver
	*/
	DLL_BASE_EXIM unsigned long int GetPrivateBytes();

	/*
		Will pause the Iserver process
	*/
	DLL_BASE_EXIM void PauseCurrentProcess();

	//xfan. 12/05/07.	
#ifdef sun
#define SYSTEM_PAGE_SIZE 8192
#else
#define SYSTEM_PAGE_SIZE 4096
#endif
	// Get the system page size
	// return false in error condition
	DLL_BASE_EXIM bool GetSymPageSize(size_t *opPageSize);

	/** 
	 * @return Name of current process 
	 */
	DLL_BASE_EXIM ReturnWString GetCurrentProcessName();
}


#endif // MBase_System_h
