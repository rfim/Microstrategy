//
//  MSICalendarMonthMainViewUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/19/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface MSICalendarMonthMainViewUIHelper : UIHelper{
    UIFont*  monthNameLabelFont;
    UIColor*  monthNameLabelTextColor;
}
@property(nonatomic, readonly) UIFont* monthNameLabelFont;
@property(nonatomic, readonly) UIColor* monthNameLabelTextColor;
@end
