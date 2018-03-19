//
//  PromptTextCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PromptBaseCell.h"

#define TEXT_CELL_HEIGHT 44

@class PromptBaseCell, ConstantPrompt;

@interface PromptTextCell : PromptBaseCell <UITextFieldDelegate> {
	UITextField *valueField;
}
+(PromptTextCell*)newCellForPrompt:(ConstantPrompt*)_prompt;

@end
