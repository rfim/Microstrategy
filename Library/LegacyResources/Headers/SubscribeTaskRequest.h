//
//  SubscribeTaskRequest.h
//  MSTRMobileSDK
//
//  Created by Srinivas Singanamalla on 9/20/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import	"MessageBasedTaskRequest.h"

@interface SubscribeTaskRequest : MessageBasedTaskRequest {
	
@private
	NSString* objectID;
	NSInteger objectType;
	NSInteger stateID;	
	NSString* promptAnswers;
	NSString* triggerID;
	NSString* pagebyKeys;
	
	//ipad or iphone
	NSInteger deviceType;
	NSString* subscriptionName;
}

@property (nonatomic, copy) NSString* objectID;
@property NSInteger objectType;
@property NSInteger stateID;
@property (nonatomic,copy) NSString* promptAnswers;
@property (nonatomic, copy) NSString* triggerID;
@property (nonatomic, copy) NSString* subscriptionName;
@property (nonatomic, copy) NSString* pagebyKeys;
@property NSInteger deviceType;

@end
