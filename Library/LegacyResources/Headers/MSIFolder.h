//
//  Folder.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 2/19/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIObjectInfo.h"

@interface MSIFolder : MSIObjectInfo {

@private
	NSMutableArray* children;
}

@property(nonatomic, strong) NSMutableArray* children;

@end
