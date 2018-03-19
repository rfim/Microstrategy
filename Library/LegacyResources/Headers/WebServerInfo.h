//
//  WebServerInfo.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

/**
 * WebServerInfo holds the server name, port, path and type. It also performs conversions supporting disk I/O.
 * It is the minimal information needed to represent a web server.
 * It is typically used as a index to get the WebServerConfiguration from device preferences.
 */

@interface WebServerInfo : NSObject <MSISerializable, NSCopying, NSCoding> {
@protected
	NSString* name;
	NSInteger port;
	NSString* path;
	NSInteger webServerType;
}

/**
 * Use this property to get or set the webServer name. This can be a domain name or IP address.
 */
@property (nonatomic, copy) NSString* name;

/**
 * Use this property to get or set the webServer port
 */
@property (nonatomic) NSInteger port;

/**
 * Use this property to get or set the webServer paths. The remainder of the URL, past the host and port. On a typical J2EE server, it is often "MicroStrategyMobile".
 */
@property (nonatomic, copy) NSString* path;

/**
 * Use this method to get the default WebServerInfo
 */
+(WebServerInfo*)defaultWebServerInfo;

/**
 * Use this method to get all available webServer
 */
+(NSArray*)webServerInfoList;

@end
