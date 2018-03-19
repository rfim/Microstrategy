//
//  MSIChangePasswordService.h
//  MicroStrategyMobile
//
//  Created by Liu Shu on 4/17/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSTRError;

@interface MSIChangePasswordService : MSIServiceImpl {
@private
	MSIProjectInfo* project;
	NSString* password;
	NSString* theNewPassword;
}

@property (nonatomic, strong) MSIProjectInfo* project;
@property (nonatomic, copy) NSString* password;
@property (nonatomic, copy) NSString* theNewPassword;

@end
