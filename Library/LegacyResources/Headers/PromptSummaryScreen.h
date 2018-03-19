//
//  PromptSummaryScreen.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@class Prompts, PromptHelper, PromptBaseCell;

/**
 Used when there are multiple prompts to answer or when the prompt will be handled entirely within
 the UITableView of the summary screen. A slider is one such type.
 */
@interface PromptSummaryScreen : UITableViewController {
	Prompts *promptList;
	PromptHelper *promptHelper;
	NSMutableArray* cells;
	
	BOOL isActualDisappear;
	NSString *reportName;
	NSInteger requiredCellCount;
	BOOL containsLocationPrompt;
	BOOL saveOnExit;
}

@property (nonatomic, retain) Prompts *promptList;
@property (nonatomic, assign) PromptHelper *promptHelper;
@property BOOL isActualDisappear;
@property (nonatomic, copy) NSString *reportName;
@property (nonatomic, retain) NSMutableArray *cells;

- (id)initWithStyle:(UITableViewStyle)style promptList:(Prompts *)prompts helper:(PromptHelper *)helper;
- (void)promptAnswersChanged;
- (void)locationUpdated:(CLLocation *)newLocation;
- (void)prepareForLocationPrompts;
- (void)cancelPrompts;
@end
