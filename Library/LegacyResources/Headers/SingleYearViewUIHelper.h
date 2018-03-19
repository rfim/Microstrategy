//
//  SingleYearViewUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/18/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface SingleYearViewUIHelper : UIHelper{
    UIColor*  yearLabelTextColor;
    UIColor*  yearLabelSelectedColor;
    UIColor*  yearLabelNotAvailableColor;
    int yearLabelFontSize;
    int yearLabelSelctedFontSize;
}
@property(nonatomic, readonly) UIColor* yearLabelTextColor;
@property(nonatomic, readonly) UIColor* yearLabelSelectedColor;
@property(nonatomic, readonly)  UIColor* yearLabelNotAvailableColor;
@property(nonatomic, readonly) int yearLabelFontSize;
@property(nonatomic, readonly) int yearLabelSelectedFontSize;
@end
