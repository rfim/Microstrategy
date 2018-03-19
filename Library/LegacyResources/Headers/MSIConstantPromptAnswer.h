//
//  ConstantPromptAnswer.h
//  MicrostrategyMobile
//
//  Created by Bruce Lawton on 2/2/12.
//  Copyright 2012 Microstrategy. All rights reserved.
//

#import "Enums.h"
#import "MSIPromptAnswer.h"

/**
 All instances will be a subclass of ConstantPromptAnswer, for Numeric, String, DateTime and BigDecimal.
 The values can be accessed via stringValue, or to avoid loss of precision, values can be accessed more directly. For example, using setDoubleValue: and doubleValue accessors of a NumericPromptAnswer instance.
 
 @see DataTypes, MSINumericPromptAnswer, MSIStringPromptAnswer, MSIDateTimePromptAnswer, MSIBigDecimalPromptAnswer

 */
@interface MSIConstantPromptAnswer : MSIPromptAnswer
{
	DataTypes	mDataType;
	NSString*	mValueStr;
}

@property DataTypes dataType;

- (id) initWithDataType: (DataTypes) dataType;



@end
