//
//  BooleanPreference.h
//  MSTRMobile
//
//  Created by Liu Shu on 4/24/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIDevicePreferences;

@interface BooleanPreference : NSObject {
@private
	NSString* prefID;
	BOOL booleanValue;
	MSIDevicePreferences * __weak parent;
}

@property (nonatomic, copy) NSString* prefID;
@property (nonatomic) BOOL booleanValue;

-(BooleanPreference*)initWithID:(NSString*)_prefID boolValue:(BOOL)_value;

@end
