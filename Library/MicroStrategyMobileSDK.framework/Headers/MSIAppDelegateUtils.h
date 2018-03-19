//
//  MSIAppDelegateUtils.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 8/27/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"
#import <UserNotifications/UserNotifications.h>
#define MSI_APPLICATION_DID_FINISH_LAUNCHING_NOTIFICATION @"ApplicationDidFinishLaunching"
@class ApplicationLauncherController;

/**
 * SDK users can Category MSIAppDelegateUtils Class to customize the functionality
 */
@interface MSIAppDelegateUtils : NSObject <UIApplicationDelegate,UNUserNotificationCenterDelegate>

/**
 * The methods exposed here, are those used by customized app
 * if noly used inside our app, we should move them to Internal header file
 * In this class, we should be careful to update key window status directly since this will also influence customized app
 **/

/**
 * Check special requirement to see whether we can startup application.
 * @param application application to be launched
 * @param errorString pointer to a string contains the error
 * @return YES if application can be launched, NO if not
 */
- (BOOL)applicationShouldLaunch:(UIApplication *)application errorDescription:(NSString**)errorString;

/**
 * Setup the root navigation controller with an ApplicationLauncherController on top
 * @param applicationLauncherController an ApplicationLauncherController
 */
- (void)setupNavigationControllerWithApplicationLauncher:(ApplicationLauncherController*)applicationLauncherController;

/**
 * Add by bwang, this API will only be called in Automation Test.
 **/
- (void)closeAllSessionsFromCurrentClient;

/**
 * Reload the application with a specific url
 * @param url Application Configuration URL
 */
- (void)reloadURL:(NSURL *)url;

- (BOOL)isLaunchedWithURL;

- (MSTRApplicationState)applicationState;

/**
 * Use this to override the settings for screen content hide on the policy manager objects.
 **/
- (void)setShouldHideScreenContentsOnBackground:(BOOL)shouldHide;

- (void)deviceUnlocked;

- (void) resetToDemo;

@end
