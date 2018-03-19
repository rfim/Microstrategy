//
//  GetCacheUpdateTimes.h
//  MSTRMobile
//
//  Created by Glenn Boysko on 4/16/10.
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MSITaskRequest.h"

@interface GetCacheUpdateTimes : MSITaskRequest {
@private
	NSString* tupleString;
	NSInteger flags;
}

@property (nonatomic, copy) NSString* tupleString;
@property NSInteger flags;
@end
