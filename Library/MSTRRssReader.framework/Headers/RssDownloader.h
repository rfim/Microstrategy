//
//  RssDownloader.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 4/28/10.
//  Copyright 2010 EastBanc Technologies All rights reserved.
//
#import <Foundation/Foundation.h>

#import "Service.h"
//#import "FileDownloadService.h"

@protocol MSMDownloaderDelegate;
@class FileDownloadService;
/**
   Downloads and stores data from feed's url
 */
@interface MSMRssDownloader : NSObject<ServiceHandler> {

}

@property (nonatomic, strong) id descriptor;
//Replace below NSURLConnection with NSURLSession if it is used again. Rally Issue US24445
@property (nonatomic, strong) NSURLConnection * feedConnection;
//@property (nonatomic, retain) NSMutableData * feedData;
@property (nonatomic, unsafe_unretained) id<MSMDownloaderDelegate> delegate;

-(id) initWithUrl:(NSString *)feedUrl delegate:(id<MSMDownloaderDelegate>)_delegate;
-(id) initWithUrl:(NSString *)feedUrl delegate:(id<MSMDownloaderDelegate>)_delegate descriptor:(id)_descriptor;
-(void) cancelDownload;

@end


@protocol MSMDownloaderDelegate

- (void) downloadCompleted:(NSData *)data withDescriptor:(id) descriptor;
- (void) downloadFailedWithError:(NSError *)error withDescriptor:(id)descriptor;

@end