//
//  PromptStepperCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptFormattableCell.h"

#define OPERATOR_FIELD_WIDTH 27
#define VALUE_SIDE_PADDING 2
#define STEPPER_CELL_RIGHT_PADDING 7

@class PromptStepperView, ConstantPrompt;

@interface PromptStepperCell : PromptFormattableCell {
	PromptStepperView *stepper;
}

+(PromptStepperCell*)newCellForPrompt:(ConstantPrompt*)prompt;
-(void)stepperValueChanged:(NSString *)newValue;

@end
