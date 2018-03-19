//
//  MSIIsSessionAliveService.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 11/19/15.
//  Copyright (c) 2015 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

typedef enum _IsSessionAliveResult {
	SessionAliveUnknown = -1, SessionAliveNo = 0, SessionAliveYes = 1
} IsSessionAliveResult;

@interface MSIIsSessionAliveService : MSIServiceImpl{
    
}

@property (nonatomic, copy) NSString* sessionState;
@property (nonatomic, retain) MSIProjectConfiguration* projectConfig;
@property (nonatomic, retain) MSIProjectInfo* project;
@property (nonatomic, readonly) IsSessionAliveResult isSessionAliveResult;

@end
