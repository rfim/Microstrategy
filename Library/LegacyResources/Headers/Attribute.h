//
//  Attribute.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/18/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIDisplayInfo.h"

@class MSIAttributeForm;

@interface MSIAttribute : MSIDisplayInfo {
	
}

@property (nonatomic, strong, readonly) NSMutableArray* forms;
- (NSInteger)formCount;
- (MSIAttributeForm*)form:(NSInteger)index;
- (MSIAttributeForm*)formByID:(NSString*)formID;
- (NSInteger)indexOfForm:(NSString*)formID;

@end
