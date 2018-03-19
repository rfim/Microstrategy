//
//  XMLAPITaskRequest.h
//  IPadMain
//
//  Created by Wei Guo on 5/10/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface XMLAPITaskRequest : MSITaskRequest {

@private
	NSString* xmlString;
	NSInteger resultFlags;
}

@property (nonatomic, copy) NSString* xmlString;
@property (nonatomic) NSInteger resultFlags;

@property (nonatomic, assign) BOOL shouldReceiveBinaryData;

@end
