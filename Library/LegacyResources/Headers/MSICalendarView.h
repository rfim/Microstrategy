//
//  CalendarView.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define CALENDAR_TOP_HEIGHT 50
#define CALENDAR_SEPARATOR_HEIGHT 2

#define CALENDAR_ORIGIN_X 110
#define CALENDAR_ORIGIN_Y 70

@class MSICalendarTopView, MSICalendarGridView, MSICalendarSeparatorView, PromptHelper, MSICalendarElement;

@interface MSICalendarView : UIView {
	MSICalendarTopView *topView;
	MSICalendarGridView *gridView;
	MSICalendarSeparatorView *separatorView;
	NSMutableArray *__weak selectedElements;
	UIView *emptyView;
	PromptHelper *__weak promptHelper;
	NSDateComponents *initialDate;
	@private
	CGFloat calendarOriginX;
	CGFloat calendarOriginY;
}

@property (nonatomic, weak) PromptHelper *promptHelper;
@property (nonatomic, weak) NSMutableArray *selectedElements;
@property (nonatomic, strong) NSDateComponents *initialDate;

-(id)initWithSelectedDays:(NSMutableArray *)days minDate:(MSICalendarElement *)minDate maxDate:(MSICalendarElement *)maxDate multiSelect:(BOOL)multiSelect promptHelper:(PromptHelper *)helper;
-(id)initWithSelectedDays:(NSMutableArray *)days minDate:(MSICalendarElement *)minDate maxDate:(MSICalendarElement *)maxDate;
-(NSDateComponents *)getInitialDate:(NSArray *)selectedDays minDate:(MSICalendarElement *)minDate maxDate:(MSICalendarElement *)maxDate;
-(void)gridHeightUpdated:(NSInteger)newHeight;
-(void)updateCalendar;
-(void)goToMonth:(NSInteger)month year:(NSInteger)year;
-(void)setupSelf;
-(void)adjustArrowVisibility;
-(void)addDate:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;

@end
