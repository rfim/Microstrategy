//
//  DSSImageCacheManager.h
//  ReportServiceCore
//
//  Created by Mingjun Zhu on 5/27/10.
//  Copyright 2010 MSTR. All rights reserved.
//

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif
#import <UIKit/UIImage.h>


@interface DSSImageCacheManager : NSObject {
	
}

// +(void) getImageNamesFromDocumentInstance:(DSSDocumentInstance*)ipDocInstance imageNames:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray;
+(void) getImageNamesFromDocumentData:(NSString*)ipDataPath imageNames:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray;
+(BOOL) isHTTPImage:(NSString *)ipPath;
+(BOOL) isAbsolutePath:(NSString *)ipPath;
+(BOOL) isDifferentFromLocalFile:(NSString*) ipImagePath forTimeStamp:(NSDate*) ipImageTimeStamp;

+ (unichar) getValidFileNameChar:(unichar) ipChar;
+ (NSString*) convertUrlToFileName:(NSString*) ipUrlPath;
+ (NSString*) convertWindowsMacFileName:(NSString*) ipFileName;
+ (NSString*) getLocalImagesHomePath;
+ (NSString*) getHttpImagesHomePath;
+ (NSString*) getAbsoluteImagesHomePath;

+ (NSString*) getLocalImagePath:(NSString*) ipPath;
+ (NSData*) getLocalUIImageByPath:(NSString*) ipPath;
+(NSData*) getLocalUIImageByPath:(NSString*)ipPath autoRelease:(BOOL) isAutoReleased;
+ (UIImage*) getLocalUIImageFromDisk:(NSString*) ipPath;
+ (NSString*) getHttpImagePath:(NSString*) ipUrlPath;
+ (NSData*) getHttpUIImageByPath:(NSString*) ipUrlPath;
+ (NSData*) getHttpUIImageByPath:(NSString*) ipUrlPath autoRelease:(BOOL) isAutoReleased;
+ (UIImage*) getHttpUIImageFromDisk:(NSString*) ipPath;

+ (void) saveHttpUIImage:(NSData*) ipImageData forPath:(NSString*) ipUrlPath forTimeStamp:(NSString*)ipTimeStamp;
+ (void) saveLocalUIImage:(NSData*) ipImageData forPath:(NSString*) ipPath forTimeStamp:(NSString*)ipTimeStamp;

// mzhu 2011-03-11: TQMS 455114: add support to save a place holder image.  Details will be explained more in implementation
+ (void) saveHttpPlaceHolderImage:(NSData*) ipImageData forPath:(NSString*) ipPath;
+ (void) saveLocalPlaceHolderImage:(NSData*) ipImageData forPath:(NSString*) ipPath;
+ (void) checkPlaceHolderImage:(NSString*) ipFinalImagePath;
+ (void) initPlaceHolderTime;
// + (void) releaseAppStartTime;

+ (void) setFileTimeStamp:(NSString*)ipTimeStamp  forPath:(NSString*) ipPath;
+ (void) compareLocalImages: (NSMutableArray*) ipPathsArray forTimeStamp: (NSMutableArray*) ipTimeStampArray missingImages: (NSMutableArray*)opMissingImagesArray;
+ (NSString*) getCacheFolderPath;

@end

