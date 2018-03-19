//
//  MSIMobileLoginManager.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 10/11/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSTRError.h"
#import "MSIProjectInfo.h"
#import "WebServerInfo.h"
#import "MSIProjectConfiguration.h"
#import "Service.h"
#import "MSIServiceImpl.h"
#import "MSIMobileLoginPromptView.h"
#import "MSIMobileLoginPromptViewController.h"
#import "MSIMobileLoginModule.h"
#import "MSIConnectionManager.h"

#define MOBILE_LOGIN_PROMPT_STATUS_WAITING      0
#define MOBILE_LOGIN_PROMPT_STATUS_ANSWERED     1
#define MOBILE_LOGIN_PROMPT_STATUS_CANCELLED	2
#define MOBILE_LOGIN_PROMPT_STATUS_ERROR        3
#define MOBILE_LOGIN_PROMPT_STATUS_RETRY  0x4321

#define MOBILE_LOGIN_PROMPT_CANCELLED_CODE -0x1234
#define MOBILE_LOGIN_PROMPT_AUTH_REQUIRED_MSG  @"Authorization required to proceed"

/**
 Protocol for mobile login prompts.
 */
@protocol MSIMobileLoginPromptDelegate <NSObject>
@optional
/**
 Called before mobile login prompts displays.
 @param promptType The type of the prompt.
 */
- (void)handleMobileLoginPromptDisplay:(MobileLoginPromptType)promptType;

/**
 Called after the prompt is dismissed and the login of mobile login prompts is success.
 @param promptType The type of the prompt.
 */
- (void)handleMobileLoginPromptSuccess:(MobileLoginPromptType)promptType;

/**
 Called after the prompt is dismissed and the login of mobile login prompts is canceled.
 @param promptType The type of the prompt.
 */
- (void)handleMobileLoginPromptCancel:(MobileLoginPromptType)promptType;

/**
 Called after the prompt is dismissed and the login of mobile login prompts failed with error.
 @param error The error of the prompt.
 @param promptType The type of the prompt.
 */
- (void)handleMobileLoginPromptError:(MSTRError*)error promptType:(MobileLoginPromptType)promptType;

@end

@interface MSIMobileLoginManager : NSObject

/**
 * Get the instance of the manager.
 * @return the instance of the manager
 */
+ (MSIMobileLoginManager*) sharedMobileLoginManager;

//

/**
 * Display prompt for the prompt type.
 * @param promptType the prompt type
 * @param promptParameters the parameters for the prompt
 * @param animated whether to show the prompt with animation
 * For AuthenticationPromptType and ChangePasswordPromptType:
     Key                        Value Type      Description
     @"projectInfo"             ProjectInfo     The project for Authentication Prompt creating MSISessionService / Change Password Prompt creating ChangePasswordService.
     @"canBeCancelled"          NSString        @"true" for YES; other for NO.
     @"onlyMobileServerError"   NSString        @"true" for YES; other for NO.
 * For HTMLFormPromptType:
     Key                        Value Type      Description
     @"projectInfo"             ProjectInfo     The project for HTML Form Prompt fetching base URL from preferencesStore.
     @"canBeCancelled"          NSString        @"true" for YES; other for NO.
     @"onlyMobileServerError"   NSString        @"true" for YES; other for NO.
     @"baseURL"                 NSString        Base URL for HTML Form Prompt to browse. If this string is available, we'll ignore the 3 parameters above.
 * For CertificatePromptType
     Key                        Value Type      Description
     @"certificateServer"       NSString        The certificate server URL for Certificate Prompt creating CertificateLoginFieldsService.
 * @param delegate the delegate for the prompt, it should implement the 4 methods defined in MSIMobileLoginPromptDelegate
 * @return the prompt status, could be MOBILE_LOGIN_PROMPT_STATUS_WAITING, MOBILE_LOGIN_PROMPT_STATUS_ANSWERED, MOBILE_LOGIN_PROMPT_STATUS_CANCELLED or MOBILE_LOGIN_PROMPT_STATUS_ERROR
 */
- (int)showPrompt:(MobileLoginPromptType)promptType parameters:(NSDictionary *)promptParameters delegate:(id<MSIMobileLoginPromptDelegate>)delegate animated:(BOOL) animated;

- (int)showPrompt:(MobileLoginPromptType)_promptType parameters:(NSDictionary *)_promptParameters delegate:(id<MSIMobileLoginPromptDelegate>)_delegate;

/**
 * Unregister prompt delegate. Usually called after the prompt is dismissed.
 * @param delegate the delegate for the prompt, it should be the current prompt's delegate
 */
- (void)unRegisterPromptDelegate:(id<MSIMobileLoginPromptDelegate>)delegate;

/**
 * Return whether mobile login prompt is displaying. Usually called when another controller is trying to present view on application navigation controller or top modal view controller.
 * @return return YES if mobile login prompt is displaying on screen.
 */
- (BOOL) isMobileLoginPromptViewControllerDisplaying;

- (MSIConnectionManager* ) getConnectionManager;

/**
 * Set the view, controller and module for the prompt type. Used for prompt customization.
 * @param userDefinedPromptView view for the prompt type
 * @param userDefinedPromptViewController view controller for the prompt type
 * @param userDefinedPromptModule module for the prompt type
 * @param promptType the prompt type
 */
- (void)setView:(MSIMobileLoginPromptView*) userDefinedPromptView controller:(MSIMobileLoginPromptViewController*) userDefinedPromptViewController module:(MSIMobileLoginModule*)userDefinedPromptModule forPromptType:(MobileLoginPromptType)promptType;

/**
 * Unset the view, controller and module for the prompt type. MSIMobileLoginManager will then use the default ones for prompts.
 * @param promptType the prompt type
 */
- (void)resetViewControllerAndModuleForPromptType:(MobileLoginPromptType)promptType;

/**
 * The connection manager for authentication prompts, it is the session connection manager by default
 */
@property (nonatomic, retain) MSIConnectionManager* connectionManager;

/**
 * The user defined connection manager
 */
@property (nonatomic, retain) MSIConnectionManager* userDefinedConnectionManager;

- (int)showCredentialsPromptWithParameters:(NSDictionary *)_promptParameters delegate:(id<MSIMobileLoginPromptDelegate>)_delegate;

@end
