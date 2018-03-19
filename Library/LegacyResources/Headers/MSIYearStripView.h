//
//  YearStripView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 3/31/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

#define NUMBER_OF_YEARS (([MSIGeneric getDeviceType] == DeviceTypeIPad && UIInterfaceOrientationIsPortrait([MSIGeneric applicationInterfaceOrientation])) ? 3 : 5)
#define NUMBER_OF_MAX_YEARS 99

@class MSICalendarViewController, MSICalendarMainView, RoundedRectView;
@class SingleYearView;
@class YearStripViewUIHelper;
@protocol MSIYearStripViewDelegate

-(void)yearChangedByStripView:(NSInteger)newYear;

@end

@interface MSIYearStripView : UIView <UIScrollViewDelegate> {
	@private
	NSInteger curSelectedYear;
	CGFloat subviewWidth;
	NSMutableArray *subviewList;
	UIScrollView *yearScrollView;
	NSInteger numberOfTotalYears;
	id<MSIYearStripViewDelegate> __weak delegate;
	RoundedRectView *leftGradientView;
	RoundedRectView *rightGradientView;
	BOOL wasYearChangedByStripView;
	
	NSInteger minYear;
	NSInteger maxYear;
	SingleYearView *minYearView;
	SingleYearView *maxYearView;
    YearStripViewUIHelper* yearStripViewUIHelper;
}

@property(nonatomic, weak) id<MSIYearStripViewDelegate> delegate;

-(id)initWithFrame:(CGRect)frame currentYear:(NSInteger)year minYear:(NSInteger)min maxYear:(NSInteger)max;
-(BOOL)needUpdateYearFromYear:(NSInteger)year;
-(void)updateYear:(NSInteger)year withFeedback:(BOOL)feedback;
-(void)updateOnOrientationChange;
-(void)calendarDidUpdateMonthScrollingAnimation;
-(id)initWithFrame:(CGRect)frame currentYear:(NSInteger)year minYear:(NSInteger)min maxYear:(NSInteger)max uiHelper:(YearStripViewUIHelper*)uihelper;
@end
