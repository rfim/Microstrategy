//
//  ImageService.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/23/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo;

//PDF Reader will reuse this service, it need to know the file's modification time
@protocol ImageServiceCallback
-(void)lastModifiedTimeStamp:(NSString*)timeStamp;
@end

@interface ImageService : MSIServiceImpl {
	NSString *imageURL;
	NSString* imageKey;
	NSString *subscriptionId;
	MSIProjectInfo *projectInfo;
    //PDF Reader will reuse this service, it need progress notify
    BOOL needProgress;
}

@property (nonatomic, copy) NSString* imageKey;
@property (nonatomic, copy) NSString *imageURL;
@property (nonatomic, copy) NSString *subscriptionId;
@property (nonatomic, strong) MSIProjectInfo *projectInfo;
@property (nonatomic) BOOL needProgress;

@end