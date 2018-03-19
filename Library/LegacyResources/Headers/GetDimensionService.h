//
//  GetDimensionService.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 5/25/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectConfiguration;

@interface GetDimensionService : MSIServiceImpl {
	NSString* dimensionID;
	MSIProjectConfiguration* project;
}

@property (nonatomic, copy) NSString* dimensionID;
@property (nonatomic, strong) MSIProjectConfiguration* project;

@end
