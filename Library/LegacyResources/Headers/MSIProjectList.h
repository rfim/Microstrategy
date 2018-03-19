//
//  ProjectList.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class MSIProjectConfiguration, WebServerConfiguration, MSIDevicePreferences, ConnectivitySerializableContext;

/**
 * Collection of all the projects connecting a web server. It supports collection operations for projects, such as add, remove and size.
 */
@interface MSIProjectList : NSObject <MSISerializable>{
@private
	NSMutableArray* projects;
	WebServerConfiguration* __weak parentWebServer;
	MSIDevicePreferences * __weak parent;
	ConnectivitySerializableContext* __weak csCxt;
}

/**
 * Use this property to get the parent web server all the projects connecting to.
 */
@property (nonatomic, weak, readonly) WebServerConfiguration* parentWebServer;

/**
 * Use this method to create a new project configuration
 * @return project configuration
 */
+(MSIProjectConfiguration *)newProjectConfiguration;


/**  
 * Use this method to add project to the configuration. If project is nil, an NSInvalidArgumentException is thrown.
 * @param projectConfiguration new projectConfiguration
 * @throws NSInvalidArgumentException if project is nil
 */
- (void) add: (MSIProjectConfiguration*) project;


/**
 * Use this method to get project at particular index
 * @param index index of the project to be fetched. If the index is out of range, an NSRangeException is thrown.
 * @return projectConfiguration
 * @throws NSRangeException if the index is out of range
*/
- (MSIProjectConfiguration*) getProject: (NSInteger) index;


- (MSIProjectConfiguration*) getProjectByProjectID: (NSString*) projectID;

/**
 * Use this method to remove project
 * @param index index of the project to be removed. If the index is out of range, an NSRangeException is thrown.
 * @throws NSRangeException if the index is out of range
 */
- (void) remove: (NSInteger) index;

/** 
 * Use this method to get the count of the projects
 * @return project count
 */
-(NSUInteger)size;

/**  
 * Use this method to get copy of the project list.
 * @param zone zone
 * @return project copy
 */
- (id)copyWithZone:(NSZone *)zone;

@end
