//
//  StringPromptAnswer.h
//  MicroStrategyMobile
//
//  Created by Bruce Lawton on 2/2/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#import "MSIConstantPromptAnswer.h"


@interface MSIStringPromptAnswer : MSIConstantPromptAnswer

- (NSString*) stringValue;
- (void) setStringValue: (NSString*) stringValue;

@end
