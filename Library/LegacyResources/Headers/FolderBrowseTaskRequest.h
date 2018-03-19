//
//  FolderBrowseTaskRequest.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface FolderBrowseTaskRequest : MSITaskRequest {
	NSString* folderID;
	NSString* typeRestriction;
	NSString* styleName;
	NSInteger executionFlags;
	NSInteger systemFolder;
	NSInteger folderBlockBegin;
	NSInteger folderBlockCount;
	BOOL includeObjectDesc;
	BOOL dereferenceShortcuts;
}

@property (nonatomic, copy) NSString* folderID;
@property (nonatomic, copy) NSString* typeRestriction;
@property (nonatomic, copy) NSString* styleName;
@property NSInteger executionFlags;
@property NSInteger systemFolder;
@property NSInteger folderBlockBegin;
@property NSInteger folderBlockCount;
@property BOOL includeObjectDesc;
@property BOOL dereferenceShortcuts;

@end