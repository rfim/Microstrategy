//
//  CalendarGridView.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/27/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#import <QuartzCore/QuartzCore.h>
#import "MSIServiceImpl.h"

#define TILE_HEIGHT 45
#define DEFAULT_TILE_WIDTH 45
#define LEFT_SIDE_TILE_WIDTH 47
#define RIGHT_SIDE_TILE_WIDTH 48

#define CALENDAR_ORIGIN_X 110
#define CALENDAR_ORIGIN_Y 70

#define UP_DIRECTION 0
#define DOWN_DIRECTION 1

@class MSICalendarTile, MSICalendarTopView, MSICalendarView, MSICalendarElement;

@interface MSICalendarGridView : UIView {
	NSMutableArray *__weak selectedDays;
	NSMutableArray *tiles;
	NSCalendar *cal;
	
	NSInteger curMonth;
	NSInteger curYear;
	NSInteger curRowCnt;
	
	NSInteger topHeightOffset;
	
	BOOL isInSwipeEvent;
	MSICalendarTile *lastTouchedTile;
	NSMutableArray *swipeSelections;
	BOOL multiSelect;
	
	MSICalendarTopView *__weak headerView;
	MSICalendarView *__weak parentView;
	
	BOOL firstMoveAfterTouchesBegan;
	
	MSICalendarElement *minDate;
	MSICalendarElement *maxDate;
	
	UIView *baseView;
	UIView *oldBaseView;
	BOOL isFirstChange;
	NSInteger curChangeDirection;
	NSInteger numberOfRows;
	
	BOOL isFirstDisplay;
	BOOL isCurrentChangeRequestValid;
	NSInteger animationDirection;
	
	BOOL hasMonthChangedSinceTouchBegan;
	CGFloat calendarOriginX;
	CGFloat calendarOriginY;
}

@property (nonatomic, weak) MSICalendarTopView *headerView;
@property (nonatomic, weak) MSICalendarView *parentView;
@property (nonatomic, weak) NSMutableArray *selectedDays;
@property NSInteger curMonth;
@property NSInteger curYear;
@property BOOL multiSelect;
@property (nonatomic, strong) MSICalendarElement *minDate;
@property (nonatomic, strong) MSICalendarElement *maxDate;
@property (nonatomic, strong) UIView *baseView;
@property (nonatomic, strong) UIView *oldBaseView;
@property BOOL isCurrentChangeRequestValid;
@property NSInteger animationDirection;

-(id)initWithMonth:(NSInteger)month year:(NSInteger)year frame:(CGRect)frame selections:(NSMutableArray *)selDays heightOffset:(NSInteger)offset  parent:(MSICalendarView *)parent;
-(void)changeMonth:(NSInteger)newMonth year:(NSInteger)newYear;
-(void)updateMonth:(NSInteger)newMonth year:(NSInteger)newYear;
-(NSRange)getDaysOfMonth:(NSInteger)month year:(NSInteger)year;
-(NSInteger)getNumberOfRows:(NSInteger)curMonthDayCount startingWeekday:(NSInteger)weekday;
-(BOOL)isSelected:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(BOOL)isAvailable:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(void)addToSelected:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(void)removeFromSelected:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(void)updateSelections;
-(NSString *)getMonthName:(NSInteger)month;
-(void)removeAllSelected;
-(void)animateUp;
-(void)animateDown;
-(void)updateBaseView:(NSInteger)direction;
-(void) adjustArrowVisibility;
@end
