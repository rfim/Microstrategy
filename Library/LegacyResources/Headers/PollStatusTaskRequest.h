//
//  PollStatusTaskRequest.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 8/21/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MessageBasedTaskRequest.h"

@interface PollStatusTaskRequest : MessageBasedTaskRequest {
	@private 
	NSInteger resultSetType;
}

@property NSInteger resultSetType;

@end
