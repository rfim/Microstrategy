//
//  PopoverLayoutMenu.h
//  MicroStrategyMobile
//
//  Created by Sun, Han (Jeremiah) on 12/4/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "PopoverMenu.h"

@interface PopoverLayoutMenu : PopoverMenu {
    BOOL fitMaxLabelWidth;
    UIColor* textColor;
    UIFont* textFont;
    UIColor* selectedTextColor;
    UIFont* selectedTextFont;
    BOOL enabled;
}

@property (nonatomic) NSInteger currentIdx;
@property (nonatomic) BOOL fitMaxLabelWidth;
@property (nonatomic, strong) UIColor* textColor;
@property (nonatomic, strong) UIFont* textFont;
@property (nonatomic, strong) UIColor* selectedTextColor;
@property (nonatomic, strong) UIFont* selectedTextFont;
@property (nonatomic) BOOL enabled;

@end
