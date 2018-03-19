//
//  RWDBinaryTaskRequest.h
//  MSTRMobile
//
//  Created by Ji Jin on 4/16/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface RWDBinaryTaskRequest : MSITaskRequest {
@private
	NSString* messageID;
    NSString* objectID;
	NSInteger resultFlags;
	NSInteger detailFlags;
	NSString* nodeKeys;
	NSString* definitionNodeKeys;
	NSInteger timeout;
	int binaryLimit;
	int orientation; // 0:unknown, 1:landscape, 2:portrait
    BOOL isSubscription;
    int objectType;
    NSString* updateKeys;
    BOOL useBackgroundTranser;
}

@property (nonatomic, copy) NSString* messageID;
@property (nonatomic, copy) NSString* objectID;
@property (nonatomic, copy) NSString* nodeKeys;
@property (nonatomic, copy) NSString* definitionNodeKeys;
@property (nonatomic, copy) NSString* updateKeys;
@property (nonatomic) NSInteger resultFlags;
@property (nonatomic) NSInteger detailFlags;
@property (nonatomic) NSInteger timeout;
@property (nonatomic) int binaryLimit;
@property (nonatomic) int orientation;
@property (nonatomic) BOOL isSubscription;
@property (nonatomic) int objectType;
@property  BOOL useBackgroundTransfer;
-(NSString*)parametersString; //this could be overwritten for newsstand
@end