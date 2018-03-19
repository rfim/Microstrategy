//
//  PromptSliderCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptFormattableCell.h"

#define SLIDER_CELL_HEIGHT 66
#define SLIDER_HEIGHT 20;
#define MIDDLE_PADDING 10 //between the title and value labels

@class ConstantPrompt, MSTRFormatter;

@interface PromptSliderCell : PromptFormattableCell {
	UISlider *slider;
	UILabel *valueField;

	float promptInterval; //will be read from the prompt
	float lastReadSliderValue;
	MSTRFormatter* formatter;
}

+(PromptSliderCell*)newCellForPrompt:(ConstantPrompt*)prompt;
-(void)setUpValues;
-(void)setValueFieldValue:(float)value;

@end
