//==============================================================================================
//	FILENAME	:	Language.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2005-02-03
//	Copyright (C) MicroStrategy Incorporated 2005
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_Language_h
#define MBase_Language_h

#include "Base.h"

namespace MBase
{
	/**
	 *
	 *  A language ID is a 16 bit value which is the combination of a
	 *  primary language ID and a secondary language ID.  The bits are
	 *  allocated as follows:
	 *
	 *       +-----------------------+-------------------------+
	 *       |     Sublanguage ID    |   Primary Language ID   |
	 *       +-----------------------+-------------------------+
	 *        15                   10 9                       0   bit
	 *
	 */
	typedef unsigned short LanguageID;

	/**
	 * @return the "primary language ID" component of the input parameter.
	 */
	inline LanguageID GetPrimaryLanguageID(LanguageID iLanguageID)
	{
		return (LanguageID) (iLanguageID & 0x03FF);
	}

	inline LanguageID MakeLanguageID(LanguageID iPrimaryLanguageID, LanguageID iSubLanguageID)
	{
		return (LanguageID)  ( iPrimaryLanguageID | (iSubLanguageID << 10) );
	}
}

#endif // MBase_Language_h
