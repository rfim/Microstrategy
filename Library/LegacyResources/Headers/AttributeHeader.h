//
//  AttributeHeader.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/19/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ElementHeader.h"

@class MSIAttribute, MSIAttributeForm;

@interface MSIAttributeHeader : MSIElementHeader {

}

/**
 The attribute object that this header represents.
 */
@property (nonatomic, strong, readonly) MSIAttribute* attribute;

/**
 The attribute form object that this header represents.
*/
@property (nonatomic, strong, readonly, getter=getAttributeForm) MSIAttributeForm* attributeForm;

- (NSInteger) baseFormID;

- (NSInteger) baseFormType;

@end
