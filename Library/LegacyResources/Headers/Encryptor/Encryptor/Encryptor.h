//==============================================================================================
//	FILENAME	:	Encryptor.h
//	AUTHOR		:	Yuan Ding
//	CREATION	:	11/15/2001
//	Copyright (C) MicroStrategy Incorporated 1999
//  All Rights Reserved
//==============================================================================================

/// M8Encryp2.Dll export definition
#ifndef Encryptor_Encryptor_h
#define Encryptor_Encryptor_h

#ifdef WIN32

	#ifdef ENCRYPTOR_DLL_EXPORT
		#define	DLL_ENCRYPTOR_EXIM	_declspec(dllexport)
	#else
		#define	DLL_ENCRYPTOR_EXIM	_declspec(dllimport)
	#endif

#else
	#define	DLL_ENCRYPTOR_EXIM
#endif

#endif //Encryptor_Encryptor_h
