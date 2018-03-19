//
//  CalendarMonthStripView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIHelper.h"
@class MSICalendarMonthStripViewUIHelper;

#define SIDE_PADDING 0
#define TOP_PADDING 5

@interface MSICalendarMonthStripView : UIView {
    MSICalendarMonthStripViewUIHelper* stripViewUIHelper;
}
- (id)initWithFrame:(CGRect)frame uihelper:(UIHelper*)uiHelper;
@end
