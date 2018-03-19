//==============================================================================================
//	FILENAME	:	ModuleMain.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-09-30
//	Copyright (C) MicroStrategy Incorporated 2002
//==============================================================================================
#ifndef MSynch_ModuleMain_h
#define MSynch_ModuleMain_h

#include "Base/Defines/ModuleMainPtr.h"

namespace MSynch
{
	// 2002-07-15 vovechkin
	// this class holds all "global" variables of this project.
	// initialization is done in the order of member declarations.
	class ModuleMain
	{
	public:
		ModuleMain();
		~ModuleMain() throw();

	};

	// declare a global variable, which holds a singleton of class ModuleMain
	extern MBase::ModuleMainPtr< ModuleMain > gModuleMainPtr;
}

#endif // MSynch_ModuleMain_h
