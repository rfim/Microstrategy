//
//  UIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/1/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//
///   when you create any class which has ui by ios, you can create a corrisponding uihelper to provide the ui standard based on ios and theme color or device
#import <Foundation/Foundation.h>
#import "Enums.h"

@class DocumentThemeHelper;
#define MIDYNAMIC_CONTENT_SIDE_PADDING 12
#define MIDYNAMIC_CELL_SIDE_PADDING 10
@interface UIHelper : NSObject{
@public
    NSInteger iosVersion;
    ThemeType themetype;
@protected
    UIColor* DA;
    UIColor* LA;
    UIColor* DB;
    UIColor* LB;
    UIColor* DD;
    UIColor* LD;
    UIColor* DE;
    UIColor* LE;
    UIColor* LC;
    UIColor* DC;
    UIColor* F;

}
@property(nonatomic, readonly)ThemeType themetype;
@property(nonatomic, readonly)NSInteger iosVersion;
-(UIColor*)getBackgroundColor;
-(UIColor*)getTextTintColor;
-(UIColor*)getIconTintColor;
-(UIColor*)getSecondaryTextColor;
-(id)initWithThemeType:(ThemeType)_themetype;
-(void)addContent;
@end
