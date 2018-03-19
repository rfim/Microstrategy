//
//  DFCLog.h
//  DFCComUtil
//
//  Created by ydong on 12/19/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

//  Fake DFCLog class

#ifndef DFCLog_h
#define DFCLog_h

//#pragma warning(disable:4251) // class 'type' needs to have dll-interface to be used by clients of class 'type2'

// include type DFCLogMask
#include "Includes/Globals.h"

// include dll export
//#include "Includes/DFCComutil_Includes.h"

#include "Base/Base/String.h"
//#include "Base/Base/System.h" // MBase::GetCurrentThreadID
//#include "Synch/Synch/CriticalSection.h"

// define some marocs
// the ENDL is used by MsiLog as enum, redefine it as "\n"
#define ENDL _ENDLN
#define	ENGINE_LOG_BEGIN    (GetEngineLog()) << _LOG_BEGIN; try {
#define	ENGINE_LOG          (GetEngineLog()) << _GET_HANDLE
#define ENGINE_LOG_END      (GetEngineLog()) << _LOG_END; } catch (...) { throw; ENGINE_LOG << _LOG_END; }

// if some message doesn't wanted to be buffered, use this.
#define DFCTRACE(x) ENGINE_LOG << _LOG_FLUSH << x << _ENDLN << _LOG_END

enum DFC_LOG_TYPE {
    _LOG_BEGIN,
    _GET_HANDLE,
    _LOG_END,
    _LOG_FLUSH,
    _LOG_HEX,
    _LOG_HEXALL,
    _LOG_DEC,
    _ENDLN
};

#define DFCLOG_BUF_SIZE 1
#define DFCLOG_BUF_START_DUMP_SIZE 1

class DFCLog {
public:
	DFCLog() {};
	~DFCLog() {};
    
	DFCLog& operator << (short	iShort )                { return *this; };
	DFCLog& operator << (unsigned short	iShort )        { return *this; };
	DFCLog& operator << (Int32	iInt )                  { return *this; };
	DFCLog& operator << (unsigned Int32	iInt )          { return *this; };
	DFCLog& operator << (float	iFloat )                { return *this; };
	DFCLog& operator << (double iDouble )               { return *this; };
	DFCLog& operator << (DFC_LOG_TYPE	iLogType )      { return *this; };
	DFCLog& operator << (const wchar_t	*iChar )        { return *this; };
	DFCLog& operator << (const MBase::String&	iChar ) { return *this; };
	DFCLog& operator << (const GUID& iGUID)             { return *this; };
	DFCLog& operator << (Int64 iInt )                   { return *this; };
	DFCLog& operator << (unsigned Int64 iInt )          { return *this; };
	DFCLog& operator << (long iInt )                    { return *this; };
	DFCLog& operator << (unsigned long iInt )           { return *this; };
	bool IsEnabled()                                    { return false; };
    
private:
    
	void HexState()                                             {};
	void LogMsg2(bool iForceDump, const TCHAR *iFormatString)   {};
    
};


// two externs
extern DFCLog *gEngineLog;
extern DFCLogMask gLogMask;

inline DFCLog & GetEngineLog() {
    if (!gEngineLog) {
        gEngineLog = new DFCLog();
    }
    return *gEngineLog;
}


#endif // DFCLog_h
