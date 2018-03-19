//
//  ExpressionPromptAnswer.h
//  MicrostrategyMobile
//
//  Created by Bruce Lawton on 2/12/12.
//  Copyright 2012 Microstrategy. All rights reserved.
//

#import "Enums.h"
#import "MSIExpression.h"
#import "MSIPromptAnswer.h"

@interface MSIExpressionPromptAnswer : MSIPromptAnswer
{
	MSIExpression*		mValue;
}

@property (nonatomic, strong) MSIExpression*	expression;


//- (id) initWithDataType: (DataTypes) dataType;

//- (NSString*) stringValue;
//- (void) setStringValue: (NSString*) stringValue;
- (int)countofCondition;

@end
