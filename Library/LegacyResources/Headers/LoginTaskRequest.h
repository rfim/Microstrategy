//
//  LoginTask.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface LoginTaskRequest : MSITaskRequest {
	NSString* serverName;
	NSString* projectName;
	NSString* userID;
	NSString* password;
	NSInteger port;
	NSInteger authMode;
	NSString* clientVersion;
	NSString* sessionKey;
	NSInteger clientType;
	NSString* deviceUUID;
    NSMutableDictionary * serverParameters;
    NSString *projectID;
}

@property (nonatomic, strong) NSString *projectID;
@property (nonatomic, copy) NSString* serverName;
@property (nonatomic, copy) NSString* projectName;
@property (nonatomic, copy) NSString* userID;
@property (nonatomic, copy) NSString* password;
@property NSInteger port;
@property NSInteger authMode;
@property (nonatomic, copy) NSString* clientVersion;
@property (nonatomic, copy) NSString* sessionKey;
@property NSInteger clientType;
@property (nonatomic, copy) NSString* deviceUUID;
@property (nonatomic, strong) NSMutableDictionary * serverParameters;

@end
