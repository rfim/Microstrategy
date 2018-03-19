//
//  UploadImageTaskRequest.h
//  MAMultimediaUploader
//
//  Created by Wang Jinde on 7/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import "MSITaskRequest.h"

@interface UploadImageTaskRequest : MSITaskRequest {
	NSString* mpFileName;
}

@property (nonatomic,copy) NSString* mpFileName;

@end
