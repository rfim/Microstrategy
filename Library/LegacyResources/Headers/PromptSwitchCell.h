//
//  PromptSwitchCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"

#define SWITCH_WIDTH 100;
#define SWITCH_CELL_HEIGHT 44

@class ConstantPrompt;

@interface PromptSwitchCell : PromptBaseCell {
	UISwitch *valueSwitch;
	NSString *onValue;
	NSString *offValue;
}

+(PromptSwitchCell*)newCellForPrompt:(ConstantPrompt*)prompt;
@end
