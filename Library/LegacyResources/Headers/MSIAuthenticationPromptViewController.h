//
//  MSIAuthenticationPromptViewController.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 3/7/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginPromptViewController.h"

@class MSIProjectInfo, SettingsTextField, MSIDevicePreferences;

#define MSI_AUTHENTICATION_DISABLE_ANIMATION_KEY @"disableAnimation"

@interface MSIAuthenticationPromptViewController : MSIMobileLoginPromptViewController

/**
 * Specifies whether to save project authentication credentials in preferences store
 * Default value is YES
 */
@property BOOL persistProjectAuthenticationCredentialsInPreferencesStore;

@end
