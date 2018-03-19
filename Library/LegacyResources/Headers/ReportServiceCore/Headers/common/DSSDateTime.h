/*
 *  DSSDateTime.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DATETIME_H
#define DATETIME_H

#ifndef __ANDROID__
    #include "DSSTimeSpan.h"
    #include "DataType/DataType/DateTime.h"
    #include "DataType/DataType/TimeSpan.h"
#else
    #include "DataType/DataType/DateTime.h"
    #include "DataType/DataType/TimeSpan.h"
#endif
namespace MDataType
{
#ifndef __ANDROID__
#ifndef DATETIME_CONST
#define DATETIME_CONST
	const long long  DAY_IN_MILLISECONDS = 24*60*60*1000;
	const long long  HOUR_IN_MILLISECONDS = 60*60*1000;
	const long long  MINUTE_IN_MILLISECONDS = 60*1000;
	const long long  SECOND_IN_MILLISECONDS = 1000;
	const long long  MILLISECOND_IN_NANOSECONDS = 1000000;
	
	const int	DAY_IN_HOURS = 24;
	const int	DAY_IN_MINUTES = 24*60;
	const int	DAY_IN_SECONDS = 24*60*60;
#endif

#ifndef DATETIME_MSISTRUCTURE
#define DATETIME_MSISTRUCTURE
	typedef struct MsiDateTime
	{
		int	DynamicTime;
		int	Year;
		int	Month;
		int	Day;
		int	Hour;
		int	Minutes;
		int	Seconds;
		int	MilliSeconds;
		
		int	Week;
		int	DayOfWeek;
		
		int		Mode;
	} MsiDateTime;	// for backward compatibility

    typedef struct ODBCDateTime
	{
		unsigned short	year;
		unsigned short	month;
		unsigned short	day;
		unsigned short	hour;
		unsigned short	minute;
		unsigned short	second;
		unsigned int	fraction;	// nanosecond
	} ODBCDateTime;
#endif
	//==============================================================================================
	//this is a partial implemention of MDataType::DSSDateTime for objective-C
	//==============================================================================================
#ifndef DATETIME_LITERAL
#define DATETIME_LITERAL
	typedef enum { SUN=0, MON=1, TUE=2, WED=3, THU=4, FRI=5, SAT=6 } DAY_OF_WEEK;
	typedef enum { JAN=1, FEB=2, MAR=3, APR=4,  MAY=5,  JUN=6,
		JUL=7, AUG=8, SEP=9, OCT=10, NOV=11, DEC=12 } MONTH_OF_YEAR;
#endif
    
#endif	
    

    
	class DSSDateTime
	{
	public:
		DSSDateTime(int inYear, int inMonth, int inDay, int inHour=0, int inMinute=0, int inSecond=0, int inMillisecond=0);
		// construct from ODBCDateTime
		DSSDateTime(const ODBCDateTime& iTime);
		// construct from VB time DATE
		// input must be in the range of -693593 (Year 0001) to 23242571 (Year 0xFFFF),
		// anything beyond will be truncated to the closest within range
		explicit DSSDateTime(double ifTime = 0.0);
		// copy constructor
		DSSDateTime(const DSSDateTime& irSrc);
		
		DSSDateTime& operator = (const DSSDateTime& irSrc);
		
		virtual ~DSSDateTime() throw();
		
		bool operator < (const DSSDateTime&) const;
		bool operator > (const DSSDateTime&) const;
		
		bool operator <= (const DSSDateTime&) const;
		bool operator >= (const DSSDateTime&) const;
		
		bool operator == (const DSSDateTime&) const;
		bool operator != (const DSSDateTime&) const;
		
		double ToDATE() const;
		double Totime_t() const;
		
		static bool IsLeapYear(int inYear);
		
		// Added at request of engine team in order to significantly improve
		// performance of xtab sorting
		// return -1 if a<b, return 0 if a==b, return 1 if a>b
		static int DateTimeOrder(const DSSDateTime& inTimeA, const DSSDateTime& inTimeB);
		
		// This method maps inYear to a year in [1970, 2038] with the same pattern
		// It can only handle inYear in range of [0, 9999], otherwise -1 will be returned.
		static int YearOfSamePattern(int inYear);
		
		// The Time Zone Bias is the number of minutes from GMT of the current locale. Positive
		// means one direction from GMT and negative means the other direction from GMT
		int GetTimeZoneBias() const;
		
		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		int GetHour() const;
		int GetMinute() const;
		int GetSecond() const;		
		int GetMillisecond() const;		
		int GetDayOfWeek() const;
		bool IsDateOnly() const;
        
        // 2/24/2012 tehe support financial date time
        int GetDaysInMonth() const;
        bool PutDay(int inDay);
        void AdjustDay();
        bool PutYear(int inYear);
        bool PutMonth(int inMonth);
        bool PutHour(int inHour);
        bool PutMinute(int inMinute);
        bool PutSecond(int inSecond);
        bool PutMillisecond(int inMillisecond);
        
#ifndef __ANDROID__
        DSSDateTime operator + (const DSSTimeSpan&) const;
        DSSDateTime operator - (const DSSTimeSpan&) const;
#else
        DSSDateTime operator + (const MDataType::TimeSpan&) const;
        DSSDateTime operator - (const MDataType::TimeSpan&) const;
#endif
        
        // jimzhang, Apr/23/2013, begin
        // give the current local time
		static DSSDateTime Now();
        // jimzhang, Apr/23/2013, end
        
	private:
		int	Sanitize(bool bAssert = true);
		int		mktime(struct tm& orTime) const;
		static void	DAYSToYMD(int inDays, unsigned short& ornYear, short& ornMonth, short& ornDay);
		static int	YMDToDAYS(unsigned short inYear, short inMonth, short inDay);
		static long long ToMilliseconds(const DSSDateTime& irTime);
		static double MillisecondsToDATE(long long inMilliseconds);
		void	SetWeekAndDayOfWeek() const;
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
	inline int DSSDateTime::GetYear() const
	{return mnYear;}
	inline int DSSDateTime::GetMonth() const
	{return mnMonth;}
	inline int DSSDateTime::GetDay() const
	{return mnDay;}
	inline int DSSDateTime::GetHour() const
	{return mnHour;}
	inline int DSSDateTime::GetMinute() const
	{return mnMinute;}
	inline int DSSDateTime::GetSecond() const
	{return mnSecond;}
	inline int DSSDateTime::GetMillisecond() const
	{return mnMillisecond;}
	inline bool DSSDateTime::IsDateOnly() const
	{
		return mnHour == 0 && mnMinute == 0 && mnSecond == 0 && mnMillisecond == 0;
	}
}

#endif
