// ======================================================================================
// File Name:	COMShared.h
// Authors:		Liqun Jin
// Created:		June 12, 2002
//	Copyright (C) MicroStrategy, Inc. 2002
// ======================================================================================

// M5COMShr.Dll export definition

#ifndef COMSHARED_INCLUDE_H
#define COMSHARED_INCLUDE_H

#ifdef WIN32

	#ifdef COMSHARED_EXPORTS
	#define COMSHARED_API __declspec(dllexport)
	#else
	#define COMSHARED_API __declspec(dllimport)
	#endif

#else
	#define	COMSHARED_API
#endif

#endif //COMSHARED_INCLUDE_H


