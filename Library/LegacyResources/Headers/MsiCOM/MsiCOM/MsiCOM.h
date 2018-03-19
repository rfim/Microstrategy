//==============================================================================================
//	FILENAME	:	MsiCOM.h
//	AUTHOR		:	Jing Li
//	CREATION	:	5/2/02
//	Copyright (C) MicroStrategy Incorporated 2002
//==============================================================================================
#ifndef MMsiCOM_MsiCOM_h
#define MMsiCOM_MsiCOM_h

#ifdef WIN32

	#ifdef MSICOM_DLL_EXPORT
		#define	DLL_MSICOM_EXIM	_declspec(dllexport)
	#else
		#define	DLL_MSICOM_EXIM	_declspec(dllimport)
	#endif

#else
	#define	DLL_MSICOM_EXIM
#endif

#endif // MMsiCOM_MsiCOM_h
