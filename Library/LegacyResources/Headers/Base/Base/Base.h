//==============================================================================================
//	FILENAME	:	Base.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Base_h
#define MBase_Base_h

#ifdef WIN32

	#ifdef BASE_DLL_EXPORT
		#define	DLL_BASE_EXIM _declspec(dllexport)
	#else
		#define	DLL_BASE_EXIM _declspec(dllimport)
	#endif

#else
	#define	DLL_BASE_EXIM
#endif

#ifdef _MSC_VER
#pragma warning(disable:4275) // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#pragma warning(disable:4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#endif

#endif // MBase_Base_h
