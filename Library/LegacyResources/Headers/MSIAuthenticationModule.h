//
//  MSIAuthenticationModule.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 3/14/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginModule.h"
@class MSIProjectInfo;

#define MSI_AUTHENTICATION_PASSWORD_KEY @"password"
#define MSI_AUTHENTICATION_IS_SERVER_ERROR_KEY @"isServerError"
#define MSI_AUTHENTICATION_ADDITIONAL_MOBILE_LOGIN_PARAMETERS_KEY @"additionalMobileLoginParameters"

@interface MSIAuthenticationModule : MSIMobileLoginModule

/**
 * Remove the project's session state from session table and close it by MSICloseSessionsService
 * @param project the project
 */
-(void)logoutFromProject:(MSIProjectInfo*) project;

/**
 * Remove all session states from session table and close them by MSICloseSessionsService
 */
-(void)logoutFromAllProjects;

@end
