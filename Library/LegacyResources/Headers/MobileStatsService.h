//#FEATURE#Tools:Mobile Statistics
//
//  MobileStatsService.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 6/14/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"

@interface MobileStatsService : XMLAPIService

@property (nonatomic, copy) NSString* command;

@end
