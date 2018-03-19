//
//  MSMCacheManager.h
//  MicroStrategyMobile
//
//  Created by Zhang, Yuan on 3/14/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIDocumentCache;
@class MSIProjectInfo;

typedef enum {
    MSMCachePriorityHigh = 0,
    MSMCachePriorityMedium,
    MSMCachePriorityLow
} MSMCachePriorities;

/**
 * This class is for SDK users to monitor and manage cache status
 **/
@interface MSICacheManager : NSObject

//Get the singleton object
+(MSICacheManager*) manager;

/**
 * This method is already deprecated, user should use the next method and set value prompt to nil if there is no value prompt
 */
-(void)retrieveCacheForObjectId:(NSString*)objectId withPriority:(MSMCachePriorities)priority withElementsPromptAnswers:(NSString*)elementsPromptAnswers andProjectInfo:(MSIProjectInfo*)projectInfo completionHandler:(void(^)(MSIDocumentCache* documentCache, NSError* error))handlerBlock;

/**
 * This method will retrieve cache at report/document and prompt answer level.
 * @param objectId is the id of the object you want to retrieve cache for.
 * @param withPriority is used to define the priority of current request.
 * @prarm projectInfo is used to specify project, if not provided, we will use default projectInfo
 * @param block is used when the MSMCache is retrieved.
 */
-(void)retrieveCacheForObjectId:(NSString*)objectId withPriority:(MSMCachePriorities)priority withElementsPromptAnswers:(NSString*)elementsPromptAnswers andValuePromptAnswers:(NSString*)valuePromptAnswers andProjectInfo:(MSIProjectInfo*)projectInfo completionHandler:(void(^)(MSIDocumentCache* documentCache, NSError* error))handlerBlock;

/**
 * This method will return the cache of certain key
 * @param key should be created by [NSString stringWithFormat:@"%@%@%@%@", projectIdentifier, objectId, elementsPromptAnswers, valuePromptAnswers];
 * ProjectInfo has an instance method called projectIdentifier, which will return a string as projectIdentifier
 * If the document does not contain prompt answer, we should provide nil as parameter: [NSString stringWithFormat:@"%@%@%@%@", projectIdentifier objectId, nil, nil];
 */
-(MSIDocumentCache*)cacheForKey:(NSString*)key;

//Delete all caches on the device
-(void)deleteAllCaches;

//Delete all caches before certain date
-(void)deleteAllCachesBeforeDate:(NSDate*)date;

//Delete a cache object for a document/report.
-(void)deleteCache:(MSIDocumentCache*)cache;

//Delete all caches for a document/report by providing the object Id and projectInfo.
-(void)deleteCachesForObjectId:(NSString*)objectId projectInfo:(MSIProjectInfo*)projectInfo;

@end
