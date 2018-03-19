//
//  MassageBasedTaskRequest.h
//  MSTRMobileSDK
//
//  Created by Ilia Baskin on 5/27/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"


@interface MessageBasedTaskRequest : MSITaskRequest {
	NSString* messageID;
	BOOL locked;
}

@property (nonatomic, copy) NSString* messageID;

@end
