//==============================================================================================
//	FILENAME	:	Prolog.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2002-07-16
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
//
// This file is included *first* by *all* PDC headers, so keep this file lean.
//
// Note: there are no inclusion guards, because this file is supposed to "execute"
//       whenever it's included, not only the first time!

#ifdef WIN32
#include "CheckWin32APIAliases.h"
#endif
