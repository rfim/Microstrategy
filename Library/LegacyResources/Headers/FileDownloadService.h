//
//  FileDownloadService.h
//  iPadMain Library
//
//  Created by Amirali Charania on 3/8/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@interface FileDownloadService : MSIServiceImpl {
	NSString* url;
}

@property (nonatomic, copy) NSString* url;

@end
