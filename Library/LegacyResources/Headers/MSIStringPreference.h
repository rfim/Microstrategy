//
//  MSIStringPreference.h
//  MSTRMobile
//
//  Created by Amirali Charania on 1/9/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIPreference.h"

@class MSIDevicePreferences;

@interface MSIStringPreference : NSObject <MSIPreference> {
@private
	NSString* prefID;
	NSString* stringValue;
	MSIDevicePreferences * __weak parent;
}

@property (nonatomic, copy) NSString* prefID;
@property (nonatomic, copy) NSString* stringValue;

-(id) initWithID:(NSString*)_prefID;

@end
