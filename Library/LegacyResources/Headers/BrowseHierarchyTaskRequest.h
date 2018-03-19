//
//  BrowseHierarchyTaskRequest.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface BrowseHierarchyTaskRequest : MSITaskRequest {
	NSString* browsePath;
	NSString* searchPattern;
	NSInteger matchCase;
	NSInteger blockBegin;
	NSInteger blockCount;
}

@property (nonatomic, copy) NSString* browsePath;
@property (nonatomic, copy) NSString* searchPattern;
@property NSInteger matchCase;
@property NSInteger blockBegin;
@property NSInteger blockCount;

@end
