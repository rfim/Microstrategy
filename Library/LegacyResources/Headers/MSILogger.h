//
//  MSILogger.h
//  MSTRMobile
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"


@class Handler;

/**
 MSILogger is used for comprehensive logging into the MicroStrategy log. The records contain detailed fields. MSILogger also provides guidance for filtering entries.
 
 @see LogLevels
*/
 
 
@interface MSILogger : NSObject {
	@private
	NSMutableArray *handlers;
	LogLevels		level;
}

/**
 logWithLevel:className:methodName:message: makes the actual log entry. LogLevels is used for guidance on whether to log something, but is not enforced. For example,
 
	 if ( [[MSILog getLogger] isLoggable: Severe ] )
	 {
		 [[MSILog getLogger] logWithLevel: Severe
							 className: @"ApplicationLauncherController"
							methodName: @"startLaunch"
							   message: @"The URL is not valid." ];
	 }
 
 @see LogLevels
 */
- (void)logWithLevel: (LogLevels) level className:(NSString *)className methodName:(NSString *)method message:(NSString *)msg;

/**
 logWithLevel:className:methodName:message:module: makes the actual log entry. LogLevels is used for guidance on whether to log something, but is not enforced. For example,
 
    if ( [[MSILog getLogger] isLoggable: Severe ] )
    {
        [[MSILog getLogger] logWithLevel: Severe
                               className: @"ApplicationLauncherController"
                              methodName: @"startLaunch"
                                 message: @"The URL is not valid."
                                  module: @"ApplicationLauncher"];
    }

 @see LogLevels
 */
- (void)logWithLevel: (LogLevels) level className:(NSString *)className methodName:(NSString *)method message:(NSString *)msg module:(NSString *) module;

/**
 isLoggable: provides guidance on whether a proposed level is important enough to be logged, given the current MSILogger logging level.
 
 @see LogLevels

 */
- (BOOL) isLoggable: (LogLevels) level;


// for internal use.
- (void)addHandler:(Handler *)handler;
- (BOOL)imageCachePerformanceLogEnabled;

@property LogLevels level;

@end
