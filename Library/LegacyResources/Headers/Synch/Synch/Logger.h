//==============================================================================================
//	FILENAME	:	Logger.h
//	AUTHOR		:	Juan Pablo Muraira
//	CREATION	:	10/23/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//==============================================================================================
#ifndef MSynch_Logger_h
#define MSynch_Logger_h

#include "PDCHeader/PDCwchar.h"

#include "Base/Interfaces/ReferenceCounted.h"
#include "Base/Defines/SmartPtrI.h"
#include "Base/Base/LastError.h"

namespace MSynch
{
	///////////////////////////////////////////////////////////////////////////////
	// Another component should implement MSynch::Logger interface and pass it
	// to the synch component. Synch will log its messages through this interface.
	// Only fatal errors will be logged (Lock order violations).
	////////////////////////////////////////////////////////////////////////////////
	class Logger:
		virtual public MBase::ReferenceCounted
	{
	public:
		typedef MBase::SmartPtrI<Logger> Ptr;
		typedef MBase::ReturnPtrI<Logger> ReturnPtr;

		virtual void LogFatal(const char* ipMessage, MBase::ErrorCode iMessageID) const = 0;
	};

	// this typedef is obsolete
	typedef Logger::Ptr LoggerPtr;
}

#endif // MSynch_Logger_h
