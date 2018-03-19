//
//  ProjectInfo.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class WebServerInfo;

/**
 * ProjectInfo is used for storing the information necessary to reach a project. It is stored as part of the WebServerInfo.
 */
@interface MSIProjectInfo : NSObject  <NSCopying, MSISerializable, NSCoding> {
@private
	NSString* projectName;
	NSString* serverName;
	NSInteger serverPort;
	WebServerInfo* parentWebServer;
}

/**
 * Use this property to get or set the project name
 */
@property (nonatomic, copy) NSString* projectName;

/**
 * Use this property to get or set the i-server name. It can be the host name or ip address.
 */
@property (nonatomic, copy) NSString* serverName;

/**
 * Use this property to get or set the i-server port 
 */
@property NSInteger serverPort;

/**
 * Use this property to get or set the parent web server
 */
@property (nonatomic, copy) WebServerInfo* parentWebServer;

/**
 * Use this method to get the default ProjectInfo
 */
+(MSIProjectInfo*)defaultProjectInfo;

/**
 * Use this method to get all available projects
 */
+(NSArray*)projectInfoList;

/**
 * Use this method to get the identifier. The identifier will concatenate projectName and serverName
 */
-(NSString*)projectIdentifier;

@end
