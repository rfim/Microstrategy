//==============================================================================================
//	FILENAME	:	ModuleMain.h
//	AUTHOR		:	vovechkin
//	CREATION	:	2004-12-01
//	Copyright (C) MicroStrategy Incorporated 2004
//==============================================================================================
#ifndef MFileSystem_ModuleMain_h
#define MFileSystem_ModuleMain_h

#include "PDCmap"

#include "Synch/Synch/Initializer.h"
#include "Synch/Synch/ProtectedSource/InprocessRecursiveMutex.h"

#include "Base/Defines/ModuleMainPtr.h"
#include "FileImpl.h"
#include "InprocessFileLocker.h"

namespace MFileSystem
{
	// 2002-07-15 vovechkin
	// this class holds all "global" variables of this project.
	// initialization is done in the order of member declarations.
	class ModuleMain
	{
	public:
		ModuleMain();
		~ModuleMain() throw();

#ifndef WIN32
	private:

		// initialize Synch
		const MSynch::Initializer mSynchInitializer;

	public:

		InprocessFileLocker::ReturnPtr GetInprocessFileLocker(const char* ipFilePath, FileHandle iFileHandle);
		mode_t mFileMode;

	protected:

		friend class InprocessFileLocker;
		void ReferenceCountIsAboutToReachZero(const InprocessFileLocker& irInprocessFileLocker);
		void Release( InprocessFileLocker* irInprocessFileLocker);

	private:

		MSynch::InprocessRecursiveMutex mMutex;

		typedef std::map<InprocessFileLocker::ID, InprocessFileLocker*> InprocessFileLockerMap;
		InprocessFileLockerMap mInprocessFileLockerMap;

#endif // WIN32
	};

	// declare a global variable, which holds a singleton of class ModuleMain
	extern MBase::ModuleMainPtr< ModuleMain > gModuleMainPtr;
}

#endif // MFileSystem_ModuleMain_h
