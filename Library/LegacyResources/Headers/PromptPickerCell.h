//
//  PromptPickerCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/27/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptFormattableCell.h"

#define NUMERIC_MODE 0
#define DATE_TIME_MODE 1

#define NUMERIC_PICKER_WIDTH 300
#define NUMERIC_PICKER_HEIGHT 40
#define NUMERIC_PICKER_SIDE_OFFSET 10
#define DATE_TIME_IMAGE_WIDTH 31

@class PromptSummaryScreen, ConstantPrompt;

@interface PromptPickerCell : PromptFormattableCell  <UIPickerViewDataSource, UIPickerViewDelegate, UITextFieldDelegate, UIPopoverControllerDelegate, UIActionSheetDelegate> {
	UITextField *valueField;
	UILabel *valueLabel;
	UIImageView *dateTimeImageView;
	PromptSummaryScreen *parent;
	NSMutableArray *numericPickerElements; //holds the available elements for the numeric picker
	NSInteger curPickerSelection; //current selection index for the numeric picker
	NSInteger newPickerSelection;
	NSInteger mode;
	NSDate *curDateSelection;
	NSDate *freshDateSelection;
	
	UIPickerView *pickerView;
	UIDatePicker *dateTimePickerView;
	UIToolbar *pickerToolbar;
	UIPopoverController *pickerViewPopup;
	UIActionSheet *pickerActionSheet;
	UIViewController *pickerViewController;
	UIBarButtonItem *yearButton;
	BOOL b_pop;
}

@property (nonatomic, assign) PromptSummaryScreen *parent;
@property (nonatomic, retain) NSDate *curDateSelection;
@property (nonatomic, retain) NSDate *freshDateSelection;

+(PromptPickerCell*)newCellForPrompt:(ConstantPrompt*)prompt;
-(void)showPicker;
-(void)hidePicker;
-(NSDate *)convertToDate:(NSString *)dateString;
-(NSString *)convertToString:(NSDate *)date;

@end
