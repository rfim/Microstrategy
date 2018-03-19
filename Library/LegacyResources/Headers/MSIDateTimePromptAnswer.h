//
//  DateTimePromptAnswer.h
//  MicroStrategyMobile
//
//  Created by Bruce Lawton on 2/2/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import "MSIConstantPromptAnswer.h"


@interface MSIDateTimePromptAnswer : MSIConstantPromptAnswer
{
    @private
	NSDate*		mDateValue;
	BOOL		mIsDateType; // if NO, is DateTime type. We currently have only 2 types (date and datetime) for date prompt answer, if later we have more types, we can use FormatTypes in MSTRFormatter.h
}

- (NSDate*) value;
- (void) setValue: (NSDate*) newValue;

- (NSString*) stringValue;
- (void) setStringValue: (NSString*) stringValue;

- (BOOL) isDateType;
- (void) setDateType:(BOOL)isDateType;

@end
