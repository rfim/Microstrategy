//
//  PromptStepperOperatorBackgroundView.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface PromptStepperOperatorBackgroundView : UIView {
	NSInteger type;
	BOOL isDisabled;
	BOOL currentlyTapped;
	NSInteger style;
	NSInteger alignment;
}

@property NSInteger type;
@property BOOL currentlyTapped;
@property BOOL isDisabled;
@property NSInteger style;
@property NSInteger alignment;

@end
