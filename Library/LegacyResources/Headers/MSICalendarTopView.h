//
//  CalendarTopView.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define TOP_PADDING 6 
#define SIDE_PADDING 5
#define ARROW_WIDTH 50 
#define ARROW_HEIGHT 26 
#define MONTH_VIEW_SIDE_PADDING 20
#define DAY_VIEW_SIDE_PADDING 2

@class MSICalendarMonthView, MSICalendarGridView, MSICalendarArrowView;

@interface MSICalendarTopView : UIView {
	MSICalendarMonthView *monthView;
	MSICalendarGridView *gridView;
	MSICalendarArrowView *rightArrow;
	MSICalendarArrowView *leftArrow;
	
	NSInteger curMonth;
	NSInteger curYear;
}

@property(nonatomic, strong) MSICalendarGridView *gridView;

-(id)initWithFrame:(CGRect)frame initialDate:(NSDateComponents *)initDate;
-(void)changeMonth:(BOOL)isForward;
-(void)updateMonth:(NSInteger)newMonth year:(NSInteger)newYear;
-(void)updateMonthAndYear:(BOOL)isIncrement;
-(void)adjustLeftArrowVisibility:(BOOL)leftArrowVisibility rightArrowVisibility:(BOOL)rightArrowVisibility;
@end
