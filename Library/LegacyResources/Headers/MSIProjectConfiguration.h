//
//  ProjectConfiguration.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class WebServerConfiguration, MSIDevicePreferences, MSIProjectCredentials, ProjectConfigurationRootFolderSettings, MSIProjectInfo, ConnectivitySerializableContext, ProjectConfigurationScheduleSettings,
DefaultProjectProperties, StatisticsSettings;

/**
 * Connectivity information for a project. ProjectConfiguration can be read from preference file or created manually.
 * ProjectConfiguration contains all the information required by a project, such as I-server name, project name, credentials and so on.
 * After ProjectConfiguration has been added to the device preferences, when use the ProjectConfiguration, application should maintain a reference to the device preference.
 */
@interface MSIProjectConfiguration : NSObject <NSCopying, MSISerializable> {
@private
	NSString* projectName;
	NSString* serverName;
	NSInteger serverPort;
	WebServerConfiguration* __weak parentWebServer;
	BOOL usesDefaultCredentials;
	MSIDevicePreferences* __weak parent;
	BOOL credentialsInvalid;
	NSInteger credentialsInvalidReason;
	NSString* personalizationFileName;
	NSString* projectID;
	ProjectConfigurationRootFolderSettings* rootFolderSettings;
	ConnectivitySerializableContext* __weak csCxt;
	MSIProjectCredentials * credentials;
	BOOL allowSubscription;
	ProjectConfigurationScheduleSettings* subscriptionScheduleSettings;
	DefaultProjectProperties* defaultProjProps;
	NSString* webVersion;
	NSString* serverVersion;
	NSString* userID;
	BOOL logStats;
	StatisticsSettings *statsSettings;
	NSString* projectKey;
}

/**
 * Use this property to get or set the project name
 */
@property (nonatomic, copy) NSString* projectName;


/**
 * Use this property to get or set the i-server name. It can be a host name or ip address.
 */
@property (nonatomic, copy) NSString* serverName;


/**
 * Use this property to get or set the i-server port 
 */
@property (nonatomic)NSInteger serverPort;//lishan atomic property can not pair synthesized set/get with user defined setter/getter


/**
 * Use this property to get the parent web server.
 * I-Server should be connect to web server to be used in iOS client.
 */
@property (nonatomic, weak, readonly) WebServerConfiguration* parentWebServer;


/**
 * Use this property to get or set whether to use default project credentials
 * If yes, project will use the defaultProjectCredentials from WebServerConfiguration,
 * otherwise, project will use its own credentials.
 * @see WebServerConfiguration
 */
@property (nonatomic)BOOL usesDefaultCredentials;//lishan nonatomic


/**
 * Use this property to get the MSIDevicePreferences to which this ProjectConfiguration belongs
 */
@property (nonatomic, weak, readonly) MSIDevicePreferences * parent;


/**
 * Use this property to get the project ID
 * Project ID is in the configuration file and will be sent to client again after authentication with I-Server.
 * Project ID can be used to locate a ProjectConfiguration in device preferences.
 */
@property (nonatomic, strong, readonly) NSString* projectID;


/**
 * Use this property to get the root folder of this project.
 */
@property (nonatomic, strong, readonly) ProjectConfigurationRootFolderSettings* rootFolderSettings;


/**
 * Use this property to get the project credentials
 */
@property (nonatomic, strong, readonly) MSIProjectCredentials * credentials;


/**
 * Use this property to get the subscription schedulde settings
 */
@property (nonatomic, strong, readonly) ProjectConfigurationScheduleSettings* subscriptionScheduleSettings;


/**
 * Use this property to get or set whether to allow subsciption for the project
 */
@property (nonatomic)BOOL allowSubscription;//lishan nonatomic

/**
 * Use this property to get the i-server version
 */
@property (nonatomic, strong, readonly) NSString* serverVersion;

/**
 * Use this method to get a project info copy.
 * ProjectInfo is a subset of ProjectConfiguration and contains minimal information to identify a project.
 * @return ProjectInfo
 */

- (MSIProjectInfo*) copyProjectInfo;


/**
 * Use this method to get the auth mode that takes effective.
 * If project uses default credentials, the effective auth mode comes from default project credentials; 
 * otherwise, the effective auth mode comes from project credentials.
 * @return effective auth mode
 */
- (NSInteger) getEffectiveAuthMode;


/**
 * Use this method to compare ProjectInfo objects
 * @return whether the project objects are equal. For them to be equal, they must match these: webserver, projectName and iserver name.
 */
- (BOOL) isEqualToInfo:(MSIProjectInfo*)project;

@end