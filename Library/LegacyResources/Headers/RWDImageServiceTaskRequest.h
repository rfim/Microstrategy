//
//  RWDImageServiceTaskRequest.h
//  MSTRMobile
//
//  Created by Mingjun Zhu on 5/19/2010.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface RWDImageServiceTaskRequest : MSITaskRequest {
@private
	NSArray* imagesArray;
	NSArray* imagesTimeStampArray;
    NSString* objectID;
}

// @property (nonatomic, copy) NSString* xmlRequest;
@property (nonatomic, strong) NSString* objectID;

- (void)setImagesArray:(NSArray*)iImagesArray forTimeStamp:(NSArray*)iImagesTimeStampArray;
@end