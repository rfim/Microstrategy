//
//  SaveSubscriptionTaskRequest.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 2/11/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface SaveSubscriptionTaskRequest : MSITaskRequest {
	
@private
	NSString* subscriptionID;
	NSString* parentSubscriptionID;
	NSString* promptAnswerXML;
	NSString* subscriptionName;
}

@property (nonatomic, copy) NSString* subscriptionID;
@property (nonatomic, copy) NSString* parentSubscriptionID;
@property (nonatomic, copy) NSString* promptAnswerXML;
@property (nonatomic, copy) NSString* subscriptionName;

@end
