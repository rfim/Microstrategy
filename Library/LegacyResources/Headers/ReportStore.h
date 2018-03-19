//
//  ReportStore.h
//  MSTR Foundation
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CacheInfo.h"

#define CACHE_SET_MAP_FILENAME @"CacheSetMappings"

@class MSIReportDisplayInfo, MSIProjectInfo, DerivedReportInfo, MSILiveReportInfo, MSIObjectInfo, IServerCacheInfo;
@class DataSetInfo;


//A small object through which we can get access to report info in the store.
//It shall be used for list of objects instead of copies of report info.
@interface ReportInfoHandle : NSObject {
	NSString* sid;
	NSString* name;
	NSInteger type;
	NSMutableDictionary* userData;
	NSString* projectID;
    NSString* objectID;
}

@property (nonatomic, copy) NSString* sid;
@property (nonatomic, copy) NSString* name;
@property NSInteger type;
@property (nonatomic, copy) NSString* projectID;
@property (nonatomic, copy) NSString* objectID;

+(ReportInfoHandle*) newHandle:(MSIReportDisplayInfo*)report;

-(void)setUserData:(id)data key:(NSString*)key;
-(id)userDataForKey:(NSString*)key;
-(NSComparisonResult)compareReportName:(ReportInfoHandle*)item;
-(NSComparisonResult)compareReportNameDiscending:(ReportInfoHandle*)item;

@end // ReportInfoHandle


/**
 ReportStore holds reports that have been received from the server and can then be accessed locally.
 */
@interface ReportStore : NSObject {
@private
	NSMutableArray* cacheList;
	NSMutableDictionary* reportMap;
	NSMutableDictionary* cacheSetIDMap;
	//NSMutableDictionary *revCacheSetIDMap;
	int cacheSize;
	int maxCacheSize;
	int softCachePercentage;
	CacheIDMap* defaultCacheIDToFilenameMap;
    
    NSMutableArray* lockedDocuments;
}


+(ReportStore*)getReportStore;
+ (void) resetReportStore;

-(NSDictionary *)getCacheSetKeys:(MSIReportDisplayInfo*)aReport;
-(NSDictionary *)getCacheSetKeysForLinkDrill:(MSIReportDisplayInfo*) aReport promptAnswer:(NSString*)promptAnswer;
-(NSInteger)addReport:(MSIReportDisplayInfo*)aReport error:(NSError**)error;
-(NSInteger)updateReport:(MSIReportDisplayInfo*)aReport deleteData:(bool)deleteData error:(NSError**)error;
-(NSInteger)deleteReport:(MSIReportDisplayInfo*)aReport error:(NSError**)error;
-(NSArray*)reportListCopy;
-(NSArray*)allReportsCopy;
-(NSArray*)reportListCopyForProject:(MSIProjectInfo*)project;
-(NSArray*)liveReportListCopyForProject:(MSIProjectInfo*)project;
-(NSArray*)DatasetListCopyForProject:(MSIProjectInfo*)project;

-(MSIReportDisplayInfo*)reportCopyBySubscriptionID:(NSString*)subscriptionID;
-(MSIReportDisplayInfo*) reportCopyByTargetID:(NSString*)targetID type:(NSInteger)targetType;

-(MSIReportDisplayInfo*)reportCopyBySubscriptionID:(NSString*)subscriptionID projectID:(NSString*)projectID;
-(MSIReportDisplayInfo*) reportCopyByTargetID:(NSString*)targetID type:(NSInteger)targetType projectID:(NSString*)projectID;
-(MSIReportDisplayInfo*) reportCopyByTargetID:(NSString*)targetID type:(NSInteger)targetType projectID:(NSString*)projectID promptAnswer:(NSString*)promptAnswer;
-(NSArray*) reportsCopyByTargetID:(NSString*)targetID type:(NSInteger)targetType projectID:(NSString*)projectID;

//DL;Async Execution;05/07/2012;support DeferExecLiveReportInfo copy
-(MSIReportDisplayInfo*)deferExecReportCopyBySubscriptionID:(NSString*)subscriptionID projectID:(NSString*)projectID;

-(NSInteger)addReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID data:(NSData*)data error:(NSError**)error;
-(NSInteger)addReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID moveFileAtLocation:(NSString*)fileName error:(NSError**) error;
-(NSInteger)addReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID data:(NSData *)data moveFileAtLocation:(NSString*)fileName withServerCache:(IServerCacheInfo *)isci error:(NSError**) error;
-(NSInteger)addReportCache:(MSIReportDisplayInfo*)aReport
                   cacheID:(NSString*)cacheID
                      data:(NSData *)data
        moveFileAtLocation:(NSString*)fileName
           withServerCache:(IServerCacheInfo *)isci
              isBackground:(BOOL)background
                     error:(NSError**) error;

-(NSData*)getReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID;
-(NSInteger)moveReportCache:(MSIReportDisplayInfo *)aReport oldCacheID:(NSString *)oldCacheID newCacheID:(NSString *)newCacheID error:(NSError **)error;
-(NSInteger)deleteReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID error:(NSError**)error;
-(BOOL)reportCacheExists:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID;
//Dataset cache check
-(BOOL)datasetCacheExists:(DataSetInfo*)aDatasetInfo;

-(void)deleteDrillCaches;

+(NSString*)reportsRootPath;
-(NSString*)getPath;
-(void)initializeCacheSetMap;

-(void)moveCacheSet:(NSString*)setID forReport:(MSIReportDisplayInfo*)aReport;
-(void)copyCacheSet:(NSString*)setID forReport:(MSIReportDisplayInfo*)aReport;

-(MSILiveReportInfo*)newLiveReport:(MSIObjectInfo*)object projectInfo:(MSIProjectInfo*)projectInfo;
-(MSILiveReportInfo*)newLiveReport:(NSString*)objID type:(NSInteger)type projectInfo:(MSIProjectInfo*)projectInfo;
-(MSILiveReportInfo*) liveReportCopyByTargetID:(NSString*)targetID type:(NSInteger)targetType project:(MSIProjectInfo*)projectInfo;
-(DerivedReportInfo*) newDerivedReport:(MSIReportDisplayInfo*)baseReport;

-(void)setFullyCached:(BOOL)value bytesLoaded:(NSInteger)bytesLoaded toReport:(MSIReportDisplayInfo*)report;
-(void)setFullyCached:(BOOL)value bytesLoaded:(NSInteger)bytesLoaded updateTime:(BOOL)updateTime toReport:(MSIReportDisplayInfo *)report;
-(BOOL)isFullyCached:(NSString*)subscriptionID projectID:(NSString*)projectID;
-(void)setPreloadingStateForReport:(MSIReportDisplayInfo*)report  layoutID:(NSInteger)layoutID sliceCount:(NSInteger)sliceCount;

-(NSMutableArray*)newUncachedReportsList;
-(BOOL)hasReportCache:(MSIReportDisplayInfo *)aReport cacheID:(NSString *)cacheID;
-(void) setInitialPageForReport:(MSIReportDisplayInfo*)report identifier:(NSString*) identifier values:(NSArray*) values;
-(BOOL)isSubscriptionReport:(MSIReportDisplayInfo*)report;

//+(NSString*)debugCacheSetName:(MSIReportDisplayInfo*)report;

+(void)performUpgradeCleanup;

-(NSInteger)deleteProjectCaches:(MSIProjectInfo*)project error:(NSError**)error;

/**
 When debugging reports, It can be useful to have them start fresh, with no cached info, prompt answers, etc. Use this method to clear out all the cahced info.
 */
+(void)dropAllReportCaches:(BOOL)clearLiveReportInfo clearSubscribedReportInfo:(BOOL)clearSubscribedReportInfo;

//I.B. Made it public to fix TQMS 412760
-(NSInteger)cacheSetSizeForReport:(MSIReportDisplayInfo*)report;

-(NSInteger)invalidateReportCacheSet:(NSString*)cacheSetID cacheRootID:(NSString*)cacheRootID project:(MSIProjectInfo*)project error:(NSError**)error;
-(NSInteger)invalidateReportCache:(NSString*)serverCacheID cacheSet:(NSString*)cacheSetID cacheRootID:(NSString*)cacheRootID project:(MSIProjectInfo*)project error:(NSError**)error;

-(NSInteger)deleteReportCacheSet:(NSString*)cacheSetID cacheRootID:(NSString*)cacheRootID project:(MSIProjectInfo*)project status:(int)status error:(NSError**)error;
-(NSInteger)deleteReportCacheSet:(NSString*)cacheSetID cacheRootID:(NSString*)cacheRootID project:(MSIProjectInfo*)project status:(int)status isBackground:(BOOL)background error:(NSError**)error;
-(NSData*)getReportCache:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID status:(NSInteger*)oStatus;
-(NSInteger)updateReport:(MSIReportDisplayInfo*)aReport dataAction:(NSInteger)action error:(NSError**)error;
-(NSString*)getReportCachePath:(MSIReportDisplayInfo*)aReport cacheID:(NSString*)cacheID status:(NSInteger*)oStatus;

/*
 * return updated version of report cache patch, if there is no updated version, return nil
 *
 * @param document The document info
 * @param cacheID The cache ID
 * @param oStatus The cache status returned
 * @param isUpdated If we are asking for updated version
 * @return Cache path on device
 */
-(NSString*)getReportCachePath:(MSIReportDisplayInfo*)document cacheID:(NSString*)cacheID status:(NSInteger*)oStatus updated:(BOOL)isUpdated;

/*
 * When set to clear on close, the report store will be emptied when the app is moved to the background. This behavior can be chosen as an extra measure of security.
 */
+(BOOL)clearOnClose;

-(NSInteger)invalidateReportCache:(MSIReportDisplayInfo*)aReport error:(NSError**)error;

/*
 * set the used flag for a document, this is so that the document binary in use will not be overwritten by other activities
 *
 * @param document The document info
 * @param locked Whether the document is being locked
 */
-(void)setDocument:(MSIReportDisplayInfo*)document locked:(BOOL)locked;

/*
 * return the array of lockedDocuments
 *
 * @return Array of lockedDocuments
 */
-(NSArray*)getLockedDocuments;

/*
 * return if the cache ID is predefined - prompt answer, server cache info, delta map
 *
 * @param cacheID cache ID
 * @return If the cacheID is predefined
 */
+(BOOL)isPredefinedCacheID:(NSString*)cacheID;

/*
 * update prompt question info and persist
 *
 * @param aReport a report
 * @param data the prompt question xml
 */
-(void)handlePromptQuestionUpdate:(MSIReportDisplayInfo*)aReport data:(NSData*)data;

/*
 * return the mapped cacheset ID
 *
 * @param report the report to get cacheset ID
 * @return the mapped cacheset ID
 */
-(NSString *)getMappedCacheSetID:(MSIReportDisplayInfo*)aReport;

@end
