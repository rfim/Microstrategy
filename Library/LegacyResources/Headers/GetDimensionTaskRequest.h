//
//  GetDimensionTaskRequest.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 5/25/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface GetDimensionTaskRequest : MSITaskRequest {
	NSString* dimensionID;
}

@property (nonatomic, copy) NSString* dimensionID;

@end
