//
//  RssDateUtils.m
//  prog1
//
//  Created by Nikolay Sazonov on 5/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


@interface MSMRssDateUtils : NSObject {	
}
- (NSString *) getDifference: (NSString*) dateString;
- (NSString *) stringFromStringWithDate: (NSString *) date;
- (NSInteger) secondsFromTimeZoneString: (NSString *)tzString hasSemicolon: (BOOL) flag;
- (NSInteger) secondsFromTimeString: (NSString *) string minutesIndex: (short) minutesIndex;
- (NSDate *) parseDate: (NSString *) dateString;
- (NSString *) resultStringFromDate: (NSDate *) date;
+ (NSString *) getCurrentDate;
+ (NSString *) getLongStringDate;
+ (NSString *) getShortStringDateFromLongString:(NSString*)longStr;
+ (NSDate *) getDateFromLongString:(NSString *)str;
+ (BOOL) isShortStringDate:(NSString *)firstDate earlyThanDate:(NSString *)secondDate;
+ (BOOL) isDate:(NSDate *)firstDate earlyThanDate:(NSDate *)secondDate;

@end



