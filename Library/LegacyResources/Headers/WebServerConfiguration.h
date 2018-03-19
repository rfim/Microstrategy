//
//  WebServerConfiguration.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class MSIProjectList, MSIDevicePreferences, ConnectivitySerializableContext, WebServerInfo,WebServerCredentials, MSIProjectCredentials;

/**
 * WebServerConfiguration contains the connectivity information for a web server and projects connecting to this web server. 
 * It can be read from preference file or created manually.
 * After WebServerConfiguration has been added to the device preferences, when use the WebServerConfiguration, application should maintain a reference to the device preference.
 */
@interface WebServerConfiguration : NSObject <MSISerializable, NSCopying> {
@private
	NSString* name;
	NSInteger port;
	NSString* path;
	NSInteger requestType;
	NSInteger type;
	BOOL usesDefaultCredentials;
	MSIProjectList* projects;
	MSIDevicePreferences * __weak parent;
	BOOL credentialsInvalid;
	NSInteger credentialsInvalidReason;
	ConnectivitySerializableContext* __weak csCxt;
	WebServerCredentials* credentials;
	MSIProjectCredentials * defaultProjectCredentials;
	NSInteger webServerType;
    NSString *formAuthErrorNetworkID;
}

/**
 * Use this property to get or set name of this web server. It can be host name or ip address.
 */
@property (nonatomic, copy) NSString* name;

/**
 * Use this property to get or set port of this web server
 */
@property (nonatomic)NSInteger port;//lishan nonatomic

/**
 * Use this property to get or set path of this web server.
 * It's the remainder of the URL, past the host and port. On a typical J2EE server, it is often "MicroStrategyMobile".
 */
@property (nonatomic, copy) NSString* path;

/**
 * Use this property to get or set request protocol with this web server
 * It can be either HTTP or HTTPS.
 * @see RequestType
 */
@property (nonatomic)NSInteger requestType;//lishan nonatomic

/**
 * Use this property to get or set server type of the web server
 * It can be either an ASP server or a JSP server.
 * @see WebServerTypes
 */
@property (nonatomic)NSInteger type;//lishan nonatomic

/**
 * Use this property to get or set whether to use default web server credentials in the WebServerList
 * If it is YES, WebServerConfiguration will use the defaultWebServerCredentials in WebServerList;
 * otherwise, it will use the Credentials of itself.
 * @see WebServerList
 */
@property (nonatomic)BOOL usesDefaultCredentials;//lishan nonatomic

/**
 * Use this property to get or set list of projects connecting to this web server
 */
@property (nonatomic, strong, readonly) MSIProjectList* projects;

/**
 * Use this property to get to which this WebServerConfiguration belongs.
 */
@property (nonatomic, weak, readonly) MSIDevicePreferences * parent;

/**
 * Use this property to get web server credentials.
 */
@property (nonatomic, strong, readonly) WebServerCredentials* credentials;

/**
 * Use this property to get default project credentials.
 * This default project credentials can be used by all the projects connecting to this web server.
 */
@property (nonatomic, strong, readonly) MSIProjectCredentials * defaultProjectCredentials;

/**
 * Copy the WebServerInfo from this WebServerConfiguration.
 * WebServerInfo contains part of the information in WebServerConfiguration.
 * @return WebServerInfo copied from this WebServerConfiguration
 */
-(WebServerInfo*)copyWebServerInfo;

/**
 * Check whether the WebServerInfo represents the same web server as this WebServeConfiguration.
 * @return YES if represents the same web server, otherwise NO.
 */
- (BOOL) isEqualToInfo:(WebServerInfo*)server;

@end
