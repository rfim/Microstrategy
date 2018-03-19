//
//  CreateFolderTaskRequest.h
//  MicroStrategyMobile
//
//  Created by lishan on 2/5/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"
@interface CreateFolderTaskRequest : MSITaskRequest {
    NSString* folderID;
    NSString* name;
    NSString* folderDesc;
}
@property (nonatomic,copy)NSString* folderID;
@property (nonatomic,copy)NSString* name;
@property (nonatomic,copy)NSString* folderDesc;

@end