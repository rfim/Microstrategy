//
//  MSICredentials.h
//  MSTRMobileSDK
//
//  Created by jacky xu on 9/29/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"
#import "MSIPreference.h"

/**
 * Credentials class represents login, password and other related information used for authentication.
 */
@interface MSICredentials : NSObject <NSCopying, MSISerializable> {
@protected
	NSInteger authMode;
	NSString* login;
	NSString* password;
	NSString* hashValue;
	NSTimeInterval passwordStartTime;
	BOOL changedByUser;
	
	__weak MSIDevicePreferences * parent;
}

/**
 * Use this property to get or set authentication mode used to connecting to web server or I-server.
 
 * @see Enums for the options
 * @see AuthModes
 */
@property (nonatomic)NSInteger authMode;

/**
 * Use this property to get or set the login username.
 */
@property (nonatomic, copy) NSString* login;

/**
 * Use this property to get ot set password.
 */
@property (nonatomic, copy) NSString* password;

/**
 * Initializer with a default authentication mode
 * @param anAuthMode authentication mode
 * @return a new Credentials object
 * @see Enums for the options
 * @see AuthModes
 */
- (id) initWithAuthMode:(NSInteger)anAuthMode;

@end
