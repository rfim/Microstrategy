//
//  MSTRError.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MSTR_DOMAIN @"MSTRDomain"
#define WRAPPED_SYSTEM_ERROR -1

@class ErrorDetails, MSTRException;

/**
 * MSTRError is the generic class to replace an error in MicroStrategyMobileSDK.
*/
@interface MSTRError : NSError {

}

/**
 * Use this property to get the ErrorDetails object from MSTRError.
 */
@property (weak, nonatomic, readonly) ErrorDetails* errorDetails;

/**
 * Initializer with ErrorDetails
 * @param details the ErrorDetails object used to create th MSTRError
 * @return an MSTRError object
 */
-(MSTRError*)initWithErrorDetails:(ErrorDetails*)details;

/**
 * Initializer with NSError.
 * @param originalError the original NSError used to create the MSTRError
 * @return an MSTRError object
 */
-(MSTRError*)initWithNSError:(NSError*)originalError;

/**
 * Initializer with MSTRException.
 * @param exception the MSTRException used to create the MSTRError
 * @return an MSTRError object
 */
-(MSTRError*)initWithException:(MSTRException*)exception;

/**
 * Static way to create a MSTRError with ErrorDetails.
 * @param anErrorDetails the ErrorDetails used to create the MSTRError
 * @return an MSTRError object
 */
+(MSTRError*)errorWithErrorDetails:(ErrorDetails*)anErrorDetails;

/**
 * Static way to create a MSTRError with NSError.
 * @param originalError the original NSError used to create the MSTRError
 * @return an MSTRError object
 */
+(MSTRError*)errorWithNSError:(NSError*) originalError;

/**
 * Static way to create a MSTRError with MSTRException.
 * @param exception the MSTRException used to create the MSTRError
 * @return an MSTRError object
 */
+(MSTRError*)errorWithException:(MSTRException*)exception;

/**
 * Send the string representation of MSTRError to standard output.
 */
-(void)log;
		
@end
