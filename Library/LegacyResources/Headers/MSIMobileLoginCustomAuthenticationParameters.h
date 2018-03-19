//
//  MSIMobileLoginCustomAuthenticationParameters.h
//  MicroStrategyMobile
//
//  Created by Wang Wenyin on 12/2/14.
//  Copyright 2014 MicroStrategy Inc. All rights reserved.
//

#define ID_SERVER_PARAMETERS @"svp"

#import <Foundation/Foundation.h>
#import "MSIPreference.h"

@class MSIDevicePreferences;

@interface MSIMobileLoginCustomAuthenticationParameters : NSObject <MSIPreference> {
@private
	NSString* prefID;
	MSIDevicePreferences* __weak parent;
	
    NSMutableDictionary* serverParameters;
}

@property (nonatomic, copy) NSString* prefID;

/**
 * set extra parameter used by server login task
 * @param aValue parameter value
 * @param aKey parameter key
 */
-(void)setServerParam:(NSString*)aValue forKey:(NSString*)aKey;

/**
 * Get all keys in the server parameters dictionary.
 * @return all keys
 */
-(NSArray *)getServerParamKeys;

/**
 * Get a server parameter value for a key
 * @param aKey key
 * @return value
 */
-(NSString*)getServerParamForKey:(NSString*)aKey;

/**
 * Remove a server parameter value for a key
 * @param aKey key
 */
-(void)removeServerParamForKey:(NSString*)aKey;

/**
 * Remove all server parameters
 */
-(void)clearServerParams;

@end
