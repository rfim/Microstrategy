//
//  IntegerPreference.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/24/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIPreference.h"

@class MSIDevicePreferences;

@interface IntegerPreference : NSObject <MSIPreference>{
@private
	NSString* prefID;
	NSInteger value;
	__weak MSIDevicePreferences * parent;
}

@property (nonatomic, copy) NSString* prefID;
@property (nonatomic)NSInteger value;//lishan nonatomic

-(id)initWithID:(NSString*)_prefID value:(NSInteger)_value;

@end
