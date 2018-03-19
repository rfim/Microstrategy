//
//  CloseSessionsTask.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface CloseSessionsTaskRequest : MSITaskRequest {
	NSString* sessionStates;
	WebServerConfiguration* webServer;
}

@property (nonatomic, copy) NSString* sessionStates;
@property (nonatomic, retain) WebServerConfiguration* webServer;


@end
