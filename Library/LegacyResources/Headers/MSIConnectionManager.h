//
//  MSIConnectionManager.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 4/8/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MSIConnectionManager : NSObject

/**
 * Get the connection to server.
 * @param connectionParameters parameters for the connection
 * @return the dictionary for the returned connection
 */
- (NSDictionary* ) getConnection:(NSDictionary*) connectionParameters;

/**
 * Get the connection information.
 * @param connectionParameters parameters for the connection
 * @return the dictionary for the returned information
 */
- (NSDictionary* ) getConnectionInformation:(NSDictionary*) connectionParameters;


/**
 * Create the connection to server.
 * @param connectionParameters parameters for the connection
 */
- (void) createConnection:(NSDictionary*) connectionParameters;


/**
 * Validate the connection to server.
 * @param connectionParameters parameters for validating the connection
 * @return the dictionary for the returned connection
 */
- (NSDictionary*) validateConnection:(NSDictionary*) connectionParameters;


/**
 * Invalidate the connection to server.
 * @param connectionParameters parameters for the connection
 */
- (void) invalidateConnection:(NSDictionary*) connectionParameters;

/**
 * Clear the connection to server.
 * @param connectionParameters parameters for the connection
 */
- (void) clearConnection:(NSDictionary*) connectionParameters;

@end
