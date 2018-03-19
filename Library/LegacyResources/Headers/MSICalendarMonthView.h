//
//  MSICalendarMonthView.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define LETTER_TOP_PADDING 6

@interface MSICalendarMonthView : UILabel {
	NSInteger month;
	NSInteger year;
}

@property NSInteger month;
@property NSInteger year;

//-(void)changeMonth:(BOOL)isIncrement;
-(NSString *)getMonthString;
-(void)updateMonth:(NSInteger)newMonth year:(NSInteger)newYear;

@end
