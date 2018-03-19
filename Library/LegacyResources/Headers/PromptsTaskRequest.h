//
//  PromptsTaskRequest.h
//  MSTRMobile
//
//  Created by Quality Engineering on 2/3/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MessageBasedTaskRequest.h"

@interface PromptsTaskRequest : MessageBasedTaskRequest {
	NSInteger reportType;
	BOOL isResolved;
	BOOL includeFormats;
}

@property NSInteger reportType;
@property BOOL isResolved;
@property BOOL includeFormats;

@end