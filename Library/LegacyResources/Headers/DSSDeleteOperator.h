/*
 *  DSSDeleteOperator.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/13/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

//==============================================================================================
//	copied from /Common/Base/Define/DSSDeleteOperator.h
//==============================================================================================

#ifndef MBase_DeleteOperator_h
#define MBase_DeleteOperator_h

//#include "Null.h"
#import <stdlib.h>

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

#endif
