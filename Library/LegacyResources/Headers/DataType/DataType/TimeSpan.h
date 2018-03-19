//==============================================================================================
//	FILENAME	:	TimeSpan.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_TimeSpan_h
#define MDataType_TimeSpan_h

#include "Base/Defines/BasicTypes.h"

#include "DataType.h"

namespace MDataType 
{
#ifndef DATETIME_CONST
#define DATETIME_CONST
//ydong
	const Int64 DAY_IN_MILLISECONDS = (24*60*60*1000);
	const Int64 HOUR_IN_MILLISECONDS = (60*60*1000);
	const Int64 MINUTE_IN_MILLISECONDS = (60*1000);
	const Int64 SECOND_IN_MILLISECONDS = 1000;
	const Int64 MILLISECOND_IN_NANOSECONDS = 1000000;

	const Int32 DAY_IN_HOURS = 24;
	const Int32 DAY_IN_MINUTES = (24*60);
	const Int32 DAY_IN_SECONDS = (24*60*60);
#endif
	//==============================================================================================
	// TimeSpan represents a period of time
	// It's a stack object. 
	// It's size is IMMUTABLE - you cannot make a bug fix to create a new member!!!
	//==============================================================================================

	class DLL_DATATYPE_EXIM TimeSpan
	{
	public:

		TimeSpan(Int64 inMilliSeconds=0);
		TimeSpan(Int32 inDays, Int32 inHours, Int32 inMinutes=0, Int32 inSeconds=0, Int32 inMilliseconds=0);
		TimeSpan(const TimeSpan&);
		
		~TimeSpan() throw();

		// Get TimeSpan in number of days, not limited by 1 - 31
		Int32 GetTotalDays() const;
		// Get TimeSpan in number of hours, not limited by 0 - 23
		Int32 GetTotalHours() const;
		// Get TimeSpan in number of minutes, not limited by 0 - 59
		Int32 GetTotalMinutes() const;
		// Get TimeSpan in number of seconds, not limited by 0 - 59
		Int32 GetTotalSeconds() const;
		// Get TimeSpan in number of milliseconds
		Int64 GetTotalMilliseconds() const;

		TimeSpan& operator = (const TimeSpan& irSrc);

		const TimeSpan& operator -= (const TimeSpan& irOther);
		const TimeSpan& operator += (const TimeSpan& irOther);

		TimeSpan operator - (const TimeSpan& irOther) const;
		TimeSpan operator + (const TimeSpan& irOther) const;

		bool operator == (const TimeSpan& irOther) const;
		bool operator != (const TimeSpan& irOther) const;
		bool operator > (const TimeSpan& irOther) const;
		bool operator < (const TimeSpan& irOther) const;
		bool operator >= (const TimeSpan& irOther) const;
		bool operator <= (const TimeSpan& irOther) const;

	private:
		Int64 mnMilliseconds;
	};
}

#endif // MDataType_TimeSpan_h
