//
//  MSICalendarTileViewUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/18/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface MSICalendarTileViewUIHelper : UIHelper{
    UIColor*  textColor;
    UIFont*  textFont;
    UIColor*  fillColor;
}
@property(nonatomic, readonly) UIColor* textColor;
@property(nonatomic, readonly) UIFont* textFont;
@property(nonatomic, readonly) UIColor* fillColor;
@end
