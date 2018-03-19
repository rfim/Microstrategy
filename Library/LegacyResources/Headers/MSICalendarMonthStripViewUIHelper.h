//
//  CalenderMonthStripViewUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/19/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface MSICalendarMonthStripViewUIHelper : UIHelper{
    int textFontSize;
    UIColor*  textColor;
}
@property(nonatomic, readonly) int textFontSize;
@property(nonatomic, readonly) UIColor* textColor;
@end
