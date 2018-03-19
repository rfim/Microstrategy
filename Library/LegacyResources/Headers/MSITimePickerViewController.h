//
//  MSITimePickerViewController.h
//  MicroStrategyMobile
//
//  Created by Yonggang Xi on 4/12/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TimeViewControllerDelegate.h"

#import <Enums.h>
#define IMAGE_WIDTH 31
#define TIME_MIDDLE_PADDING 5

@interface MSITimePickerViewController : UIViewController <UIActionSheetDelegate, UIPopoverControllerDelegate, TimeViewController> {
	CGRect mPickerFrame;
	CGRect mRootFrame;
	UIView *__weak parentView;
	
	UIDatePicker *timePickerView;
	UIToolbar *pickerToolbar;
	UIActionSheet *pickerActionSheet;
	UIPopoverController *pickerViewPopup;
	UIViewController *pickerViewController;
	BOOL b_pop;
	BOOL b_allowDismiss;
	
	NSDate *confirmedTimeSelection;
	NSDate *selectedTimeSelection;

	NSString *minuteInterval;
	
	id<TimeViewControllerDelegate> __weak delegate;
    ThemeType mThemeType;
    UIViewController* parentController;
    UIAlertController *pickerActionSheetController;
}

-(id)initWithDateTime:(NSDate *)dateTime interval:(NSString *)interval;
-(void)hidePicker;
-(void)showPicker;
-(id)initWithDateTime:(NSDate *)dateTime interval:(NSString *)interval withThemeType:(ThemeType)themetype;
@property (nonatomic, weak) id<TimeViewControllerDelegate> delegate;
@property (nonatomic, assign) CGRect pickerFrame;
@property (nonatomic, assign) CGRect rootFrame;
@property (nonatomic, weak) UIView *parentView;
@property (nonatomic, strong) NSDate *confirmedTimeSelection;
@property (nonatomic, strong) NSDate *selectedTimeSelection;

@property (nonatomic, copy) NSString *minuteInterval;

@end
