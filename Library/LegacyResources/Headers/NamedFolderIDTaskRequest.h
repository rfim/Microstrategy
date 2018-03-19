//
//  NamedFolderIDTaskRequest.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface NamedFolderIDTaskRequest : MSITaskRequest {
	NSInteger systemFolder;
}

@property NSInteger systemFolder;

@end

