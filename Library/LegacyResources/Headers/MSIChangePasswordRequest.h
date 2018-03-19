//
//  MSIChangePasswordRequest.h
//  MicroStrategyMobile
//
//  Created by Liu Shu on 4/17/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface MSIChangePasswordRequest : MSITaskRequest {
@private
	NSString* serverName;
	NSString* userID;
	NSString* password;
	NSString* theNewPassword;
}

@property (nonatomic, copy) NSString* serverName;
@property (nonatomic, copy) NSString* userID;
@property (nonatomic, copy) NSString* password;
@property (nonatomic, copy) NSString* theNewPassword;

@end
