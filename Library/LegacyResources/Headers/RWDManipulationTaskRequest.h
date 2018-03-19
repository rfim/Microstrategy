//
//  RWDManipulationTaskRequest.h
//  IPadMain
//
//  Created by Ji Jin on 4/25/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface RWDManipulationTaskRequest : MSITaskRequest {

@private
	NSString* messageID;
	//NSString* rwb;
	NSString* events;
}

@property (nonatomic, copy) NSString* messageID;
//@property (nonatomic, copy) NSString* rwb;
@property (nonatomic, copy) NSString* events;

@end