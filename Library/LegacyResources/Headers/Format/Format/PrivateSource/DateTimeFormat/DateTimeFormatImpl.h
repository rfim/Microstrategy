//==============================================================================================
//	FILENAME	:	DateTimeFormatImpl.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	9/10/01
//	Modified by Liang Liu at May 27, 2010. 
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_DateTimeFormatImpl_h
#define MFormat_DateTimeFormatImpl_h

#include "Format/Format/DateTimeFormat.h"
#include "Base/Base/BaseString.h"
#include "common/DSSDateTime.h"	//kernel/iphone/ReportServiceCore/Headers/common/DateTime.h

namespace MFormat
{
	class DateTimeFormatImpl:
		public DateTimeFormat
	{
	public:

		DateTimeFormatImpl(const WCHAR* ipcPattern = NULL, 
#ifndef __ANDROID__			
			CFLocaleRef ipLocale = NULL);
#else
			LCID inLocale = 0);
#endif

		void Delete() throw ();

		virtual ~DateTimeFormatImpl() throw ();

		MBase::ReturnWString Format(const MDataType::DSSDateTime& irTime, int& oResult) const;
		WCHAR* Format2(const MDataType::DSSDateTime& irTime, int& oResult) const;

	private:
		MBase::String mPattern;
#ifndef __ANDROID__	
		CFLocaleRef mpLocale;
#else
		LCID mnLocale;
#endif

		void hFormat(const MDataType::DSSDateTime& irTime, const WCHAR* ipcPattern, int& oResult, MBase::String& oOutput) const;


		typedef struct _SYSTEMTIME
		{
			WORD wYear;
			WORD wMonth;
			WORD wDayOfWeek;
			WORD wDay;
			WORD wHour;
			WORD wMinute;
			WORD wSecond;
			WORD wMilliseconds;
		}
		SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#ifndef __ANDROID__
		int hGetDateFormat(CFLocaleRef inLocale, unsigned int inFlags, const SYSTEMTIME* ipDate, const WCHAR* ipcFormat, WCHAR* opcDateStr, int inSize) const;
		int hGetTimeFormat(CFLocaleRef inLocale, unsigned int inFlags, const SYSTEMTIME* ipTime, const WCHAR* ipcFormat, WCHAR* opcTimeStr, int inSize) const;
#else
		int hGetDateFormat(LCID inLocale, unsigned int inFlags, const SYSTEMTIME* ipDate, const WCHAR* ipcFormat, WCHAR* opcDateStr, int inSize) const;
		int hGetTimeFormat(LCID inLocale, unsigned int inFlags, const SYSTEMTIME* ipTime, const WCHAR* ipcFormat, WCHAR* opcTimeStr, int inSize) const;
#endif
		bool isMinuteSecondFormat(WCHAR* ipcFormat, size_t inFormatCurPos) const;

	};
}

#endif // MFormat_DateTimeFormatImpl_h
