//
//  MSICalendarViewControllerUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/17/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "MIPromptDetailsViewControllerUIHelper.h"

@interface MSICalendarViewControllerUIHelper : MIPromptDetailsViewControllerUIHelper{
    CGRect yearStripViewFrame;
    CGRect calendarViewFrame;
    UIColor*  backgroundColor;

}
@property(nonatomic,readonly) CGRect yearStripViewFrame;
@property(nonatomic,readonly) CGRect calendarViewFrame;
@property(nonatomic,readonly) UIColor* backgroundColor;
@property(nonatomic,readonly) UIColor* selectedTableViewTextColor;
-(UIColor*) selectedTableViewTextColor;
-(UIColor*) tableSeperateLineColor;
-(UIFont*)selectedTableViewTextFont;
-(CGRect)dividerLineFrame;
-(id)initWithThemeType:(ThemeType)_themetype;
-(int)timeButtonHeight;
-(UIColor*)timeButtonBackgroundColor;
-(UIFont*)timeButtonTitleFont;
-(UIColor*)timeButtonTitleColor;
-(UIColor*)getTintColor;
@end
