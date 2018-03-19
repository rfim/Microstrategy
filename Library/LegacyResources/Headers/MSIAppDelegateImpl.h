//
//  MSIAppDelegateImpl.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 8/13/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIAppDelegate.h"
#import <UserNotifications/UserNotifications.h>
/**
 * This is a pure mobile app delegate class, and it has been referenced in XIB
 * If any other app want integrate our feature, this class should be an example to see how to integrate
 **/
@interface MSIAppDelegateImpl : NSObject <MSIAppDelegate,UNUserNotificationCenterDelegate>

/**
 * This property is exposed in UIApplicationDelgate, here is adding "IBOutlet" to combine the outlet in XIB
 **/
@property (nonatomic, strong) IBOutlet UIWindow *window NS_AVAILABLE_IOS(5_0);
@property (copy) void (^backgroundSessionCompletionHandler)();
@end
