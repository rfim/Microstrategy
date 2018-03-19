//
//  PromptTimePickerCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/22/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"

#define IMAGE_WIDTH 31
#define TIME_MIDDLE_PADDING 5

@class PromptSummaryScreen;

@interface PromptTimePickerCell : PromptBaseCell <UIActionSheetDelegate> {
	UILabel *valueLabel;
	UIImageView *timeImageView;
	PromptSummaryScreen *parent;
	
	UIDatePicker *timePickerView;
	UIToolbar *pickerToolbar;
	UIActionSheet *pickerViewPopup;
	
	NSDate *curTimeSelection;
	NSDate *freshTimeSelection;
	NSString *dateValue; //need this value while submitting answers
}

+(PromptTimePickerCell *)newCellForPrompt:(Prompt*)_prompt;
-(NSDate *)convertToDate:(NSString *)timeString;
-(NSString *)convertToString:(NSDate *)date withDateInfo:(BOOL)appendDateInfo;
-(void)showPicker;
-(NSString *)getTimeValue:(NSString *)dateStr;
-(NSString *)getDateValue:(NSString *)dateStr;

@property (nonatomic, assign) PromptSummaryScreen *parent;
@property (nonatomic, retain) NSDate *curTimeSelection;
@property (nonatomic, retain) NSDate *freshTimeSelection;
@property (nonatomic, copy) NSString *dateValue;

@end
