//
//  PromptCalendarCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/2/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"
#import "PromptSummaryScreen.h"
#import "CalendarTopLevelController.h"

#define IMAGE_WIDTH 31
#define CALENDAR_MIDDLE_PADDING 5

@class PromptSummaryScreen, Prompt;

@interface PromptCalendarCell : PromptBaseCell <CalendarDelegate> {
	UILabel *valueLabel;
	UIImageView *calendarImageView;
	
	PromptSummaryScreen *parent;
}

@property (nonatomic, assign) PromptSummaryScreen *parent;

+(PromptCalendarCell*)newCellForPrompt:(Prompt*)_prompt;

-(void)dateSelectionDone:(NSDate *)newSelection;
-(void)dateSelectionCancelled;

@end
