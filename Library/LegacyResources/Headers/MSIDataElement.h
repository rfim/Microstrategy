//
//  DataElement.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/19/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIHeaderValue.h"

/**
 Abstract superclass for non-metric (attribute, custom group or consolidation) header value.
 */
@interface MSIDataElement : MSIHeaderValue {
	NSString* elementID;
    //we use int instead of Axis because Enum.h is not a public header file
    int axis;
}
@property (nonatomic, strong) NSString* elementID;
@property (nonatomic) int axis;
- (NSString*)elementIDWithoutAttributeID;

@end
