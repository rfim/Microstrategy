//
//  ObjectService.h
//  MSTRMobile
//
//  Created by Glenn Boysko on 4/20/10.
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MSIServiceImpl.h"

@class ObjectStore;

@interface ObjectService : MSIServiceImpl {
@private
	ObjectStore* objectStore;
	NSInteger namedFolder;
	NSString* folderID;
}

@property (nonatomic, strong) ObjectStore* objectStore;
@property (nonatomic) NSInteger namedFolder;
@property (nonatomic, copy) NSString* folderID;
@property (nonatomic, getter = isRefreshing) bool refreshing;

@end
