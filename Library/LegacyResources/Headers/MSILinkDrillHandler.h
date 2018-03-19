//
//  MSILinkDrillHandler.h
//  MicroStrategyMobile
//
//  Created by Zhang, Yuan on 5/8/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSICommanderDelegate.h"

@protocol MSILinkDrillCustomEventDelegate

@required
/**
 *  If SDK user want to implement the following function to do custom link drill method, SDK user should implement this method to yes.
 *  1. If the delegate doesn't implement this method or if the delegate returns false, the current behavior applies
 *  2. If the delegate implements this method and returns YES, all the events (including regular report execution, link drilling etc) will be sent to the new executeURL:withCommander:sourceView delegate method (or -(void)executeURL:(NSString *)url withCommander:(id<MSICommanderDelegate>)commander if the new one is not implemented) and the delegate is responsible to handle the navigation accordingly.
 *  3. If the delegate implements this method and returns YES, no navigation will be performed by the app, as the delegate is responsible to handle it.
 */
- (BOOL)shouldHandleLinkDrillNavigation;

/**
 * SDK user should implement this method if they want to define custom event for link drill
 * @param url is the url string defined inside the document
 * @param commander is the commander of current controller
 * @param sourceView, from which to trigger the link drill
 */
- (void)executeURL:(NSString *)url withCommander:(id<MSICommanderDelegate>)commander sourceView:(UIView *)view;

@optional
/**
 * SDK user should implement this method if they want to define custom event for link drill
 * @param url is the url string defined inside the document
 * @param commander is the commander of current controller
 */
- (void)executeURL:(NSString*)url withCommander:(id<MSICommanderDelegate>)commander __attribute__((deprecated));

@end


@interface MSILinkDrillHandler : NSObject

@property (nonatomic, weak) id<MSILinkDrillCustomEventDelegate> customEventDelegate;

// Get the singleton object
+ (MSILinkDrillHandler*) linkDrillHandler;

/**
 * SDK user can call this class method to execute a url
 * @param link is the url string defined inside the document
 * @param commander is the commander of current controller
 */
+ (void)handleURLLink:(NSString*)link withCommander:(id<MSICommanderDelegate>)commander;

+ (NSString*)reconstructResolvedLinksWithString:(NSString*)resolvedLinks dictionary:(NSDictionary*)dic;

@end
