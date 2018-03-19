//
//  RWRemoveInboxMsgs.h
//  iPadMain Library
//
//  Created by Ji Jin on 8/25/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"

@class MSIProjectInfo;

@interface RWRemoveInboxMsgs : XMLAPIService {
	NSArray* messages;
}

@property (nonatomic, strong) NSArray* messages;

@end
