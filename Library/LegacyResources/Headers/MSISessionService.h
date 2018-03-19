//
//  MSISessionService.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo, MSTRError, ErrorDetails;

@interface MSISessionService : MSIServiceImpl {
@private
	MSIProjectInfo* project;
	//added to support session creation with a given project configuration
	MSIProjectConfiguration* projectConfig;
	NSString* sessionState;
	BOOL hasMobileRWPrivilege;
	MSTRError* error;
	NSString* projectID;
	BOOL isCompatible;
	ErrorDetails* compatibilityError;
	BOOL ignoreAccessProjectError;
	NSString* webVersion;
	NSString* serverVersion;
	NSString* userID;
	BOOL logStats;
	BOOL logManipulations;
	BOOL collectLocation;
	BOOL usherPollStatusInvalid;
}

@property (nonatomic, strong) MSIProjectInfo* project;
@property (nonatomic, strong) MSIProjectConfiguration* projectConfig;
@property (nonatomic, copy) NSString* sessionState;
@property BOOL hasMobileRWPrivilege;
@property BOOL ignoreAccessProjectError;//Whether or not we should ignore privilege error, for now, whether project access errors are viewed as invalid credentials errors
@property BOOL shouldNotPutSessionStateIntoTable;
@property BOOL shouldUpdatePreferenceStoreWhenUsingProjectConfig;
@end
