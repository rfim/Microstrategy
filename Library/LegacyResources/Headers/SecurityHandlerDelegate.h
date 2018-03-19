//
//  SecurityHandlerDelegate.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 11/24/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


@protocol SecurityHandlerDelegate <NSObject>

/** Callback for authorization success
  */
- (void)authorizationSuccess;

@optional

- (void)policyUpdate:(NSDictionary*)appPolicySettings;
- (void)notAuthorized;

/** Callback for authorization failure
 */
- (void)authorizationFailure;

@end
