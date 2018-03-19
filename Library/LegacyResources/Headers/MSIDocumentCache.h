//
//  MSMDocumentCache.h
//  MicroStrategyMobile
//
//  Created by Zhang, Yuan on 3/14/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSICache.h"
#import "MSIProjectInfo.h"

@interface MSIDocumentCache : MSICache

@property (nonatomic, readonly, copy) NSString* documentId;
@property (nonatomic, readonly, copy) MSIProjectInfo* projectInfo;

@end
