//
//  IPadPromptHelper.h
//  MicroStrategyMobile
//
//  Created by Firat Alpergin on 2/23/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PromptHelper.h"

@class Prompts, IPadWaitView;

@interface IPadPromptHelper : PromptHelper {
	@private
		IPadWaitView* waitView;
}

- (id)initWithPrompts:(Prompts*)prompts;

@end
