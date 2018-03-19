//
//  MSIMobileLoginPromptViewController.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 1/3/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSIMobileLoginPromptView.h"
#import "MSIMobileLoginModule.h"
#import "Enums.h"
#import "MSTRError.h"

@class MSIMobileLoginPromptViewController;

/**
 Protocol for mobile login manager to implement.
 */
@protocol MSIMobileLoginPromptViewControllerDelegate

/**
 Called when controller is dismissed.
 @param viewController The mobile login prompt view controller
 @param promptType The type of the prompt
 @param updateFlag Whether the prompt has updated credentials
 */
- (void) mobileLoginPromptViewController:(MSIMobileLoginPromptViewController *)viewController withPromptType:(MobileLoginPromptType)promptType didDismissWithUpdateFlag:(BOOL) updateFlag;

/**
 Called when controller has encountered error.
 @param viewController The mobile login prompt view controller
 @param promptType The type of the prompt
 @param error The error
 */
- (void) mobileLoginPromptViewController:(MSIMobileLoginPromptViewController *)viewController withPromptType:(MobileLoginPromptType)promptType didEncounterError:(MSTRError *) error;;

@end

#define MSI_AUTHENTICATION_KEY_KEY @"Key"
#define MSI_AUTHENTICATION_TYPE_DICTIONARY_KEY @"Type"
#define MSI_AUTHENTICATION_CONFIGURATION_DICTIONARY_KEY @"Configuration"
#define MSI_AUTHENTICATION_DEFAULT_VALUE_KEY @"Default Value"
#define MSI_AUTHENTICATION_LABEL_KEY @"Label"
#define MSI_AUTHENTICATION_PLACE_HOLDER_TEXT_KEY @"Placeholder Text"
#define MSI_AUTHENTICATION_AUTHENTICATION_PARAMETERS_KEY @"Authentication Parameters"

@interface MSIMobileLoginPromptViewController : UIViewController<MSIMobileLoginPromptViewDelegate, MSIMobileLoginModuleDelegate>

@property (nonatomic, weak) id<MSIMobileLoginPromptViewControllerDelegate> delegate;
@property (nonatomic, strong) MSIMobileLoginModule* mobileLoginModule;

/**
 Init the view controller with view and mobileLoginModule.
 @param mobileLoginPromptView The mobile login prompt view
 @param mobileLoginModule The mobile login module
 @param parameters  The parameters mobile login mamager passes to the view controller for init
 */
- (void) setupWithMobileLoginPromptView:(MSIMobileLoginPromptView *) mobileLoginPromptView mobileLoginModule:(MSIMobileLoginModule*) mobileLoginModule parameters:(NSDictionary*) promptParameters;

/**
 Show the loading view with information.
 @param info The information to displauy
 */
- (void)showLoadingViewWithInfo:(NSString*)info;

/**
 Hide the loading view
 */
- (void)hideLoadingView;

@end
