//
//  PromptHelper.h
//  MSTRMobile
//
//  Created by Amirali Charania on 1/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "MSIServiceImpl.h"
#import "BarcodeScannerController.h"
#import "KeypadInputController.h"
#import "MSICalendarViewController.h"

typedef enum _EnumPromptAnswerStatus
{
    PromptAnswerStatusUnknown = -1,
    PromptAnswerStatusSucceed = 0,
    PromptAnswerStatusCancelled = 1,
    PromptAnswerStatusError = 2,
    PromptAnswerStatusUnchanged = 3//by lishan 664123
} EnumPromptAnswerStatus;

@class Prompts, MSIReportDisplayInfo, MSIObjectInfo, MSIProjectConfiguration, ElementListController, 
SelectedElementListController, /*BaseDataDisplayController,*/ MSIConstantPrompt, PromptsService,PromptSelectorController, ElementListController;
@class MSIPrompt;

@protocol PromptHelperDelegate

- (void)updatePromptedReport:(MSIReportDisplayInfo*)report;
- (UIViewController*)viewController;
- (void)addBackgroundCoveringViewAfterPrompt:(UIView *)ipView
								belowSubview:(UIView *) ipSiblingView;

@optional

//TQMS:438696, we have to hide the right navigation bar item when start reprompt and restore it when cancel the prompt summary screen
- (void)onPromptScreenClosed:(int)status;
- (void)onPromptScreenPopsUp;
- (void)willPopViewController;
- (id)getCurrentViewer;
@end


  
/**
 PromptHelper displays each individual Prompt and will display the PromptSummaryScreen if there are multiple prompts.
 
 PromptHelper is the primary way that the data controller (as in MVC) gets document prompts answered. A PromptHelper is obtained using the PromptControllersFactory, which will provide one according to the idiom, iPhone or iPad.

 The delegate of the PromptHelper is expected to have a viewController with a navigationController.
 
 
 The simplest way to use to use it is shown in this snippet:
 
	PromptHelper* promptHelper = [[ PromptControllersFactory promptControllersFactory ] newPromptHelperWithPrompts: promptsToShowFirst ];
	promptHelper.report = promptedReport;
	promptHelper.delegate = self;			// self is an IPadDataController. Expected to respond to viewController, updatePromptedReport:, onPromptScreenPopsUp, onPromptScreenClosed
	promptHelper.isReprompt = isReprompt;	// YES or NO

	// Assign to our property...
	self.promptHelper = promptHelper;
	[ promptHelper release ];

	// Display the prompt summary screen...
	BOOL	animate = NO;
	[ self.promptHelper displayPrompt: animate ];

 
 
	For custom prompting, we allow six prompt-related options:
		a) programmatically set answers as soon as they arrive from the server.
		b) bypass all prompt UI and proceed directly to running the report.
		c) proceed with prompts UI (default)
		d) provide custom prompt cell, VC for the PromptSummaryScreen.
		e) provide a custom answer editor, like the Elements one.
		f) provide a custom validateAnswer method.
 
 CustomPromptManager checks for a file, MSTRCustomizations.plist, and looks for a key customPromptManager
 that we'll use for these. It has a protocol where all methods are optional and they are ...
 
 

 
 @see PromptControllersFactory
 @see PromptSummaryScreen
*/
@class MIPromptViewController;

@interface PromptHelper : NSObject <ServiceHandler, CalendarDelegate, CLLocationManagerDelegate, BarcodeScannerControllerDelegate, KeypadInputControllerDelegate> {
	MSIReportDisplayInfo* report;
	NSInteger currentPromptIndex;
	MSIObjectInfo* attribute;
	Prompts* promptCache;
	BOOL multiSelect;
	BOOL searchRequired;
	id<PromptHelperDelegate> __weak delegate;
	UIViewController *currentController;
    MIPromptViewController* mpPromptViewController;
	BOOL anyPromptAnswerChanged;
	NSMutableArray *prevPromptAnswers;
	NSMutableArray *prevConstantPromptAnswers; //to understand if any constant prompt answer has been changed
	PromptsService* promptsService;
	BOOL animateInitially;
	NSThread *currentThread;
	BOOL isReprompt;
    BOOL isLinkDrill;
    NSDictionary *answerModes;
	CLLocationManager *locationManager;
	NSInteger currentLocErrorCode;
    BOOL currentlyAnsweringLocPrompts;
	BOOL containsLocationPrompt;
	PromptSelectorController* currentSelectorController;
	EnumPromptAnswerStatus currentPromptAnswerStatus;
	UIView *backgroundCoveringView;
	BOOL isAutoDisplayDetails;
    ThemeType themetype;
    BOOL isBackgroundNavigationBarHidden;//793295
    UIColor* accentColor;
}

- (id)initWithPrompts:(Prompts*)prompts;
- (Prompts*) prompts;
- (void) setPrompts: (Prompts*)p;
/**
 Displays a modal view for editing an Element list.
 */
- (void) displayElementListScreen: (MSIObjectInfo*) promptAttribute parentViewController: (UIViewController*) parent;

/**
 Displays a modal view for custom editing of a prompt answer.
 */
- (void) displayCustomPromptScreenWithParentViewController: (UIViewController*) parent;

- (void) storePersonalization:(NSMutableArray*)elements dismissPreviousView:(BOOL)iDismiss;

/**
 Used when a custom prompt editor is done. It validates the answer for the current prompt and then dismisses the custom editor.
 */
- (void) validateCustomSaveAndDismissPreviousView: (BOOL) dismiss;

- (MSIProjectConfiguration*) project;

/**
 Gets the prompt answers into the cache if they are not already there and displays the prompts from the cache.
 
*/
- (void) displayPrompt: (BOOL) animate;
- (EnumPromptAnswerStatus)getCurrentPromptAnswerStatus;
- (BOOL)validatePromptAnswer:(MSIPrompt *)curPrompt;
- (BOOL)validateConstantLocationPromptAnswers;
- (void)barcodeScannerDone:(NSString *)newSelection;
- (void) elementSelectionDone;
- (void) elementSelectionCancelled;
- (void)displayPromptAtIndex:(NSInteger)index;
- (void)savePromptAnswers;
- (void)saveAnswersAndDisplayReport;
- (BOOL)didElementsChange:(NSMutableArray *)elements;
- (void)saveExistingPromptAnswers;
- (BOOL)promptAnswersChanged;
- (void)saveExistingConstantPromptAnswers;
- (BOOL)constantPromptAnswersChanged;
- (void)locationUpdated:(CLLocation *)newLocation;
- (NSArray *)locationFromConstantPrompts;
- (void)displayCalendar:(MSIConstantPrompt*)curPrompt;
- (BOOL)shouldDisplayDateTime:(MSIPrompt *)curPrompt;
- (void)cancelPrompts;
- (void)doneSummaryPrompts;
- (void)cancelModalPrompts;
- (void)displayBarcodeScanner:(MSIConstantPrompt *)curPrompt;
- (void)displayPromptsFromCache:(Prompts *)data;
- (void)completeSaveAndDisplay;
- (void)cancelRunningThread;
+ (void)setBarcodeXmlValue:(NSString*)value prompt:(MSIConstantPrompt*)curPrompt;
+ (NSString*)getBarcodeValue:(MSIConstantPrompt*)curPrompt;
- (void)displayWaitView;
- (void)hideWaitView;

- (void)calendarCanceled;
- (void)expressionEditingDone;
- (MIPromptViewController*)getPromptViewController;
+ (BOOL)isPromptInPrompt:(Prompts *)prompts inDocument:(MSIReportDisplayInfo *)document;
- (BOOL)isPromptInPromptInDocument:(MSIReportDisplayInfo *)document;
- (void)setCurrentPrompt:(MSIPrompt*)prompt;
- (NSArray *)getCachedPrompts;
@property (nonatomic, strong) MSIReportDisplayInfo* report;
@property BOOL multiSelect;
@property BOOL searchRequired;
@property (nonatomic, strong) PromptSelectorController* currentSelectorController;
@property (readonly) NSInteger currentPromptIndex;
@property (nonatomic, strong) MSIObjectInfo* attribute;
@property (nonatomic, weak) id<PromptHelperDelegate> delegate;

@property (nonatomic, strong) UIViewController *currentController;
@property BOOL animateInitially;
@property BOOL anyPromptAnswerChanged;
@property (readonly) PromptsService* promptsService;
@property (nonatomic, strong) NSThread *currentThread;
@property BOOL isReprompt;
@property BOOL isLinkDrill;
@property (nonatomic, copy) NSDictionary* answerModes;
@property BOOL containsLocationPrompt;
@property (readonly) BOOL currentlyAnsweringLocPrompts;
@property (readonly) NSInteger currentLocErrorCode;
@property (readonly) BOOL isAutoDisplayDetails;
@property (nonatomic) ThemeType themetype;
@property (nonatomic, copy) UIColor* accentColor;
@property BOOL isBackgroundNavigationBarHidden;
@end
