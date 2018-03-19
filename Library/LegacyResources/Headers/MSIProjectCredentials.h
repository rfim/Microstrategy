//
//  MSIProjectCredentials.h
//  MSTRMobileSDK
//
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSICredentials.h"

@class MSIMobileLoginCustomAuthenticationParameters;

/**
 * SubClass of Credentials.
 * Credentials used in the ProjectConfiguration as project credentials and WebServerConfiguration as default project credentials. 
 * This credentials will be initialized with Standard authentication mode by default.
 * It has the same behavior as a basic Credentials.
 */
@interface MSIProjectCredentials : MSICredentials {
    MSIMobileLoginCustomAuthenticationParameters * customAuthenticationParameters;
}

@property (nonatomic, strong) MSIMobileLoginCustomAuthenticationParameters * customAuthenticationParameters;

@end
