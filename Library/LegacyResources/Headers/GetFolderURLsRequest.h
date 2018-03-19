//
//  GetFolderURLsRequest.h
//  MicroStrategyMobile
//
//  Created by Yuqiang Huang on 1/12/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"
@interface GetFolderURLsRequest : MSITaskRequest {
@private
NSString* xmlString;
//UIBackgroundTaskIdentifier bgTask;
}

@property (nonatomic, copy) NSString* xmlString;

@end
