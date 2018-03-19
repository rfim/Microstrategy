//
//  CalendarViewController.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 3/31/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSICalendarMainView.h"
#import "MSIYearStripView.h"
#import "MIPromptDetailsViewController.h"
#import "TimeViewControllerDelegate.h"
#import "MSICalendarViewControllerUIHelper.h"

#define CALENDAR_VIEW_TOP_PADDING (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? 30 : 20)
#define CALENDAR_VIEW_TOP_PADDING_COMPACT 5
#define CALENDAR_VIEW_MIDDLE_PADDING 10
#define CALENDAR_VIEW_MIDDLE_PADDING_COMPACT 0
#define CALENDAR_YEAR_VIEW_LEFT_PADDING 0
#define CALENDAR_YEAR_VIEW_LENGTH (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? (UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation]) ? 440 : 440) : 320)
#define CALENDAR_YEAR_VIEW_PORTRAIT_LENGTH 454
#define CALENDAR_YEAR_VIEW_HEIGHT (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? 60 : 50)

#define CALENDAR_VIEW_LEFT_PADDING (([MSIGeneric getDeviceType] == DeviceTypeIPad && UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation])) ? 84 : 3)

#define CALENDAR_PORTRAIT_OFFSET 23
#define CALENDAR_HEIGHT_PORTRAIT_OFFSET 220

#define CALENDAR_TIME_PICKER_POSITION_X (UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation]) ? 516 : 138)
#define CALENDAR_TIME_PICKER_POSITION_Y (UIInterfaceOrientationIsLandscape([MSIGeneric applicationInterfaceOrientation]) ? 225 : 738)
#define CALENDAR_TIME_PICKER_WIDTH 200
#define CALENDAR_TIME_PICKER_HEIGHT 216
#define CALENDAR_TIME_PICKER_HEIGHT_IN_CALENDAR_VIEW (([MSIGeneric getDeviceType] == DeviceTypeIPad && UIInterfaceOrientationIsPortrait([MSIGeneric applicationInterfaceOrientation])) ? CALENDAR_TIME_PICKER_HEIGHT : 0)

#define CALENDAR_TIME_BUTTON_HEIGHT 30

@class MSICalendarMainView;

@class PromptHelper;
@class MSICalendarElement;
@class BucketHeader;
@class MIFilter;
@class BucketElementCountImageView;
@class MSICalendarViewControllerUIHelper;
@protocol CalendarDelegate

-(void)dateSelectionDone:(NSDate *)newSelection;
-(void)dateSelectionCancelled;

@end

@interface MSICalendarViewController : MIPromptDetailsViewController <MSICalendarViewDelegate, MSIYearStripViewDelegate, UITableViewDataSource, UITableViewDelegate> {
@private
    BOOL isPopoverShown;
	BOOL isViewFirstAppear;
	MSICalendarMainView *calendarView;
	
	PromptHelper *__weak promptHelper;
	id<CalendarDelegate>__weak callBack; //for date time constant prompts
	MSIYearStripView *yearStripView;
	
	MIFilter *__weak filter;
	NSMutableArray *availableElements;
	

	MSICalendarElement *minDateElement;
	MSICalendarElement *maxDateElement;
	BucketHeader* dummyHeader;
	
	UIViewController<TimeViewController> *timeViewController;
	UIButton *timeButton;
	BOOL isDisplayTimeAllowDismiss;
    
    MSICalendarViewControllerUIHelper* calendarviewcontrolleruihelper;
    
    
    //   UISegmentedControl *mSearchControl;
    UIToolbar* toolBar;
    
    //    UIView* baseView; /// searchControl to control the view change in this view
    
    NSMutableArray* sortedSelectedDates;
    NSMutableArray* selectedElementsToClear;

}


@property(nonatomic, weak) id<CalendarDelegate>callBack;
@property(nonatomic, weak) PromptHelper *promptHelper;
@property(nonatomic, weak) MIFilter *filter;
@property(nonatomic, strong) UIViewController<TimeViewController> *timeViewController;

-(void)setSelectedElements:(NSArray *)elements;
-(void)setSingleDate:(NSDate *)singleDate;  //for constant prompts
-(void)setMinDate:(NSString *)minDate;
-(void)setMaxDate:(NSString *)maxDate;
-(void)display;


@end

