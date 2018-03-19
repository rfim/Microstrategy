//
//  ExpressionPrompt.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 2/17/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIPrompt.h"

@class MSIExpression;

@interface ExpressionPrompt : MSIPrompt {
	
@private
//	Expression* answer;
	MSIObjectInfo* origin;
	NSInteger expressionType;
}

//@property (nonatomic, retain) Expression* answer;
@property (nonatomic, strong) MSIObjectInfo* origin;
@property NSInteger expressionType;

@end
