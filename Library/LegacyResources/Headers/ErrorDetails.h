//
//  ErrorDetails.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIProjectInfo;

#define REPORT_NOT_FOUND -1
#define ERROR_NOT_CACHED -2
#define ERROR_CODE_INBOX_FAILURE 22 // TBD: fix to correct error code

#define ERROR_DETAILS_KEY @"errorDetails"

/**
 * ErrorDetails contains all the error information when connecting to a project.
 */
@interface ErrorDetails : NSObject {
	MSIProjectInfo* project;
	NSString* src;
	NSInteger statusCode;
	NSInteger errorCode;
	NSString* errorMsg;
}

/**
 * Use this property to get or set where this error comes from.
 */
@property (nonatomic, copy) NSString* src;

/**
 * Use this property to get or set the error status code from the HTTP response.
 */
@property NSInteger statusCode;

/**
 * Use this property to get or set the error code from server response.
 */
@property NSInteger errorCode;

/**
 * Use this property to get or set the error message from HTTP response or server response.
 */
@property (nonatomic, copy) NSString* errorMsg;

/**
 * Use this property to get or set the project
 */
@property (nonatomic, strong) MSIProjectInfo* project;

/**
 * Initialize
 *
 * @return self
 */
-(id)init;

/**
 * Return string representation for log purpose
 *
 * @return String contain all the variables
 */
-(NSString*)log;

@end
