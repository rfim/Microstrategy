//
//  ChartDataTaskRequest.h
//  MSTRMobile
//
//  Created by xi zhang on 3/3/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MessageBasedTaskRequest.h"

@interface ChartDataTaskRequest : MessageBasedTaskRequest {
	NSInteger stateID;
	NSString* pageByKeys;	
}

@property NSInteger stateID;
@property (nonatomic, copy) NSString* pageByKeys;

@end

