//
//  DuplicateMessageRequest.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 1/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MessageBasedTaskRequest.h"

@interface DuplicateMessageRequest : MessageBasedTaskRequest {
	NSInteger objectType;
}

@property NSInteger objectType;

@end
