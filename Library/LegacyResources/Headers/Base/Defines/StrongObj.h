//==============================================================================================
//	FILENAME	:	StrongObj.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/01
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_StrongObj_h
#define MBase_StrongObj_h

#include "StrongBase.h"

namespace MBase
{
	/**
	 * StrongObj is the equivalent of a StrongPtr for objects such as HANDLES.
	 * Please read the comments on the StrongPtr.
	 */
	template<class T,class DeleteOperator>
	class StrongObj:public StrongBase<T,DeleteOperator>
	{
	public:
		explicit StrongObj(T ipData = NULL) throw() : StrongBase<T,DeleteOperator>(ipData)
		{
		}

		void Acquire(StrongObj<T, DeleteOperator>& iStrongObj) throw()
		{
			Reset(iStrongObj.GiveUp());
		}

		void Acquire(T& irData) throw()
		{
			Reset(irData);
			irData=NULL;
		}
	};
}

#endif // MBase_StrongObj_h
