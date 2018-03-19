//
//  ImageCache.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 1/11/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

/**
 * Class renamed from GridWidgetImageCache to MSIImageCache since the class is used to cache any images
 */

#import <Foundation/Foundation.h>
#import "Service.h"
#import "ImageCacheCallback.h"
#import	"ImageInfo.h"

#define IMAGE_INDEX_FILE_NAME @"imageFileIndex.dat"
#define CACHE_KEY_FILE_NAME @"imageCacheKeys.dat"
#define IMAGES_FOLDER_NAME @"Images"
#define MAX_SERVICES_NUM 10
#define GUID_KEY_PREFIX @"GUID"

@class ReportInfo, ImageInfo, MSIProjectInfo, FileCache;

@interface MSIImageCache : NSObject <ServiceHandler> {
	NSMutableArray *activeRequests;
    NSMutableDictionary *allRequests;           //maps the url to a MutableArray which store all RequestsUnit with same url
	NSMutableDictionary *reportToCacheIndexMap; //maps the subscription id to the cache index file which stores the next number to use while renaming image files, etc. Image0, Image1
	NSMutableDictionary *reportToCacheInfoMap; //maps the subscription id to the another nsmutabledictionary which itself stores the mappings from URL to the image file name
	NSMutableDictionary *failedRequests; //TQMS:407735 Holds the url and subscriptionid of failed requests to get images from server.
    //added by gyu
	NSMutableDictionary *urlToServiceMapActivePool;
	NSMutableDictionary *urlToServiceMapWaitingPool;
	NSMutableDictionary *toBeCncledRequests;
    NSMutableDictionary *urlToImageDict;
    NSMutableDictionary *imageRefCount;
	NSString *mBaseUrl;
    
    dispatch_queue_t       mBackgroudQueue;
}

+(MSIImageCache *)getImageCache;
+(BOOL)isImageUrlForHighResolution:(NSString*)url;
-(void)getImage:(NSString *)imageUrl callback:(id<ImageCacheCallback>) cback project:(MSIProjectInfo*)projectInfo;
-(void)getImageForGridCell:(NSString *)imageUrl callback:(id<ImageCacheCallback>) cback project:(MSIProjectInfo*)projectInfo;
-(void)getImageWithInfo:(ImageInfo*)imgInfo callback:(id<ImageCacheCallback>) cback project:(MSIProjectInfo*)projectInfo async:(BOOL)async;
-(void)cancelRequestAndService:(NSString *)url callback:(id)cbk;

- (UIImage*) cacheImageForUrl:(NSString*) url callback:(id) cback project:(MSIProjectInfo*)projectInfo checkLocalOnly:(BOOL)check;
//clear all inMemory Cache
- (void)clearInMemoryCache;

//delete caches?
-(void)deleteReportImageCaches:(ReportInfo *)report;
-(void)deleteCachesWithSubscriptionId:(NSString *)subscriptionId;
-(void)deleteDrillImageCaches;

//get time stamp
+(NSString*) hGetCurrentTimeStamp;
@end
