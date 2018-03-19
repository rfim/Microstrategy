//	Archive: /Castor/DSSCOM/SourceCode/ATL/DSSAtlbase.h
//	Start date: 19th March 1999
//	Inital author: Hurwood

//	Copyright (C) 1999 MicroStrategy Incorporated
//	All rights reserved

// This header file is used to include the DSS replacement for ATL
// It should be included almost first of all
// In particular it will include MsiPlatformSupport.  This header
// must not be oncluded before this file, or compilation will fail
#ifndef DSSATL_INCLUDES_H
#define DSSATL_INCLUDES_H 1

#include "Synch/Synch/CriticalSection.h"
#include "Synch/Synch/RWLock.h"
#include "Synch/Synch/LockCategories.h"

#ifdef _USE_MSI_COM
	#include "PDCHeader/PDCatlbase.h"
#else
	// include MSI replacement of the <atlbase.h> header file
	#include "ATL/DssAtlbase.h"
#endif //_USE_MSI_COM

// You may derive a class from CComModule and use it if you want to override
// something, but do not change the name of _Module
extern CComModule _Module;

#ifdef _USE_MSI_COM
	#include "PDCHeader/PDCatlcom.h"
#else
	// include MSI replacement of the <atlcom.h> header file
	#include "ATL/DssAtlcom.h"
#endif //_USE_MSI_COM

#endif // DSSATL_INCLUDES_H
