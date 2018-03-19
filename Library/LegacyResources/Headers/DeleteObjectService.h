//
//  DeleteObjectService.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 9/21/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"
#import "Enums.h"

@class MSIProjectConfiguration;

@interface DeleteObjectService : MSIServiceImpl {
	NSString* objectID;
	ObjectTypes objectType;
	MSIProjectConfiguration* project;
}

@property (nonatomic, copy) NSString* objectID;
@property ObjectTypes objectType;
@property (nonatomic, strong) MSIProjectConfiguration* project;

@end
