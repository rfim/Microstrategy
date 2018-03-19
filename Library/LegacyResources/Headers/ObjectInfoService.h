//
//  ObjectInfoService.h
//  MSTRMobile
//
//  Created by Amirali Charania on 4/5/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo;
@class MSIObjectInfo;

/**
 * ObjectInfoService is used to get a list of ObjectInfos from server side.
 * Caller of this service needs to provide objectTypes, objectIDs and the project where these objects is in.
 * Each Element of objectTypes applies to the element of objectIDs of the same index. 
 * For example, the first object from objectTypes applies to the first object from objectIDs, etc.
 */
@interface ObjectInfoService : MSIServiceImpl {
	NSMutableArray* objectTypes;
	NSMutableArray* objectIDs;
	MSIProjectInfo* project;
	NSMutableArray* objects;
    
    MSIObjectInfo* currentParsingObject;
    bool includeParentFolder;
}

/**
 * types of objects to be retrieved
 */
@property (nonatomic, strong) NSMutableArray* objectTypes;

/**
 * IDs of objects to be retrieved
 */
@property (nonatomic, strong) NSMutableArray* objectIDs;

/**
 * Project and Intelligence Server info about where these objects reside.
 */
@property (nonatomic, strong) MSIProjectInfo* project;


@property (nonatomic, assign) bool includeParentFolder;

@end
