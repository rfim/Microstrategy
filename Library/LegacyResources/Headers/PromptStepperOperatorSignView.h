//
//  PromptStepperOperatorSignView.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PromptStepperOperatorView;

@interface PromptStepperOperatorSignView : UIView {
	NSInteger type;
	PromptStepperOperatorView *parent;
}

@property NSInteger type;
@property (nonatomic, assign) PromptStepperOperatorView *parent;

@end
