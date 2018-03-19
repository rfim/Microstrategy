//
//  MSICalendarMainViewUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/18/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface MSICalendarMainViewUIHelper : UIHelper{
    CGGradientRef topGradient;
    CGGradientRef bottomGradient;
    UIFont*  curMonthNameLabelFont;
    UIColor*  curMonthNameLabelTextColor;
};
@property (nonatomic, readonly) CGGradientRef topGradient;
@property (nonatomic, readonly) CGGradientRef bottomGradient;
@property (nonatomic, readonly) UIFont* curMonthNameLabelFont;
@property (nonatomic, readonly) UIColor* curMonthNameLabelTextColor;
- (UIColor*)curMonthNameLabelTextColor;
- (UIImage *)navigationBarShadowImage;
-(UIColor*)tileTextColor;
@end
