//
//  BucketHierarchyElement.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 7/6/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BucketElement.h"

@class MSIElement;

@interface BucketHierarchyElement : BucketElement {
	MSIElement* element;
}

- (id)initWithElement:(MSIElement*)_element;

@property (nonatomic, readonly) MSIElement* element;

@end
