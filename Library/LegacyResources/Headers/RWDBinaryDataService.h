//
//  RWDBinaryDataService.h
//  MSTRMobile
//
//  Created by Ji Jin on 4/16/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RWBinaryService.h"
#import "ReconcileService.h"
#import "DeferExecCommon.h"

#define PORTRAIT_ORIENTATION 1
#define LANDSCAPE_ORIENTATION 2

@interface RWDBinaryDataService : RWBinaryService <ReconcileServiceHandler, DeferExecCandidateReportStatusListener> {

}

@property (nonatomic, copy) NSString* cacheIDUsed;
@property BOOL cacheInvalid;
@property (nonatomic, strong) NSArray* desiredElements;
@property BOOL forceUseDeviceCache;
@property BOOL fullBinaryDownloaded;
@property (nonatomic, strong) NSArray* groupByElements;
@property BOOL hitCache;
@property BOOL ignoreDeviceCache;
@property NSInteger layoutID;
@property BOOL memoryDownload;
@property BOOL mbPreloadAtBackground;
@property BOOL needProgressNotification;
@property (nonatomic, copy) NSString* nodeKeyID;
@property (nonatomic, copy) NSString* nodeKeys;
@property BOOL shouldIgnoreCacheForSubsequents;
@property (nonatomic, copy) NSArray* dataSetIDs;
@property (nonatomic, strong) NSString* binDataPath;

-(BOOL)isCached;
@end
