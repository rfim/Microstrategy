//
//  TimeViewControllerDelegate.h
//  MicroStrategyMobile
//
//  Created by Yonggang Xi on 4/12/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

@protocol TimeViewControllerDelegate <NSObject>

-(void)timeControllerSelectionDone;
-(void)timeViewDidDisappear;
-(void)popoverViewDidDisappear;
@end

@protocol TimeViewController <NSObject>

-(void)setDelegate:(id<TimeViewControllerDelegate>)dlg;
-(void)displayTimeView:(CGRect)pickerFrame fromFrame:(CGRect)rootFrame inView:(UIView *)view inController:(UIViewController*)controller allowDismiss:(BOOL)allowDismiss;
-(NSDate *)getSelectedTime;
-(void)hidePicker;
@end