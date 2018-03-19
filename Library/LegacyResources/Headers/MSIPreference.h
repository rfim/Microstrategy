/*
 *  MSIPreference.h
 *  MSTR Foundation
 *
 *  Created by Amirali Charania on 2/24/09.
 *  Copyright 2009 MicroStrategy. All rights reserved.
 *
 */

#import "MSISerializable.h"

@class MSIDevicePreferences;

@protocol MSIPreference <NSCopying, MSISerializable>

-(void)setParent:(MSIDevicePreferences *)devicePreference;
@property (nonatomic, copy) NSString* prefID;


@end
