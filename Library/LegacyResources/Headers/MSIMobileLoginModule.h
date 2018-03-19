//
//  MSIMobileLoginModule.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 3/14/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginPromptView.h"

@class MSIMobileLoginModule;
/**
 Protocol for mobile login controller to implement.
 */
@protocol MSIMobileLoginModuleDelegate <NSObject>

/**
 Called when mobile login module successfully did the login.
 @param loginModule the mobile login module that sends the message
 @param successInfo The information mobile login module returns to MSIMobileLoginModuleDelegate
 */
- (void) mobileLoginModule:(MSIMobileLoginModule *)loginModule mobileLoginDidSucceed:(NSDictionary *)_successInfo;

/**
 Called when mobile login module failed to login.
 @param loginModule the mobile login module that sends the message
 @param failureInfo The information mobile login module returns to MSIMobileLoginModuleDelegate
 */
- (void) mobileLoginModule:(MSIMobileLoginModule *)loginModule mobileLoginDidFail:(NSDictionary *)_failureInfo;

/**
 Called when mobile login module starts login.
 @param loginModule the mobile login module that sends the message
 @param preLoginInfo The information mobile login module returns to MSIMobileLoginModuleDelegate
 */
- (void) mobileLoginModule:(MSIMobileLoginModule *)loginModule willHandlePreMobileLogin:(NSDictionary *)_preLoginInfo;

/**
 Called when mobile login module finishs login.
 @param loginModule the mobile login module that sends the message
 @param postLoginInfo The information mobile login module returns to MSIMobileLoginModuleDelegate
 */
- (void) mobileLoginModule:(MSIMobileLoginModule *)loginModule willHandlePostMobileLogin:(NSDictionary *)_postLoginInfo;

@end

#define MSI_AUTHENTICATION_PROJECT_INFO_KEY @"projectInfo"
#define MSI_AUTHENTICATION_ERROR_KEY @"error"
#define MSI_AUTHENTICATION_DATA_KEY @"data"

@interface MSIMobileLoginModule : NSObject {

}


@property (nonatomic, weak) id<MSIMobileLoginModuleDelegate> mobileLoginModuleDelegate;

/**
 Start login.
 @param mobileLoginParameters The information for mobile login module to login
 */
- (void) startMobileLogin:(NSDictionary *) _mobileLoginParameters;

@end
