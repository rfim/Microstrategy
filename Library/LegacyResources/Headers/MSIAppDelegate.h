//
//  MSIAppDelegate.h
//  MSTRMobileSDK
//
//  Created by Tejas Ranade on 5/4/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSIAppDelegateUtils;

/**
 * This protocal should be implemented by the app which wants use MicroStrategy SDK
 **/
@protocol MSIAppDelegate <UIApplicationDelegate>

/**
 * This is the real MSTR Delegate implementation instance and holds the function implementation
 **/
@property (nonatomic, readonly) MSIAppDelegateUtils * mstrAppDelegateUtils;

/*
 * This is the place where we handle MSTR Mobile App Start View Controller
 * MSTR Mobile Function will start from this controller
 * Normally, this should be set as the Application Root View Controller
 */
- (void)updateMSTRStartViewController:(UIViewController*)startViewController;

@end
