//
//  ConstantPrompt.h
//  MSTRMobile
//
//  Created by Quality Engineering on 2/8/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"
#import "MSIPrompt.h"


/**
	Also known as a Value prompt, MSIConstantPrompt covers Date & Time, various Numeric, Text and Big Decimal. 
 */
@interface MSIConstantPrompt : MSIPrompt
{
	DataTypes	mAnswerDataType;
}

@property DataTypes answerDataType;


- (id) copyWithZone: (NSZone*) zone;

/**
 Creates a ConstantPromptAnswer according to the given data type, i.e. for Numeric, String, DateTime and BigDecimal.
 @param	dataType	One of DataTypeBigDecimal, MSIDateTimePromptAnswer, DataTypeNumeric, DataTypeFloat, DataTypeReal, DataTypeDouble, DataTypeDecimal, DataTypeChar
 
 @return	a subclass of ConstantPromptAnswer, not having an initial value.
 
 @see DataTypes, MSINumericPromptAnswer, MSIStringPromptAnswer, MSIDateTimePromptAnswer, MSIBigDecimalPromptAnswer
 */
- (MSIPromptAnswer*) newAnswerWithType: (DataTypes) dataType;

/**
 The answer should be a subclass of ConstantPromptAnswer, for Numeric, String, DateTime and BigDecimal.
 
 @see NumericPromptAnswer, MSIStringPromptAnswer, MSIDateTimePromptAnswer, MSIBigDecimalPromptAnswer
 */
- (MSIPromptAnswer*) answer;

/**
 The answer should be a subclass of ConstantPromptAnswer, for Numeric, String, DateTime and BigDecimal.
 Setting this does not trigger any responses, such as re-running a report or sending to the server.
 
 @see NumericPromptAnswer, MSIStringPromptAnswer, MSIDateTimePromptAnswer, MSIBigDecimalPromptAnswer
 */
- (void) setAnswer: (MSIPromptAnswer*) newAnswer;

- (BOOL) hasAnswer;

-(NSNumber*)valueAsNumber;
-(NSNumber*)minAsNumber;
-(NSNumber*)maxAsNumber;

-(NSDate*)valueAsDate;
-(NSDate*)minAsDate;
-(NSDate*)maxAsDate;

@end
