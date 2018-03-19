//
//  MIPromptSummaryScreen.h
//  MSTRMobile
//
//  Created by Victor Pena on 5/10/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "MIDynamicTableViewController.h"

@protocol MIIPromptSummaryScreenDelegate <NSObject>

- (void)PromptSummaryScreenDidLoad;
- (void)PromptSummaryScreenWillAppear;
- (void)PromptSummaryScreenDidAppear;

@end

@class Prompts, PromptHelper;
@class MIDynamicTableViewCell;
@class MIErrorMessageController;

@interface MIPromptSummaryScreen : MIDynamicTableViewController {
	Prompts *__weak promptList;
	PromptHelper *__weak promptHelper;
	
	MIErrorMessageController *errorMsgController;
	MIDynamicTableViewCell *errorMsgVisibleCellInRotation;
	BOOL isApplyErrorMsgVisible;
	BOOL isActualDisappear;
	NSString *reportName;
	NSInteger requiredCellCount;
	BOOL containsLocationPrompt;
	BOOL saveOnExit;
    BOOL enableButtons;
	id<MIIPromptSummaryScreenDelegate> __weak promptSummaryDelegate;
@private
     UIImageView* seperatorLine;
    BOOL didRotate;
}

@property (nonatomic, weak) Prompts *promptList;
@property (nonatomic, weak) PromptHelper *promptHelper;
@property BOOL isActualDisappear;
@property (nonatomic, copy) NSString *reportName;
@property (nonatomic, weak) id<MIIPromptSummaryScreenDelegate> promptSummaryDelegate;
@property (nonatomic, strong)  UIImageView* seperatorLine;
- (id)init:(Prompts *)prompts helper:(PromptHelper *)helper;
- (void)promptAnswersChanged;
//- (void)locationUpdated:(CLLocation *)newLocation;
//- (void)prepareForLocationPrompts;
- (void)answerPromptsFromSender:(id)sender;
- (void)cancelPrompts;
- (void)clearButtons;
- (void)addButtons;
-(void) enableButtons:(BOOL)enable;
- (void)answerPromptsFromCache:(BOOL)answerChanged;
@end
