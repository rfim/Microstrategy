//
//  URLLaunchHelper.h
//  MSTRMobileSDK
//
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIProjectInfo;

@interface URLLaunchHelper : NSObject {
}

/*!
 Load application using parameters specified in dictionary. 
 */
+ (void) loadApplication:(NSDictionary*) dict parentController:(UIViewController*)parent;

/**
 Create a new ProjectInfo from serverName and projectName.Now there can be four cases 
 
 1.If both server name and project name are provided, then the project with the matching projectName and serverName
 will be returned if one exists.If such a project cannot be found a nil value will be returned.
 
 2.If only the server name is provided(i.e projectName is nil), then the first project in the server matching the serverName will
 be returned.If no such server exists or such a server does not have any projects then nil will be returned.
 
 3.If only the project name is provided(i.e. serverName is nil), then all the servers will be searched for a project with 
 name matching projectName.If such a project exists then it will be returned.
 
 4.If neither server name or project name is provided, we will return the first project in the first server if one exists.
 */
+ (MSIProjectInfo*) newProjectInfoWithServerName:(NSString*)server projectName:(NSString*)project;

/**
 Create a new ProjectInfo from serverName and projectName/projectID
 */
+ (MSIProjectInfo*) newProjectInfoWithServerName:(NSString*)server projectName:(NSString*)project projectID:(NSString*)projectID;

+ (MSIProjectInfo*) newProjectInfo:(NSString*)webServer iServer:(NSString*) iserver projectName:(NSString*)project;

+ (void)handleEventWithParameters:(NSDictionary*)parameters 
		   OnNavigationController:(UINavigationController*)navigationController
					  projectInfo:(MSIProjectInfo*)projectInfo;

@end
