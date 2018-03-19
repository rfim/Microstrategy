//
//  MIPromptSummaryScreenSV.h
//  MSTRMobile
//
//  Created by Victor Pena on 6/22/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "MIDynamicTableViewController.h"

@class Prompts, PromptHelper, MIPromptSummaryScreen;

@interface MIPromptSummaryScreenSV : UIViewController<MIDynamicTableViewControllerDelegate> {
	Prompts *__weak promptList;
	PromptHelper *__weak promptHelper;

	UISplitViewController *splitView;
	MIPromptSummaryScreen *summaryScreen;
	UINavigationController *leftViewController;
	UINavigationController *rightViewController;
	UIViewController *__weak detailsScreen;
	
	BOOL isActualDisappear;
	NSString *reportName;
	NSInteger requiredCellCount;
	BOOL containsLocationPrompt;
	BOOL saveOnExit;
}

@property (nonatomic, weak) Prompts *promptList;
@property (nonatomic, weak) PromptHelper *promptHelper;
@property (nonatomic) MIPromptSummaryScreen *summaryScreen;
@property (nonatomic, weak) UIViewController *detailsScreen;
@property (nonatomic) UISplitViewController *splitView;
@property BOOL isActualDisappear;
@property (nonatomic, copy) NSString *reportName;


- (id)init:(Prompts *)prompts helper:(PromptHelper *)helper;
- (void)promptAnswersChanged;
//- (void)locationUpdated:(CLLocation *)newLocation;
//- (void)prepareForLocationPrompts;
@end
