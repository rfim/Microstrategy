//
//  DefaultProjPropService.h
//  iPadMain Library
//
//  Created by Ji Jin on 2/23/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"
#import "MSTRHandler.h"

@class MSIProjectInfo;
@class MSIProjectConfiguration;
@class MSIDevicePreferences;

@interface DefaultProjPropService : MSIServiceImpl <MSTRHandler> {
@protected
	MSIProjectInfo* project;
	NSString* emptyMessage;
	BOOL displayEmptyMsg;
	BOOL ignoreAccessProjectError;
}

@property (nonatomic, strong) MSIProjectInfo* project;
@property (nonatomic, strong) NSString* emptyMessage;
@property BOOL displayEmptyMsg;
@property BOOL ignoreAccessProjectError;//Whether or not we should ignore privilege error, for now, whether project access errors are viewed as invalid credentials errors
@property BOOL drillingWithFullBinary;

@end