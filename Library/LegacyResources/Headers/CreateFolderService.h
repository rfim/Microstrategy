//
//  CreateFolderService.h
//  MicroStrategyMobile
//
//  Created by lishan on 28/4/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "MSIServiceImpl.h"

@interface CreateFolderService : MSIServiceImpl
@property (nonatomic,copy)NSString* folderID;
@property (nonatomic,copy)NSString* name;
@property (nonatomic,copy)NSString* folderDesc;
@property (nonatomic,strong) MSIProjectConfiguration* project;
@property (nonatomic, copy) NSString* sessionState;

@end
