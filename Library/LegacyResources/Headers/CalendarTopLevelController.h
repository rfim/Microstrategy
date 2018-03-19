//
//  CalendarTopLevelController.h
//  RingWidget
//
//  Created by Firat Alpergin on 2/1/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MultiSelectTableViewController.h"
#import "CalendarViewController.h"

#define EXTRA_SPACING 6

@class CalendarView, PromptHelper, PromptCalendarCell, CalendarElement, MultiDeleteButton, CalendarViewController;

/*@protocol CalendarCallback

-(void)dateSelectionDone:(NSString *)newSelection;
-(void)dateSelectionCancelled;

@end*/


@interface CalendarTopLevelController : MultiSelectTableViewController {
	CalendarView *calendarView;
	UITableView *dateListView;
	NSInteger curSelectedScreenIndex;
	UIBarButtonItem *editButton;
	UIBarButtonItem *doneButton;
	UIBarButtonItem *cancelButton;
	BOOL changesMade;
	UISegmentedControl *segmentedControl;
	
	PromptHelper* promptHelper;
	BOOL multiSelect;
	BOOL searchRequired;
	NSMutableArray* selectedElements;
	
	id<CalendarDelegate>callBack; //for date time constant prompts
	CalendarElement *minDateElement;
	CalendarElement *maxDateElement;
	
	NSMutableArray *defaultModeToolbarItems;
	NSMutableArray *editModeToolbarItems;
	MultiDeleteButton *deleteButton;
	UIButton *selectAllButton;
}

@property (nonatomic, assign) PromptHelper* promptHelper;
@property BOOL multiSelect;
@property BOOL searchRequired;
@property (nonatomic, retain) NSMutableArray* selectedElements;
@property (nonatomic, assign) id<CalendarDelegate>callBack;
@property (nonatomic, retain) NSMutableArray *defaultModeToolbarItems;
@property (nonatomic, retain) NSMutableArray *editModeToolbarItems;

- (id)initWithSelectedElements:(NSArray *)elements minDate:(NSString *)minDate maxDate:(NSString *)maxDate isMultiSelect:(BOOL)mulSelect promptHelper:(PromptHelper *)helper;
- (id)initWithDate:(NSString *)date minDate:(NSString *)minDate maxDate:(NSString *)maxDate;
- (void)updateScreen:(NSInteger)index;
- (void)setupToolbar;
- (void)goToToday;
- (void)goToMonth:(NSInteger)month year:(NSInteger)year;
- (void)createCalendarElementList:(NSArray *)elementList;
- (void)createElementListFromCalendarElements;
- (void)setupSelf;
- (CalendarElement *)newCalendarElementFromDate:(NSString *)date;
- (void)updateToolbar;
- (void)updateDeleteButtonLabel:(NSInteger)newSelectionCnt;
- (void)updateSelectAllButtonLabel:(NSInteger)newSelectionCnt;
- (void)setSelectedArrayTo:(BOOL)boolValue;

@end
