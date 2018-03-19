//
//  CalendarMonthMainView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Enums.h"
#import "UIHelper.h"
#define MONTH_NAME_SIDE_PADDING 10

@class MSICalendarMainView, MSICalendarMonthGridView;
@class MSICalendarMonthMainViewUIHelper;

@interface MSICalendarMonthMainView : UITableViewCell {
	@private
	MSICalendarMainView *__weak parentView;
	NSInteger month;
	NSInteger year;
	UILabel *monthNameLabel;
	MSICalendarMonthGridView *gridView;
	NSInteger cellHeight;
    MSICalendarMonthMainViewUIHelper* calendarMonthMainViewUIHelper;
}

@property(nonatomic, weak) MSICalendarMainView *parentView;
@property NSInteger year;
@property NSInteger month;
@property (nonatomic) MSICalendarMonthGridView *gridView;
@property (readonly) NSInteger cellHeight;
@property (readonly) ThemeType themeType;
-(id)initWithFrame:(CGRect)frame month:(NSInteger)curMonth year:(NSInteger)curYear parent:(MSICalendarMainView *)parent uihelper:(UIHelper*)uiHelper;
-(id)initWithFrame:(CGRect)frame month:(NSInteger)curMonth year:(NSInteger)curYear parent:(MSICalendarMainView *)parent;
-(void)updateOnOrientationChange;

@end
