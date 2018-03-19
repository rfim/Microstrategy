//==============================================================================================
//	FILENAME	:	DateTimeFormat.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	9/10/01
//	Modified by Liang Liu at May 27, 2010. 
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_DateTimeFormat_h
#define MFormat_DateTimeFormat_h

#include "PDCHeader/PDCwtypes.h"

#include "Base/Base/ReturnString.h"
#include "Base/Defines/StrongPtr.h"

#include "common/DSSDateTime.h"	//kernel/iphone/ReportServiceCore/Headers/common/DateTime.h

namespace MFormat
{
	// when support for new locales are added, change this value and 
	// add support in DateTimeFormatImpl.cpp DateTimeFormatImpl::hGetDateFormat and 
	//				     					 DateTimeFormatImpl::hGetTimeFormat
	// and DateTimeParser.cpp - DateTimeParser::DateTimeParser()
	const int MAX_SUPPORTED_DATETIME_LOCALES = 33; 
	class DateTimeFormat
	{
	public:
		virtual void Delete() throw () = 0;

		// pattern is case-insensitive except for the cases of "AM/PM", "am/pm", "A/P", "a/p"
		// d	Day of the month in one or two numeric digits, as needed (1 to 31)
		// dd	Day of the month in two numeric digits (01 to 31)
		// ddd	First three letters of the weekday (Sun to Sat)
		// dddd	Full name of the weekday (Sunday to Saturday)
		// w	Day of week ( 1 for Sunday throught 7 for Saturday)
		// m	Month of the year in one or two numeric digits, as needed (1 to 12)
		// mm	Month of the year in two numeric digits (01 to 12)
		// mmm	First three letters of the month (Jan to Dec)
		// mmmm	Full name of the month (January to December)
		// q	Date displayed as the quarter of the year (1 to 4)
		// 278544
		// change "y" interpretation to be the same as "yy"
		// to match GUI output
		// If needed we can have logic of y is other formatting string such as [d]
		// see [h]
		// y	Number of day of the year (1 to 366)
		// yy	Last two digits of the year (01 to 99)
		// yyyy	Full year (0100 to 9999)
		// h	Hour in one or two digits, as needed (0 to 23)
		// hh	Hour in two digits (01 to 23)
		// n	Minute in one or two digits, as needed (0 to 59)
		// nn	Minute in two digits (00 to 59)
		// s	Second in one or two digits, as needed (0 to 59)
		// ss	Second in two digits (00 to 59)
		// AM/PM	Twelve-hour clock with the uppercase letters AM or PM, as appropriate
		// am/pm	Twelve-hour clock with the lowercase letters am pr pm, as appropriate
		// AMPM		Twelve-hour clock with the uppercase letters AM or PM, as appropriate
		// c	Century, 0 for the 1900s,  1 for 2000s, 2 for 2100s
		// [h]	Show date as number of hours since 1899/12/30
		// \	Escape the following character

		// ddddd	Use	the Short Date predefined format, defined in Control Panel/Regional Setting
		//			On unix, it's equivalent to "m/d/yy"
		// dddddd	Use the Long Date predefined format, defined in Control Panel/Regional Setting
		//			On unix, it's equivalent to "w mmmm dd yyyy"
		// t	Uses the time format of the specified locale
		//		On unix, it's equivalent to "hh:nn:ss"

		// the following is ONLY available on Windows platform
		// A/P	Twelve-hour clock with the uppercase letter of A or P, as appropriate
		// a/p	Twelve-hour clock with the lowercase letter of a or p, as appropriate

		// Return 0 - success
		//	anything else - fail, oTime is not valid for use
		static int Parse(WCHAR* ipText, MDataType::DSSDateTime& oTime, LCID inLocale=0);

		// exception-free version of Format. non-zero oResult indicate it's not a valid time format
		virtual MBase::ReturnWString Format(const MDataType::DSSDateTime& iTime, int& oResult) const = 0;

		// caller is responsible to free memory
		virtual WCHAR* Format2(const MDataType::DSSDateTime& iTime, int& oResult) const = 0;
	};

	//////////////
	// Factory Functions
	// If iPattern is NULL, default format is "ddddd" for pure date, "t" for pure time, "ddddd t" for datetime.
#ifndef __ANDROID__	
    typedef const struct __CFLocale* CFLocaleRef;
	DateTimeFormat* CreateDateTimeFormat(const WCHAR* iPattern = NULL, CFLocaleRef ipLocale = NULL);
#else
	DateTimeFormat* CreateDateTimeFormat(const WCHAR* iPattern = NULL, LCID inLocale=0);
#endif

	typedef MBase::StrongPtr<DateTimeFormat, MBase::DeleteOperatorGeneric<DateTimeFormat> > DateTimeFormatPtr;
}

#endif // MFormat_DateTimeFormat_h

