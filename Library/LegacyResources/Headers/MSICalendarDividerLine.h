//
//  MSICalendarDividerLine.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 12/12/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Enums.h"
@interface MSICalendarDividerLine : UIView{
    CGGradientRef mGradient;
    ThemeType mThemeType;
}
- (id)initWithFrame:(CGRect)frame themeType:(ThemeType)themetype;
@end
