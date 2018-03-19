//
//  WebServerList.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WebServerConfiguration, WebServerCredentials,WebServerInfo, MSIDevicePreferences, MSIProjectConfiguration, MSIProjectInfo, ConnectivitySerializableContext, FacebookCredentials;

/**
 * A WebServerList holds all of the connection info for the web servers and their projects.
 * This info is often set from a ConnectionInfo.plist file. It can be set programmatically as well.
 */
@interface WebServerList : NSObject {
@private
	NSMutableArray* webServers;
	MSIDevicePreferences* __weak parent;
	NSString* prefID;
	ConnectivitySerializableContext* __weak csCxt;
	WebServerCredentials* defaultWebServerCredentials;
	
	//Used in load from Config XML: won't apply if no web servers/projects exist
	WebServerCredentials* tempDefaultWebServerCredentials;
	
	NSInteger mode;//for parsing
}

/**
 * Use this property to get the default webserver credentials.
 * It can be used by all the web servers in the preferences.
 */
@property (nonatomic, strong, readonly) WebServerCredentials* defaultWebServerCredentials;

/**
 * Use this method to get new web server configuration
 * @return web server configuration
 */
+(WebServerConfiguration *)newWebServerConfiguration;

/** 
 * Use this method to add new webserver to the configuration.
 * @param webServer new webServer
 * @throws NSInvalidArgumentException if the webserver is nil
 */
-(void)add:(WebServerConfiguration*)webserver;

/**  
 * Use this method to get the webServer at index. If the index is < 0 or beyond the last webserver, an exception is thrown.
 * @param index index of the webserver needed
 * @return webServer at index
 * @throws NSRangeException if the index is out of range
 */
-(WebServerConfiguration*)getWebServer:(NSInteger)index;

/**  
 * Use this method to remove webserver
 * @param index index of the webserver to be removed. If the index is < 0 or beyond the last webserver, an exception is thrown.
 * @throws NSRangeException if index is out of range
 */
-(void)remove:(NSInteger)index;

/** 
 * Use this method to get the count of the webservers
 * @return webServer count
 */
-(NSUInteger)size;

/** 
 * Use this method to get the index of particular webServer. If it is not found, returns NSNotFound.
 * @param webserver webServer
 * @return index of webServer
 */
-(NSInteger)indexOfWebServer:(WebServerConfiguration*)webserver;

/**
 * Use this method to get the ProjectConfiguration from the ProjectInfo object.  If it is not found, returns nil.
 */
-(MSIProjectConfiguration*)getProjectConfiguration:(MSIProjectInfo*)project;

/** 
 * Use this method to get the ProjectConfiguration from the ProjectID.  If it is not found, returns nil.
 */
-(MSIProjectConfiguration*)getProjectConfigurationByProjectID:(NSString*) projectID;

/**
 * Use this method to get the ProjectConfiguration from the ProjectName.  If it is not found, returns nil.
 */
-(MSIProjectConfiguration*)getProjectConfigurationByProjectName:(NSString*) projectName;

/**
 * Use this method to get the WebServerConfiguration from the WebServerInfo object.  If it is not found, returns nil.
 */
-(WebServerConfiguration*)getWebServerConfiguration:(WebServerInfo*)server;

/**  
 * Use this method to remove all the webservers
 */
-(void)clear;


/** 
 * Use this method to check whether any projects are available
 */
-(BOOL) hasProjects;

@end
