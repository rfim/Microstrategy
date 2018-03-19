//
//  BigDecimalPromptAnswer.h
//  MicroStrategyMobile
//
//  Created by Bruce Lawton on 2/2/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import "MSIConstantPromptAnswer.h"


@interface MSIBigDecimalPromptAnswer : MSIConstantPromptAnswer
{
	long long	mMantissa;
	long		mScale;
}

/*
	The BigDecimal type is described here:
		http://docs.oracle.com/javase/1.5.0/docs/api/java/math/BigDecimal.html

 
	Simply put, it is an arbitrary precision integer mantissa and a scale (a.k.a. exponent)
	E.g. 1000 could be represented as 100 x10^1, where the mantissa and exponent are 100 and 1

 */

- (void) setStringValue: (NSString*) newValueStr;
- (void) setMantissa: (long long) mantissa andScale: (long) scale;

@end


