//
//  CalendarElement.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/12/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIElement;

@interface MSICalendarElement : NSObject {
	NSInteger day;
	NSInteger month;
	NSInteger year;
}

-(id)initWithDateString:(NSString *)date;
-(id)initWithDate:(NSDate *)date;
-(void)parseElementValue:(NSString *)value;
-(NSDate *)getDate;
-(NSString *)getElementName;
-(NSComparisonResult)compareToElement:(MSICalendarElement *)element;

@property NSInteger day;
@property NSInteger month;
@property NSInteger year;

@end
