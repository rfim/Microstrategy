//
//  RemoveJobService.h
//  MicroStrategyMobile
//
//  Created by temp on 11/17/11.
//  Copyright (c) 2011 MicroStrategy Inc. All rights reserved.
//

#import "XMLAPIService.h"

@class MSIReportDisplayInfo;

@interface RemoveJobService : XMLAPIService {
	NSString* messageID;
}

@property (nonatomic, copy) NSString* messageID;

@end
