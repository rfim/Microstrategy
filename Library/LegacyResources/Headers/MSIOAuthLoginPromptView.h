//
//  MSIOAuthLoginPromptView.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 5/27/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginPromptView.h"

typedef enum _PromptUIModes {
	PromptUIModeDefault = 0,
	PromptUIModeUsher = 2
} PromptUIModes;

#define MSI_AUTHENTICATION_CURRENT_UI_MODE_KEY @"currentUIMode"
#define MSI_AUTHENTICATION_USER_NAME_TEXT_KEY @"usernameText"

@interface MSIOAuthLoginPromptView : MSIMobileLoginPromptView

@end
