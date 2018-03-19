//==============================================================================================
//	FILENAME	:	DateTime.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_DateTime_h
#define MDataType_DateTime_h

#include "DataType.h"

#include "TimeSpan.h"
#include "Base/Base/ReturnString.h"

namespace MDataType
{
#ifndef DATETIME_MSISTRUCTURE
#define DATETIME_MSISTRUCTURE
	typedef struct MsiDateTime
	{
		Int32	DynamicTime;
		Int32	Year;
		Int32	Month;
		Int32	Day;
		Int32	Hour;
		Int32	Minutes;
		Int32	Seconds;
		Int32	MilliSeconds;

		Int32	Week;
		Int32	DayOfWeek;

		Int32		Mode;
	} MsiDateTime;	// for backward compatibility

	typedef struct ODBCDateTime
	{
		unsigned short	year;
		unsigned short	month;
		unsigned short	day;
		unsigned short	hour;
		unsigned short	minute;
		unsigned short	second;
		unsigned Int32	fraction;	// nanosecond
	} ODBCDateTime;
#endif
	//==============================================================================================
	// DateTime is a raw datetime class. It represents a datetime as a normal break down structure,
	// What it really stands for is up to the client's interpretation.
	// It can do conversion to/from VB DATE, ODBC datetime structure, and conversion between UTC and localtime
	//
	// The constructor is lenient to invalid input, any error will be corrected silently by the following rule
	// any year less than 0 is set to 0;
	// any month less than JAN is set to JAN; larger than DEC is set to DEC
	// any day less than 1 is set to 1; larger than the last day of the month is set to the last day
	// any minute less than 0 is set to 0; larger than 59 is set to 59
	// any second less than 0 is set to 0; larger than 59 is set to 59
	// any millisecond less than 0 is set to 0; larger than 999 is set to 999
	// The user is responsible for passing in valid data to construct a DateTime.
	//
	// PutXXX method checks input by the same rule. Any input fall out of the valid range is rejected,
	// which leaves the object intact.
	// The only exception is PutDay, as long as the input day is in the range of [1, 31],
	// even though it might be larger than the last day of the month, it's allowed to proceed.
	//
	// It's size is IMMUTABLE - you can not make a bug fix which creates new member!!!
	//==============================================================================================
#ifndef DATETIME_LITERAL
#define DATETIME_LITERAL
	typedef enum { SUN=0, MON=1, TUE=2, WED=3, THU=4, FRI=5, SAT=6 } DAY_OF_WEEK;
	typedef enum { JAN=1, FEB=2, MAR=3, APR=4,  MAY=5,  JUN=6,
				   JUL=7, AUG=8, SEP=9, OCT=10, NOV=11, DEC=12 } MONTH_OF_YEAR;
#endif
	class DLL_DATATYPE_EXIM DateTime
	{
	public:

		DateTime(Int32 inYear, Int32 inMonth, Int32 inDay, Int32 inHour=0, Int32 inMinute=0, Int32 inSecond=0, Int32 inMillisecond=0);
		// construct from ODBCDateTime
		DateTime(const ODBCDateTime& iTime);
		// construct from VB time DATE
		// input must be in the range of -693593 (Year 0001) to 23242571 (Year 0xFFFF),
		// anything beyond will be truncated to the closest within range
		explicit DateTime(double ifTime = 0.0);
		// copy constructor
		DateTime(const DateTime& irSrc);

		DateTime& operator = (const DateTime& irSrc);

		~DateTime() throw();

		Int32 GetYear() const;
		Int32 GetMonth() const;
		Int32 GetDay() const;
		Int32 GetHour() const;
		Int32 GetMinute() const;
		Int32 GetSecond() const;
		Int32 GetMillisecond() const;
		Int32 GetWeek() const;
		Int32 GetDayOfWeek() const;
		// 1 - 366
		Int32 GetDayOfYear() const;

		// return totals day in current month
		Int32 GetDaysInMonth() const;

		// return the total number of weeks in current month
		// int GetWeeksOfMonth() const;

		// return false if inYear < 0
		bool PutYear(Int32 inYear);

		// return false if inMonth < JAN || inMonth > DEC
		bool PutMonth(Int32 inMonth);

		// return false if inDay !([-31, -1] || [1, 31])
		// negative inDay implies count the days from the end of the month
		bool PutDay(Int32 inDay);

		// return false if inHour < 0 || inHour > 23
		bool PutHour(Int32 inHour);

		// return false if inMinute < 0 || inMinute > 59
		bool PutMinute(Int32 inMinute);

		// return false if inSecond < 0 || inSecond > 59
		bool PutSecond(Int32 inSecond);

		// return false if inMillisecond < 0 || inMillisecond > 999
		bool PutMillisecond(Int32 inMillisecond);

		// return false if inWeek < 1 || inWeek > 6
		bool PutWeek(Int32 inWeek);

		// return false if inDayofweek < SUN || inDayofweek > SAT
		bool PutDayOfWeek(Int32 inDayofweek, bool ibChangeMonth=false);

		bool PutWeekAndDayOfWeek(Int32 inWeek, Int32 inDayofWeek);

		// YYYY-MM-DD hh:mm:ss.msec, according to ISO 8601
		MBase::ReturnString ToString() const;

		// parse a string in ISO 8601 format (YYYY-MM-DD hh:mm:ss.msec)
		// and return the number of parsed characters
		unsigned int FromString(const char* ipISO8601String);

		DateTime operator + (const TimeSpan&) const;
		DateTime operator - (const TimeSpan&) const;
		TimeSpan operator - (const DateTime&) const;

		bool operator < (const DateTime&) const;
		bool operator > (const DateTime&) const;

		bool operator <= (const DateTime&) const;
		bool operator >= (const DateTime&) const;

		bool operator == (const DateTime&) const;
		bool operator != (const DateTime&) const;

		double ToDATE() const;

		ODBCDateTime ToODBCDateTime() const;

		// see it as a localtime and convert to UTC according to local time zone
		DateTime ToUTCTime() const;
		// see it as a UTC time and convert to localtime according to local time zone
		DateTime ToLocalTime() const;

		// give the current local time
		static DateTime Now();

		// give the current UTC time
		static DateTime NowUTC();

		// 12/05/2003 yuwen give the time zone information
		static bool GetTimeZoneInfo(Int32& inOffsetInMinutes);

		bool IsLeapYear() const;

		static bool IsLeapYear(Int32 inYear);

		// Added at request of engine team in order to significantly improve
		// performance of xtab sorting
		// return -1 if a<b, return 0 if a==b, return 1 if a>b
		static Int32 DateTimeOrder(const DateTime& inTimeA, const DateTime& inTimeB);

		// This method maps inYear to a year in [1970, 2038] with the same pattern
		// It can only handle inYear in range of [0, 9999], otherwise -1 will be returned.
		static int YearOfSamePattern(int inYear);

		// for backward compatibility
		static unsigned Int32 GetSaveObjSize();
		// caller is responsible to allocate memory for ipBuffer
		Int32 SaveObj(unsigned Int32 inSize, void* ipBuffer, Int32 mode = 0 /* LOCAL */) const;
		Int32 LoadObj(unsigned int lnSize, void* ipBuffer,bool bAssert = true);

		// input represents the seconds elapsed since midnight (00:00:00), January 1, 1970
		explicit DateTime(time_t inSecond, Int32 inMillisecond=0);
		time_t Totime_t() const;

		// The Time Zone Bias is the number of minutes from GMT of the current locale. Positive
		// means one direction from GMT and negative means the other direction from GMT
		// comment out because link error of timezone on Android
		//Int32 GetTimeZoneBias() const;

		// lpan 2010-04-28: TQMS 388539. According to Tao He's TQMS comment, NCS use format MM/DD/YYYY
		MBase::ReturnString ToDateString() const;
		
		Int64 Totime_t64() const;
		
	private:
		Int32	Sanitize(bool bAssert = true);
		void	SetWeekAndDayOfWeek() const;
		void	AdjustDay();
		Int32		mktime(struct tm& orTime) const;
		static void	DAYSToYMD(Int32 inDays, unsigned short& ornYear, short& ornMonth, short& ornDay);
		static int	YMDToDAYS(unsigned short inYear, short inMonth, short inDay);
		static Int64 ToMilliseconds(const DateTime& irTime);
		static double MillisecondsToDATE(Int64 inMilliseconds);
		static int WeekFromDayAndDayOfWeek(int inDay, int inDayOfWeek);


	private:
		unsigned short mnYear;
		short mnMonth;
		short mnDay;
		short mnHour;
		short mnMinute;
		short mnSecond;
		short mnMillisecond;
		mutable short mnWeek;
		mutable short mnDayOfWeek;
	};
}

#endif // MDataType_DateTime_h
