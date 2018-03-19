//
//  ReportInfo.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIReportDisplayInfo.h"
#import "CacheInfo.h"
//#import "ReportCacheMap.h"

@class ReportCacheMap, CacheIDMap;

@interface ReportInfo : MSIReportDisplayInfo {
@private
	ReportCacheMap* cacheMap;
	BOOL readCacheInfoEntries;
	CacheIDMap* cacheIDToFilenameMap;
	//The cache set path of the last successfully saved cache item
	//We compare it with the cache set path of the cache item being
	//saved. If they are equal then we know that the corresponding 
	//folder exists.
	NSString* cacheSetPath;
}

// internal methods meant for report store usage
-(void)populate:(NSData*)xmlData omitCacheInfo:(BOOL)omitCacheInfo;

// Class level methods...
+(void)deleteAllReportCachesDir;

@end
