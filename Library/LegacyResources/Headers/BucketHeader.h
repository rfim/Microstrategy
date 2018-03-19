//
//  BucketHeader.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 7/5/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIObjectInfo;

@interface BucketHeader : NSObject {
	MSIObjectInfo* headerObject;
    BOOL inList;
}

@property (nonatomic, strong) MSIObjectInfo* headerObject;
@property (nonatomic) BOOL inList;
- (id)initWithObject:(MSIObjectInfo*)_headerObject;
- (NSString*)name;

@end
