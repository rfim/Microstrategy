//
//  ExpressionHelper.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 2/23/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class OperatorNode;

@interface ExpressionHelper : NSObject {

}

+(NSInteger) resolveMRPFunctionFromFunction:(NSInteger)function property4:(NSString*)pr4;

+(void) updateNodeWithMRPFunction:(NSInteger)mrpFunction outerNode:(OperatorNode*)outerNode innerNode:(OperatorNode*)innerNode ascending:(BOOL)ascending;

@end
