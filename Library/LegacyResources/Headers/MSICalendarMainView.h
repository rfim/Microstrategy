//
//  CalendarView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSISelectorPinView.h"
#import "TimeViewControllerDelegate.h"

#define CURRENT_MONTH_LABEL_HEIGHT (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? 0 : 34)
#define MONTH_STRIP_PADDING (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? (UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation]) ? 123 : 93) : 0)
#define MONTH_STRIP_WIDTH 308
#define MONTH_STRIP_HEIGHT (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? 28 : 23)
#define CALENDAR_ROW_HEIGHT 44
#define CALENDAR_WIDTH (MONTH_STRIP_PADDING + MONTH_STRIP_WIDTH)
#define CALENDAR_HEIGHT (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? (UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation]) ? 660 : 916) : 381)
#define CALENDAR_INBETWEEN_MONTH_PADDING 10
#define PIN_OFFSET 22
#define NUMBER_OF_YEARS_LOADED 3

@class MSICalendarMonthStripView, MSICalendarViewController, MSIYearStripView, SelectorPinView, MSICalendarTileView, MSICalendarElement;
@class MSICalendarMonthMainView;
@class RoundedRectView;
@class MSICalendarMainViewUIHelper;
@protocol MSICalendarViewDelegate

-(void)selectionsMade:(NSMutableArray *)dateList;
-(void)updateSelectedTime:(NSDate *)selectedTime;
-(BOOL)isDateSelected:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(BOOL)isDateAvailable:(NSInteger)day month:(NSInteger)month year:(NSInteger)year;
-(BOOL)multiSelectSupported;
-(void)yearChangedByCalendarView:(NSInteger)newYear fromYear:(NSInteger)curYear updateCalendar:(BOOL)update waitUntilDone:(BOOL)wait;
-(void)didUpdateMonthScrollingAnimation;
-(void)timepickerPopoverDidDisappear;
@end

@interface MSICalendarMainView : UIView <SelectorPinDelegate, UITableViewDelegate, UITableViewDataSource, TimeViewControllerDelegate> {
	@private
	UILabel *curMonthNameLabel; // for iPhone display current month name
	MSICalendarMonthStripView *monthStripView;
	UITableView *monthTableView;
	// NSInteger mYear;
	// NSInteger mMonth;
	// NSInteger mDay;
	NSMutableArray *cellList;
	id<MSICalendarViewDelegate> __weak currentCallback;
	NSMutableArray *selectedDays;
	NSMutableArray *selectedTiles;
	NSInteger yearToUpdate;
	NSInteger curDisplayedYear;  // for the cell which is displaying, will change when scrolling
	NSInteger curDisplayedMonth; // for the cell which is displaying or scrolled if stop scrolling, will change when scrolling
	NSInteger curScrolledIndex;   // for the cell which is scrolled, will NOT change when scrolling
	BOOL isScrolling;
	BOOL isScrollingToSelected;
	
	BOOL inSelectionMode;
	MSISelectorPinView *leftPin;
	MSISelectorPinView *rightPin;
	MSISelectorPinView *__weak currentlyTouchedPin;
	MSICalendarTileView *__weak currentLeftPinTile;
	MSICalendarTileView *__weak currentRightPinTile;
	MSICalendarTileView *lastTouchedTile;
	MSICalendarTileView *lastReleasedTile;
	BOOL firstMoveAfterTouchesBegan;
	NSMutableArray *currentHlitedTiles;
	BOOL shouldClearhHlites;
	BOOL touchesState;
	
	RoundedRectView *topGradientView;
	RoundedRectView *bottomGradientView;
	
	MSICalendarElement *minDateToUpdate;
	MSICalendarElement *maxDateToUpdate;
	MSICalendarElement *minDate;
	MSICalendarElement *maxDate;
	
	MSICalendarMonthMainView *__weak lastUpdatedCell;
	NSInteger lastDisplayedMonth; // for iPhone, lastDisplayedMonth showed the white color
	
	UIViewController<TimeViewController> *__weak timeViewController;
	BOOL isDisplayTimeAllowDismiss;
    
    @private
    MSICalendarMainViewUIHelper* calendarMainViewUIHelper;
 
    UIImageView* upDividerLine;
}

@property(nonatomic, weak) id<MSICalendarViewDelegate> currentCallback;
@property BOOL inSelectionMode;
@property NSInteger yearToUpdate;
@property(nonatomic, strong) NSMutableArray *selectedDays;
@property(nonatomic, strong) NSMutableArray *selectedTiles;
@property(nonatomic, strong) MSISelectorPinView *leftPin;
@property(nonatomic, strong) MSISelectorPinView *rightPin;
@property(nonatomic, weak) MSISelectorPinView *currentlyTouchedPin;
@property(nonatomic, weak) MSICalendarTileView *currentLeftPinTile;
@property(nonatomic, weak) MSICalendarTileView *currentRightPinTile;
@property(nonatomic, strong) MSICalendarElement *minDateToUpdate;
@property(nonatomic, strong) MSICalendarElement *maxDateToUpdate;
@property(nonatomic, strong) MSICalendarElement *minDate;
@property(nonatomic, strong) MSICalendarElement *maxDate;
@property(nonatomic, weak) MSICalendarMonthMainView *lastUpdatedCell;
@property(nonatomic, weak) UIViewController<TimeViewController> *timeViewController;

-(id)initWithFrame:(CGRect)frame year:(NSInteger)curYear month:(NSInteger)curMonth minDate:(MSICalendarElement *)min maxDate:(MSICalendarElement *)max callback:(id<MSICalendarViewDelegate>)callback;
-(void)updateYear;
-(void)updateYearAndScroll:(BOOL)scroll;
-(void)updateSelections;
-(void)clearAllSelected;
-(void)scrollAfterLoad;
-(void)scrollToSelected;
-(void)updateOnOrientationChange;
//-(void)displayTimeViewAndAllowDismiss;
-(id)initWithFrame:(CGRect)frame year:(NSInteger)curYear month:(NSInteger)curMonth minDate:(MSICalendarElement *)min maxDate:(MSICalendarElement *)max callback:(id<MSICalendarViewDelegate>)callback uiHelper:localuihelper;
@end
