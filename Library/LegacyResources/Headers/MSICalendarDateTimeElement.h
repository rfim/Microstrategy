//
//  CalendarDateTimeElement.h
//  MicroStrategyMobile
//
//  Created by Yonggang Xi on 4/12/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSICalendarElement.h"

@interface MSICalendarDateTimeElement : MSICalendarElement {
	NSInteger hour;
	NSInteger minute;
	NSInteger second;
}

@property NSInteger hour;
@property NSInteger minute;
@property NSInteger second;

@end
