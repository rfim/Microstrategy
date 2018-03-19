//==============================================================================================
//	FILENAME	:	SharedMemoryIncl.h
//	AUTHOR		:	Ananth Kini
//	CREATION	:	05/15/2007
//	Copyright (C) MicroStrategy Incorporated 2007
//	All Rights Reserved
//==============================================================================================
#ifndef MSharedMemory_SharedMemoryIncl_h
#define MSharedMemory_SharedMemoryIncl_h

#if defined(WIN32) || defined(WIN64)
	#ifdef SHAREDMEMORY_DLL_EXPORT
		#define	DLL_SHAREDMEMORY_EXIM _declspec(dllexport)
	#else
		#define	DLL_SHAREDMEMORY_EXIM _declspec(dllimport)
	#endif

#else
	#define	DLL_SHAREDMEMORY_EXIM
#endif

#ifdef _MSC_VER
#pragma warning(disable:4275) // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#pragma warning(disable:4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#endif

#endif // MSharedMemory_SharedMemoryIncl_h
