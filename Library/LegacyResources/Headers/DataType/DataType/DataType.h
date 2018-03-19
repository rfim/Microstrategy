//==============================================================================================
//	FILENAME	:	DataType.cpp
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================

// M8DatTy2.Dll export definition

#ifndef DATATYPE_INCLUDE_H
#define DATATYPE_INCLUDE_H

#ifdef WIN32

	#ifdef DATATYPE_DLL_EXPORT
		#define	DLL_DATATYPE_EXIM	_declspec(dllexport)
	#else
		#define	DLL_DATATYPE_EXIM	_declspec(dllimport)
	#endif

#else
	#define	DLL_DATATYPE_EXIM
#endif

#endif //DATATYPE_INCLUDE_H
