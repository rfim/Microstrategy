//
//  UploadImageService.h
//  MAMultimediaUploader
//
//  Created by Wang Jinde on 7/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo;

@interface MSIUploadImageService : MSIServiceImpl {
	MSIProjectInfo* mpProjectInfo;
	NSString* mpFileName;
	NSString* mpFilePath;
	NSString* mpSessionState;
}

@property (nonatomic, strong) MSIProjectInfo* mpProjectInfo;
@property (nonatomic, copy) NSString* mpFileName;
@property (nonatomic, copy) NSString* mpFilePath;
@property (nonatomic, copy) NSString* mpSessionState;

@end
