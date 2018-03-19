//
//  MSIAppContext+Delegate.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 8/22/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "MSIAppContext.h"
#import "MSTRHandler.h"

/**
 * This is the place where holds the Delegate related methods
 **/
@interface MSIAppContext (Delegate) <MSTRHandler>

// Appication Life Cycle
- (void)handleApplicationStartupInitialization:(BOOL)withURL;
- (void)handleApplicationEnterForeground;
- (void)handleApplicationResignActive;
- (void)handleApplicationEnterBackground;
- (void)handleApplicationWillTerminate;
- (void)handleDidRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
- (void)handleDidFailToRegisterForRemoteNotificationsWithError:(NSError *)error;

// Preferences/Configuration
- (BOOL)useDefaultPreferences;
- (BOOL)updateExistingPreferences;
- (BOOL)needsliveConfigFetch;

// Reset home screen
- (BOOL)applicationLauncherNeedsSetup;

// Notification
- (void)handleDidReceiveRemoteNotification:(NSDictionary *)userInfo;
- (void)handleDidReceiveRemoteNotificationForNK:(NSDictionary *)userInfo;

// Application Launcher Controller
- (void)handleApplicationStartupFinished;
- (void)handleApplicationInitialized; //call this method when application is initialized

- (BOOL)isMSTRConfigID:(NSString*)configID excludeCurrent:(BOOL)excludeCurrent;
- (void)cleanupMobileAddressesForServers;

@end
