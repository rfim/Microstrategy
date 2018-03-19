//
//  AttributeForm.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/18/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIDisplayInfo.h"

@class MSIAttribute;

@interface MSIAttributeForm : MSIDisplayInfo {
	
}

@property (nonatomic, weak, readonly) MSIAttribute* attribute;
@property NSInteger dataType;
@property (nonatomic, copy) NSString* controlPropertyFormatString;

- (NSInteger) baseFormID;
- (NSInteger) baseFormType;

/**
	Returns attributeId:attFormId which uniquely defines an attribute form
 **/	
- (NSString*)uniqueId;

- (NSString*)attributeId;

- (NSString*) baseFormDisplayID;

- (NSString*) baseFormDisplayName;


@end
