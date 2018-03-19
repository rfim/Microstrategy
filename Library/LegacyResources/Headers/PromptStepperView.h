//
//  PromptStepperView.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OperatorCallback.h"

@class PromptStepperOperatorView, PromptStepperLabelView, PromptStepperCell;

@interface PromptStepperView : UIView <OperatorCallback> {
	PromptStepperOperatorView *minusView;
	PromptStepperOperatorView *plusView;
	PromptStepperLabelView *valueLabel;
	PromptStepperCell *parent;
	
	float value;
	float increment;
	float max;
	float min;
	BOOL minSet;
	BOOL maxSet;
}

@property float value;
@property float increment;
@property float max;
@property float min;
@property (nonatomic, assign) PromptStepperCell *parent;

-(id)initWithFrame:(CGRect)frame value:(float)val parent:(PromptStepperCell *)par;
-(void)operatorTouchesBegan:(PromptStepperOperatorView *)operatorView;
-(void)setValueLabelValue:(float)newValue;

@end
