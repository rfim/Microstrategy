//
//  MSISessionConnectionManager.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 4/8/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIConnectionManager.h"
#import "MSIProjectInfo.h"
#import "WebServerInfo.h"
#import "MSIProjectConfiguration.h"
#import "Service.h"
#import "MSIServiceImpl.h"

/**
 * SessionStateWrapper wraps the session state, the timestamp when the session state is accessed last time and whether the session state has mobile Read Write privilege.
 */
@interface SessionStateWrapper : NSObject

/**
 * Initialize with the session state and set the timestamp as the current time
 * @param sessionState session state
 * @return self
 */
- (id) initWithSessionState:(NSString*)sessionState;

/**
 * session state
 */
@property (copy, readonly) NSString* sessionState;

/**
 * timestamp when the session state is last accessed
 * Default is current time.
 */
@property (copy) NSDate* timeStamp;

/**
 * whether the session state has mobile Read Write privilege
 * Default is NO.
 */
@property BOOL hasMobileRWPrivilege;

/**
 * return YES if sessionState.length == 0; only used as placeholder in this case
 */
- (BOOL)isDummyWrapper;

@end // SessionStateWrapper

#define MSI_AUTHENTICATION_PROJECT_CONFIGURATION_KEY @"projectConfiguration"
#define MSI_AUTHENTICATION_SUPRESS_NETWORK_ERROR_ALERTS_KEY @"suppressNetworkErrorAlerts"
#define MSI_AUTHENTICATION_IGNORE_ACCESS_PROJECT_ERROR_KEY @"ignoreAccessProjectError"
#define MSI_AUTHENTICATION_SHOW_CREDENTIALS_PROMPT_KEY @"showCredentialsPrompt"
#define MSI_AUTHENTICATION_PARENT_SERVICE_KEY @"parentService"
#define MSI_AUTHENTICATION_TYPE_KEY @"type"
#define MSI_AUTHENTICATION_SESSION_TABLE_KEY @"sessionTable"
#define MSI_AUTHENTICATION_CONNECTED_SERVER_KEY @"connectedServers"
#define MSI_AUTHENTICATION_SESSION_LIST_FOR_SERVER_KEY @"sessionListForServer"
#define MSI_AUTHENTICATION_WEB_SERVER_INFO_KEY @"webServerInfo"
#define MSI_AUTHENTICATION_SESSION_STATE_WRAPPER_FOR_PROJECT_KEY @"sessionStateWrapperForProject"
#define MSI_AUTHENTICATION_SESSION_STATE_WRAPPER_LIST_FOR_SERVER_KEY @"sessionStateWrapperListForServer"
#define MSI_AUTHENTICATION_SESSION_STATE_KEY @"sessionState"
#define MSI_AUTHENTICATION_HAS_MOBILE_RW_PRIVILEGE_KEY @"hasMobileRWPrivilege"
#define MSI_AUTHENTICATION_PROJECT_CONFIG_KEY @"projectConfig"
#define MSI_AUTHENTICATION_SHOULD_NOT_PUT_SESSION_STATE_INTO_TABLE_KEY @"shouldNotPutSessionStateIntoTable"
#define MSI_AUTHENTICATION_SHOULD_NOT_UPDATE_PREFERENCE_STORE_WHEN_USING_PROJECT_CONFIG_KEY @"shouldUpdatePreferenceStoreWhenUsingProjectConfig"
#define MSI_AUTHENTICATION_VALIDATE_SUCCESS_KEY @"validateSuccess"
#define MSI_AUTHENTICATION_CAN_BE_CANCELLED_KEY @"canBeCancelled"
#define MSI_AUTHENTICATION_ONLY_MOBILE_SERVER_ERROR_KEY @"onlyMobileServerError"
#define MSI_AUTHENTICATION_CREDENTIALS_PROMPT_RETURN_VALUE_KEY @"credentialsPromptReturnValue"
#define MSI_AUTHENTICATION_SESSION_STATE_WRAPPERS_KEY @"sessionStateWrappers"

#define MSI_SESSION_SERVICE_FAILED_NOTIFICATION @"sessionServiceFailed"
@interface MSISessionConnectionManager : MSIConnectionManager

/**
 * Get whether mobile read write privilege is granted for the project in the active session.
 * If there is no active session created for this project, it returns YES.
 * @param project project to be used to find a session
 * @return whether the current active project session has mobile read write privilege
 */
-(BOOL)hasMobileRWPrivilege:(MSIProjectInfo*)project;

/**
 * get a copy of the section table.
 * @return Returns a copy of the section table
 */
-(NSDictionary*)getSessionTable;

/**
 * Get the list of web servers that the client is currently connected with.
 * @return list of WebServerInfo
 * @see WebServerInfo
 */
-(NSArray*)connectedServers;

/**
 * Get all the session states created from a web server
 * @param webServer web server to be used to looked for session states
 * @return list of session state strings
 * @see WebServerInfo
 */
-(NSArray*)sessionListForServer:(WebServerInfo*)webServer;

/**
 * Get  the session state wrapper created from a project
 * @param project info to be used to looked for session state wrapper
 * @return SessionStateWrapper of the project
 * @see ProjectInfo
 */
-(SessionStateWrapper*)sessionStateWrapperForProject:(MSIProjectInfo*)project;

/**
 * Get all the session state wrappers created from a web server
 * @param webServer web server to be used to looked for session state wrapper
 * @return list of session state strings
 * @see WebServerInfo
 */
-(NSArray*)sessionStateWrapperListForServer:(WebServerInfo*)webServer;


@end
