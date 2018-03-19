//
//  CacheInfo.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"
#import "MSTRHandler.h"
#import "SAXSupport.h"

#define DRILL_CACHE_ROOT_ID @"Drill"

@class CacheIDMap;

@interface CacheInfo : NSObject <MSISerializable, MSTRHandler>{
@private
	NSInteger size;
	NSString* subscriptionID;
	NSString *cacheSetID;
	NSString* cacheID;
	NSDate* lastTimeRead; // lazily cached
	NSInteger status;
	NSString* projectID;
    NSDate* creationTime;
}

@property NSInteger size;
@property (nonatomic, copy) NSString *subscriptionID;
@property (nonatomic, copy) NSString *cacheSetID;
@property (nonatomic, copy) NSString *cacheID;
@property NSInteger status;
@property (nonatomic, copy) NSString *projectID;
@property (nonatomic, copy) NSDate *creationTime;
-(NSString*)cacheSetPath;
-(NSString*)cachePath:(CacheIDMap*) cacheIDMap;
-(NSString*)cachePath:(CacheIDMap*) cacheIDMap createDirectory:(BOOL)create;
-(NSString*)cachePath:(CacheIDMap*) cacheIDMap createDirectory:(BOOL)create locked:(BOOL)locked;
-(NSString*)cachePath:(CacheIDMap*) cacheIDMap inDirectory:(NSString*)cacheDir createDirectory:(BOOL)create;
-(NSString*)cachePath:(CacheIDMap*) cacheIDMap inDirectory:(NSString*)cacheDir createDirectory:(BOOL)create locked:(BOOL)locked;

-(BOOL)cacheExistsOnDisk:(CacheIDMap*) cacheIDMap;
-(BOOL)isSpecial;
-(NSDate*) lastTimeRead:(CacheIDMap*)ownerMap;
-(void) updateLastTimeRead:(CacheIDMap*) ownerMap;
-(void)populate:(NSData*) data;
-(NSString*)getXML;
-(void) initializeCreationTime:(CacheIDMap*) ownerMap;
/*
 * Move contents from updated folder to main report cache folder, delete the updated folder
 */
-(void) mergeDataFromUpdatedFolder;

@end
