//
//  WidgetPropManipulationProtocal.h
//  MicroStrategyMobile
//
//  Created by Zhang, Yuan on 7/31/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
@class WidgetPropTreeNode;

@protocol WidgetPropManipulationProtocal <NSObject>
@optional
- (BOOL)clearAll;
- (void)changePropertyWithNode:(WidgetPropTreeNode*)iNode;
@required
- (void)changePropertWithPropertyInfo:(void *)propertyInfo;
@end
