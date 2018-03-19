//
//  CustomPromptManager.h
//  MicroStrategyMobile
//
//  Created by Lawton, Bruce on 1/17/12.
//  Copyright 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Prompts.h"


/**
 These are the methods that an app can provide in order to customize the appearance and behavior of prompts. All methods are optional. When implemented, they are to be implemented in a subclass of CustomPromptManager. The MicroStrategy Mobile SDK will create a single instance of the class when needed.
 */
@protocol MSICustomPromptManagerProtocol <NSObject>

@optional	// all methods are optional

/**
 This is the opportunity for a custom prompt manager to set answers before the UI is presented.
 It is called as soon as the original prompt answers are received from the server.
 
 @return	YES if any answers were changed.
 */
- (BOOL) reviseAnswersForPrompts: (Prompts*) prompts;


/**
 A custom prompt manager can bypass all of the prompting UI. It is called just before the UI is shown.

 @return	YES to skip the prompting UI and proceed directly to running the report.
 */
- (BOOL) shouldSkipPromptingUIForPrompts: (Prompts*) prompts;




/**
 An app indicates that it has a custom editor for the answer to the given prompt.
 
 @return	YES to get the custom editor used for the given Prompt
 */
- (BOOL) hasCustomAnswerEditorForPrompt: (MSIPrompt*) prompt;


/**
 An app provides a custom editor for the answer to the given prompt. The view controller should already be autoreleased and the SDK will be responsible for retaining and releasing.
 
 @return	a view controller to get the custom editor used for the given Prompt.
 */
- (UIViewController*) customAnswerEditorForPrompt: (MSIPrompt*) prompt;


/**
 Checks a prompt answer to ensure it meets any special criteria. This is different from - [ Prompt validateAnswer ] which only checks that a prompt has any required answer.
 
 @return	YES	if the prompt is valid.
 */
- (BOOL) validateAnswer: (MSIPrompt*) prompt;



@end // MSICustomPromptManagerProtocol




@interface MSICustomPromptManager : NSObject <MSICustomPromptManagerProtocol>

+ (MSICustomPromptManager*) sharedCustomPromptManager;

/**
 A custom answer editor will be invoked when the user selects the cell for that prompt. Just like the Element case.
 */
+ (BOOL) hasCustomAnswerEditorForPrompt: (MSIPrompt*) prompt;


@end



