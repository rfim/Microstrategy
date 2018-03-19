//
//  SearchMetaDataService.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 7/14/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectConfiguration;

@interface SearchMetaDataService : MSIServiceImpl {
	NSString* searchXML;
	MSIProjectConfiguration* project;
	NSInteger blockBegin;
	NSInteger blockCount;
	NSString* searchPattern;

}

@property (nonatomic, strong) MSIProjectConfiguration* project;
@property (nonatomic, copy) NSString* searchXML;
@property NSInteger blockBegin;
@property NSInteger blockCount;
@property (nonatomic, copy) NSString* searchPattern;

@end
