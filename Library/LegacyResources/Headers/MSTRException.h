//
//  MSTRException.h
//  MSTRFoundation
//
//  Created by Amirali Charania on 3/11/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#define PREFERENCES_WRITE_FAIL @"PreferencesWriteFailException"
#define REPORT_STALE @"ReportStaleException"
#define CREATE_SESSION_FAILURE @"CreateSessionFailureException"
#define AUTHORIZATION_REQUIRED @"AuthorizationRequiredException"

#import <Foundation/Foundation.h>

@class ErrorDetails;

/**
 * MSTRException is the generic class to represent an exception in MicroStrategyMobileSDK. Application code can throw and catch this kind of exceptions when something unexpected happens.
 */
@interface MSTRException : NSException {
}

/**
 * Initializer with ErrorDetails
 * @param details the ErrorDetails used to create the MSTRException
 * @return an MSTRException
 */
-(MSTRException*)initWithErrorDetails:(ErrorDetails*)details;

/**
 * Get ErrorDetails from MSTRException
 */
-(ErrorDetails*)errorDetails;

/**
 * Send the string representation of MSTRError to standard output.
 */
-(void)log;

@end
