//
//  PromptStepperOperatorView.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define SIDE_IMAGE_WIDTH 5
#define CENTER_IMAGE_WIDTH 1
#define SIGN_PADDING 7 //padding from all 4 side of the button while drawing the operator image
#define SIGN_WIDTH 2

#define OPERATOR_MINUS 0
#define OPERATOR_PLUS 1

#define VIEW_STEPPER_STYLE 0 //uses blue stepper images
#define VIEW_BUTTON_STYLE 1 //uses black system button images

#define LEFT_OPERATOR 0
#define RIGHT_OPERATOR 1

@class PromptStepperView, PromptStepperOperatorBackgroundView, PromptStepperOperatorSignView, OperatorCallback;

@interface PromptStepperOperatorView : UIView {
	NSInteger type;
	BOOL currentlyTapped;
	
	PromptStepperOperatorBackgroundView *backgroundView;
	PromptStepperOperatorSignView *operatorSignView;
	OperatorCallback *parent;
}

@property NSInteger type;
@property (nonatomic, assign) OperatorCallback *parent;

-(id)initWithFrame:(CGRect)frame type:(NSInteger)operatorType style:(NSInteger)viewStyle alignment:(NSInteger)alignmentType;
-(void)updateOperatorView;
-(void)setDisabled:(BOOL)disabled;
-(BOOL)isDisabled;
-(void)setTapped:(BOOL)isTapped;

@end
