//
//  InboxMessageTimeTaskRequest.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 8/26/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface InboxMessageTimeTaskRequest : MSITaskRequest {
	@private 
	NSString *messageId;
}

@property(nonatomic, copy) NSString *messageId;

@end
