//
//  MIPromptDetailsViewControllerUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/19/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface MIPromptDetailsViewControllerUIHelper : UIHelper{
    UIColor*  navigationbarTintColor;
    UIColor*  buttonDoneColor;
    UIColor*  titleColor;
    UIColor*  disableButtonColor;
    UIColor*  backButtonColor;
}
@property(nonatomic, readonly) UIColor* navigationbarTintColor;
@property(nonatomic, readonly) UIColor* buttonDoneColor;
@property(nonatomic, readonly) UIColor* titleColor;
@property(nonatomic, readonly) UIColor* disableButtonColor;
@property(nonatomic, readonly) UIColor* backButtonColor;
-(UIColor*) tableSeperateLineColor;
@end
