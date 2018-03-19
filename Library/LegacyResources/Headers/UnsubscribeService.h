//
//  UnsubscribeService.h
//  MSTRMobileSDK
//
//  Created by Srinivas Singanamalla on 7/13/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@interface UnsubscribeService : MSIServiceImpl {
	@private
	NSString* subscriptionID;
	NSString* parentSubscriptionID;
	MSIProjectInfo* projectInfo;
}

@property (nonatomic, copy) NSString* subscriptionID;
@property (nonatomic, copy) NSString* parentSubscriptionID;
@property (nonatomic, strong) MSIProjectInfo* projectInfo;

+ (UnsubscribeService*) newUnsubscribeService:(NSString*)subscriptionId withParentSubscriptionId:(NSString*)parentSubscriptionId withProjectInfo:(MSIProjectInfo*)projectInfo serviceHandler:serviceHandler;

@end
