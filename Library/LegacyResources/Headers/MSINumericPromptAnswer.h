//
//  NumericPromptAnswer.h
//  MicroStrategyMobile
//
//  Created by Bruce Lawton on 2/2/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import "MSIConstantPromptAnswer.h"




@interface MSINumericPromptAnswer : MSIConstantPromptAnswer
{
	NSNumber*	mNumericValue;
}

- (NSString*) stringValue;
- (void) setStringValue: (NSString*) newStringValue;

- (float) floatValue;
- (void) setFloatValue: (float) newFloatValue;
- (double) doubleValue;
- (void) setDoubleValue: (double) newDoubleValue;
- (long) longValue;
- (void) setLongValue: (long) newLongValue;
- (bool) boolValue;
- (void) setBoolValue: (bool) newBoolValue;

@end
