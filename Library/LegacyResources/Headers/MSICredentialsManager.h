//
//  MSICredentialsManager.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 10/10/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WebServerInfo, MSIProjectInfo, WebServerConfiguration, WebServerCredentials;

@interface MSICredentialsManager : NSObject

/**
 * Set the credentials for mobile server default authentication
 * @param username set the login username
 * @param password set the password
 */
+(void)setDefaultServerCredentialsWithUsername:(NSString *) username password:(NSString *) password;

/**
 * Set the credentials for a mobile server authentication
 * @param server the mobile server
 * @param username set the login username
 * @param password set the password
 */
+(void)setServerCredentials:(WebServerInfo*)server username:(NSString *) username password:(NSString *) password;

/**
 * Set the credentials for project default authentication of a mobile server
 * @param server the mobile server
 * @param username set the login username
 * @param password set the password
 * @param project set the project, this parameter is mainly used for cleaning caches
 */
+(void)setDefaultProjectCredentialsForWebServer:(WebServerInfo*)server username:(NSString *) username password:(NSString *) password project:(MSIProjectInfo *)project;

/**
 * Set the credentials for project default authentication of a mobile server
 * @param server the mobile server
 * @param username set the login username
 * @param password set the password
 * @param parameters set the custom parameters
 * @param project set the project, this parameter is mainly used for cleaning caches
 */
+(void)setDefaultProjectCredentialsForWebServer:(WebServerInfo*)server username:(NSString *) username password:(NSString *) password customParameters:(NSDictionary *)parameters project:(MSIProjectInfo *)project;

/**
 * Set the credentials for a project authentication
 * @param project the project
 * @param username set the login username
 * @param password set the password
 * @param parameters set the custom parameters
 */
+(void)setProjectCredentials:(MSIProjectInfo *)project username:(NSString *) username password:(NSString *) password customParameters:(NSDictionary *)parameters;

/**
 * Set the credentials for a project authentication
 * @param project the project
 * @param username set the login username
 * @param password set the password
 */
+(void)setProjectCredentials:(MSIProjectInfo *)project username:(NSString *) username password:(NSString *) password;

/**
 * Delete the credentials for mobile server default authentication
 */
+(void)deleteDefaultServerCredentials;

/**
 * Delete the credentials for a mobile server authentication
 * @param server the mobile server
 */
+(void)deleteServerCredentials:(WebServerInfo*)server;

/**
 * Delete credentials for all mobile server authentications, including mobile server default authentication
 */
+(void)deleteAllServerCredentials;

/**
 * Delete the credentials for project default authentication of a mobile server
 * @param server the mobile server
 */
+(void)deleteDefaultProjectCredentialsForWebServer:(WebServerInfo*)server;

/**
 * Delete the credentials for a project authentication
 * @param project the project
 */
+(void)deleteProjectCredentials:(MSIProjectInfo *)project;

/**
 * Delete credentials for all project authentications of a mobile server, including project default authentication
 * @param server the mobile server
 */
+(void)deleteAllProjectCredentialsForWebServer:(WebServerInfo*)server;

/**
 * Delete credentials for all project authentications of all mobile servers, including project default authentication
 * @param server the mobile server
 */
+(void)deleteAllProjectCredentials;

/**
 * Delete all credentials, including all mobile server authentications and project authentications
 * @param server the mobile server
 */
+(void)deleteAllCredentials;

/**
 * Return the credentials for mobile server.
 * @return the credentials for the mobile server
 */
+ (WebServerCredentials *) credentialsForServer:(WebServerConfiguration *)webServer;

@end
