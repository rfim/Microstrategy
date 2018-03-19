//==============================================================================================
//	FILENAME	:	DeleteOperator.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	9/26/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================
#ifndef MBase_DeleteOperator_h
#define MBase_DeleteOperator_h

#include "Null.h"

namespace MBase
{
	/**
	 * A feature of Smart/Strong pointers is that they take a "functor" (class that
	 * defines operator() ) as a template operator. This allows you to declare
	 * a Smart/Strong pointer with whatever delete method is appropriate.
	 * Since a Smart/Strong pointer may delete a resource in
	 * different ways (delete, delete [], ::LocalFree(), etc.)
	 * A class with a single method (functor) is used to encapsulate this
	 * This class is used by Smart/Strong Pointers.
	 */

	////////////////////////
	// C++ delete
	template<class T>
	class DeleteC
	{
	public:
		void operator()(T* ipData) throw()
		{
			if (ipData!=NULL)
			{
				delete ipData;
			}
		}
	};

	///////////////////////////////
	// C++ array delete
	template<class T>	// Delete used by arrays
	class DeleteArray
	{
	public:
		void operator()(T* ipData) throw()
		{
			if (ipData!=NULL)
			{
				delete [] ipData;
			}
		}
	};

	//////////////////////////
	// DeleteOperatorGeneric
	template<class T>
	class DeleteOperatorGeneric
	{
	public:
		void operator()(T* ipData) throw()
		{
			if (ipData)
			{
				ipData->Delete();
			}
		}
	};

	////////////////////////
	//xhan 09/25/05, Don't delete, just call the destructor of T
	// C++ destroy 
	template<class T>
	class Destroy
	{
	public:
		void operator()(T* ipData) throw()
		{
			if (ipData!=NULL)
			{
				ipData->~T();
			}
		}
	};

}

#endif // MBase_DeleteOperator_h
