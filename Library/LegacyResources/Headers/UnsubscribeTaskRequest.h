//
//  UnsubscribeTaskRequest.h
//  MSTRMobileSDK
//
//  Created by Srinivas Singanamalla on 9/20/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import	"MSITaskRequest.h"

@interface UnsubscribeTaskRequest : MSITaskRequest {
@private
	NSString* subscriptionID;
}

@property (nonatomic, copy) NSString* subscriptionID;

@end
