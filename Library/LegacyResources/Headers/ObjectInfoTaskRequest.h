//
//  ObjectInfoTaskRequest.h
//  MSTRMobile
//
//  Created by Amirali Charania on 4/5/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface ObjectInfoTaskRequest : MSITaskRequest {
	NSString* objectIDs;
	NSString* objectTypes;
    bool includeParentFolder;
}

@property (nonatomic, copy) NSString* objectIDs;
@property (nonatomic, copy) NSString* objectTypes;
@property (nonatomic) bool includeParentFolder;

@end
