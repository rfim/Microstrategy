//
//  DeleteObjectTaskRequest.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 9/21/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"
#import "Enums.h"

@interface DeleteObjectTaskRequest : MSITaskRequest {
	NSString* objectID;
	ObjectTypes objectType;
}

@property (nonatomic, copy) NSString* objectID;
@property ObjectTypes objectType;

@end
