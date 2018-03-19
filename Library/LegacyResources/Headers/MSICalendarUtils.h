//
//  MSICalendarUtils.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface MSICalendarUtils : NSObject {
	@protected
	NSCalendar *calendar;
}

+(NSString *)getDateString:(NSDate *)currentDate;
+(NSString *)getTimeString:(NSDate *)currentDate;
+(NSString *)getDateTimeString:(NSDate *)currentDate;
+(NSString *)getMonthName:(NSInteger)month;	
+(NSString *)getShortMonthName:(NSInteger)month;
+(NSString *)getShortDayName:(NSInteger)dayNumber;
+(NSString *)getDayName:(NSInteger)dayNumber;
+(NSInteger)getNumberOfRowsForMonth:(NSInteger)month year:(NSInteger)year;
+(NSRange)getDaysOfMonth:(NSInteger)month year:(NSInteger)year;
+(NSInteger)getFirstDayOfMonth:(NSInteger)month year:(NSInteger)year;
+(NSInteger)getLastDayOfMonth:(NSInteger)month year:(NSInteger)year;
+(BOOL)isDayGreater:(NSInteger)day month:(NSInteger)month year:(NSInteger)year aDay:(NSInteger)aDay aMonth:(NSInteger)aMonth aYear:(NSInteger)aYear;
+(BOOL)isMonthGreater:(NSInteger)month year:(NSInteger)year aMonth:(NSInteger)aMonth aYear:(NSInteger)aYear;

// supplement by wenzhou, 03/04/2015
+(NSInteger) getYear:(NSDate*) date;
+(NSInteger) getMonth:(NSDate*) date;
+(NSInteger) getDay:(NSDate*) date;
+(NSInteger) getHour:(NSDate*) date;
+(NSInteger) getMinite:(NSDate*) date;
+(NSInteger) getSecond:(NSDate*) date;
+(NSInteger) getWeekday:(NSDate*) date;
+(NSString*) getWeekdayShortNameByDate:(NSDate*)date;
+(NSString*) getWeekdayNameByDate:(NSDate*) date;
+(NSString*) getMonthShortNameByDate:(NSDate*) date;
+(NSString*) getMonthNameByDate:(NSDate*) date;

+(NSDate*) getDateByString:(NSString*) iString;
+(NSInteger) getFirstWeekday; // return [1~7], 1:Sunday.
+(void) setFirstWeekday:(NSInteger) dayNumber; // dayNumber should be [1~7].

+(NSInteger) getLastWeekday; // return [1~7], 1:Sunday

@end
