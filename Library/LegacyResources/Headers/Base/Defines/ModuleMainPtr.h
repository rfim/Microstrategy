//==============================================================================================
//	FILENAME	:	ModuleMainPtr.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2001-12-05
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MBase_ModuleMainPtr_h
#define MBase_ModuleMainPtr_h

#include "Asserte.h"

#include "../Interfaces/BaseException.h"
#include "SpinLock.h"

namespace MBase
{
	/**
	 * A smart pointer to a module main object, which is a user-defined per module 
	 * (and per process) singleton.
	 *
	 * Usage: 
	 * 1. Define a class in your library to hold all global variables, call it MYours::ModuleMain;
	 * 2. Create a static instance of MBase::ModuleMainPtr< MYours::ModuleMain >;
	 * 3. Define an exported MYours::Initializer class to call Initialize()/Finalize() methods
	 *    from its constructor/destructor;
	 * 4. Inside your project, use ModuleMainPtr::operator-> to access ModuleMain members.
	 */
	template< class I /* interface */, class T = I /* class implementing I */ >
	class ModuleMainPtr
	{
	public:

		// 2002-08-29 vovechkin -- IMPORTANT:
		// We can't initialize member variables in this contructor
		// because (at least on AIX w/VisualAge) this contructor
		// might be called **after** somebody have called Initialize()
		// i.e. if they use a static C++ object as an initializer.
		ModuleMainPtr()
		{
			// C/C++ compiler guarantees that all global variables without 
			// explicit initializers will be initialized to 0 at program load time.

			// Since a typical variable of this template is a global variable,
			// then we should expect that mnUsageCounter and mpModuleMain are both 0
		}
		
		inline void Initialize();

		inline void Finalize() throw();

		inline I* operator->()
		{
			I* lpModuleMain = mpModuleMain;
			_ASSERTE( lpModuleMain != NULL );
			return lpModuleMain;
		}

		inline I& operator*()
		{
			return *(operator->());
		}

	private:
		SpinLock mSpinLock;
		unsigned Int32 mnUsageCounter;
		I* mpModuleMain;
	};

	template< class I, class T >
	inline void ModuleMainPtr< I, T >::Initialize()
	{
		SpinLock::SmartLock lLock(mSpinLock);

		// sanity check
		_ASSERT( mnUsageCounter >= 0 && mnUsageCounter < 10000 );

		if( ++mnUsageCounter == 1 )
		{
			try
			{
				mpModuleMain = new T;
			}
			catch (MBase::BaseException& e)
			{
				--mnUsageCounter;
				_ASSERT( mnUsageCounter == 0 );

				// module initialization failed!
				_ASSERT( false );

				throw;
			}
			catch(...)
			{
				--mnUsageCounter;
				_ASSERT( mnUsageCounter == 0 );

				// module initialization failed!
				_ASSERT( false );

				throw;
			}
		}
	}

	template< class I, class T >
	inline void ModuleMainPtr< I, T >::Finalize() throw()
	{
		SpinLock::SmartLock lLock(mSpinLock);

		// sanity check
		_ASSERT( mnUsageCounter > 0 && mnUsageCounter <= 10000 );

		if( --mnUsageCounter == 0 )
		{
			// delete as T, which was used for operator new
			// this way, I does not have to have a virtual destructor
			delete static_cast< T* >( mpModuleMain );

			// ground the pointer
			mpModuleMain = NULL;
		}
	}
}

#endif // MBase_ModuleMainPtr_h

