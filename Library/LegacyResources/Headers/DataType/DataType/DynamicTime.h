//==============================================================================================
//	FILENAME	:	DynamicTime.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	6/11/02
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================

#ifndef _DYNAMICTIME_H_
#define _DYNAMICTIME_H_

#include "DataType/DataType/DateTime.h"

namespace MDataType {

//==============================================================================================
// It's size is IMMUTABLE - you can not make a bug fix which creates new member!!!
//==============================================================================================

// according to EnumDSSDynamicTime
typedef enum 
{ 
	TIME_END		= 0,
	TIME_BEGINNING	= 1,
	NOW				= 2,
	TODAY			= 3,
	THIS_WEEK		= 4,
	THIS_MONTH		= 5,
	THIS_YEAR		= 6,
	THIS_SUNDAY		= 7,
	THIS_MONDAY		= 8,
	THIS_TUESDAY	= 9,
	THIS_WEDNESDAY	= 10,
	THIS_THURSDAY	= 11,
	THIS_FRIDAY		= 12,
	THIS_SATURDAY	= 13,
	STATIC_DATE		= 0x20,
	DYNAMIC_HOUR	= 0x40,
	DYNAMIC_MINUTE	= 0x80,
	NOT_DYNAMIC_TIME = -100
} DYNAMIC_TIME ;

// according to EnumDSSTimeGranularityLevel
typedef enum  
{
	SECONDS	= 1,
	MINUTES	= 2,
	HOUR	= 3,
	DAY		= 4,
	MONTH	= 5,
	YEAR	= 6,
	WEEK	= 7
} TIME_GRANULARITY;

class DLL_DATATYPE_EXIM DynamicTime
{
public:

	DynamicTime(DYNAMIC_TIME inTime=NOT_DYNAMIC_TIME, Int32 inDynamicDayOffset=0);
	DynamicTime(const DynamicTime& irSrc);
	
	DynamicTime& operator = (const DynamicTime& irSrc);

	virtual ~DynamicTime() throw();
	
	void PutDynamicTime(DYNAMIC_TIME inDynamicTime);
	DYNAMIC_TIME GetDynamicTime() const;

	Int32 GetDynamicDayOffset() const;
	void PutDynamicDayOffset(Int32 inDayOffset);

	Int32 GetYear() const;
	Int32 GetMonth() const;
	Int32 GetDay() const;
	Int32 GetHour() const;
	Int32 GetMinute() const;
	Int32 GetSecond() const;
	Int32 GetWeek() const;
	Int32 GetDayOfWeek() const;

	void PutYear(Int32 inYear);
	void PutMonth(Int32 inMonth);
	void PutDay(Int32 inDay);
	void PutHour(Int32 inHour);
	void PutMinute(Int32 inMinute);
	void PutSecond(Int32 inSecond);
	void PutWeek(Int32 inWeek);
	void PutDayOfWeek(Int32 inDayofweek);

	// return 0 = success
	// return -1, it's TIME_END or TIME_BEGINNING, cannot convert to valid non-dynamic time.
	// orTime is UTC.
	Int32 ToDateTime(DateTime& orTime) const;

	static void MoveNthTimePointBefore(DateTime& iorTime, Int32 inCount, TIME_GRANULARITY inLevel);

	short GetDynamicMonthOffset() const;
	void PutDynamicMonthOffset(short inOffset);

private:

	DYNAMIC_TIME mnDynamicTime;

	Int32	mnDayOffset;

	Int32 mnYear;
	Int32 mnMonth;
	Int32 mnDay;
	Int32 mnHour;
	Int32 mnMinute;
	Int32 mnSecond;
	Int32 mnWeek;
	Int32 mnDayOfWeek;

	short mnMonthOffset;
};

} // namespace

#endif // _DYNAMICTIME_H_
