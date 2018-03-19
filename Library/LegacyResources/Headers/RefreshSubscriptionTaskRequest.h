//
//  RefreshSubscriptionTaskRequest.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 8/21/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface RefreshSubscriptionTaskRequest : MSITaskRequest {
	@private
	NSString *subscriptionId;
	NSString* parentSubscriptionId;
	NSInteger timeout;
	NSInteger pollingFrequency;
	BOOL checkExistingMessage;
	BOOL forceRefresh;
}

@property(nonatomic, copy) NSString *subscriptionId;
@property(nonatomic, copy) NSString *parentSubscriptionId;
@property NSInteger timeout;
@property NSInteger pollingFrequency;
@property BOOL checkExistingMessage;
@property BOOL forceRefresh;

@end
